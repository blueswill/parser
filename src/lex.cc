#include"lex.hpp"

namespace parser {
    const char *not_valid_grammer::what() const throw() {
        return "lex prammer is not valid";
    }

    template<typename Iter, typename StartIter>
    lex::lex(Iter start, Iter end, StartIter token_start, StartIter token_end) {
        bool checked = false;
        for (auto i = start; i != end; ++i) {
            if (parse(*i, checked ? false : true)) {
                checked = true;
            }
        }
        for (auto i = token_start; i != token_end; ++i) {
            auto iter = find_token(*i, false);
            if (iter == token_mp.end()) {
                throw not_valid_grammer();
            }
            start_token.push_back(iter->second);
        }
    }

    std::unordered_map<std::string, lex::TokenIndex>::iterator lex::find_token(const std::string &token, bool insertable) {
        auto iter = token_mp.find(token);
        if (iter == token_mp.end() && insertable) {
            if (rule_list.empty()) {
                rule_list.push_back({});
            }
            iter = token_mp.insert({token, rule_list.size()}).first;
            rule_list.push_back({});
        }
        return iter;
    }

    bool lex::parse(const std::string &str, bool check_left) {
        std::string::size_type arrow_pos = str.find("->");
        if (arrow_pos == std::string::npos || !arrow_pos) {
            throw not_valid_grammer();
        }
        std::string token_left = str.substr(0, arrow_pos);
        std::string token_right;
        char right_chr;
        std::string::size_type right_pos = arrow_pos + 2;
        if (right_pos == str.size()) {
            if (token_left.empty()) {
                throw not_valid_grammer();
            }
            auto &&iter = find_token(token_left);
            rule_list[iter->second][-1].push_back(0);
            return false;
        }
        bool ret = false;
        if (str[right_pos] == '{') {
            if (check_left) {
                left_linear = true;
                ret = true;
            }
            std::string::size_type right_cls = str.find('}', right_pos + 1);
            if (right_cls == std::string::npos) {
                throw not_valid_grammer();
            }
            token_right = str.substr(right_pos + 1, right_cls - right_pos - 1);
            right_pos = right_cls + 1;
            if (right_pos == str.size() - 1) {
                if (str[right_pos] == '\\') {
                    throw not_valid_grammer();
                }
                right_chr = str[right_pos];
            }
            else if (right_pos == str.size() - 2) {
                if (str[right_pos] != '\\') {
                    throw not_valid_grammer();
                }
                right_chr = str[right_pos + 1];
            }
            else if (right_pos != str.size()) {
                throw not_valid_grammer();
            }
        }
        else {
            if (str[right_pos] == '\\') {
                if (right_pos == str.size() - 1) {
                    throw not_valid_grammer();
                }
                right_chr = str[right_pos + 1];
                right_pos += 2;
            }
            else {
                right_chr = str[right_pos];
                ++right_pos;
            }
            if (right_pos != str.size()) {
                // left_linear wanted
                if (str[right_pos] != '{' ||
                    (!check_left && left_linear)) {
                    throw not_valid_grammer();
                }
                if (check_left) {
                    left_linear = false;
                }
                ret = true;
                std::string::size_type right_cls = str.find('}', right_pos + 1);
                if (right_cls != str.size() - 1) {
                    throw not_valid_grammer();
                }
                token_right = str.substr(right_pos + 1, right_cls - right_pos - 1);
            }
        }
        // token_right may be 0-sized string
        if (token_left.size() == 0) {
            throw not_valid_grammer();
        }
        auto &&left_found = find_token(token_left);
        std::unordered_map<std::string, TokenIndex>::iterator right_found;
        if (token_right.size()) {
            right_found = find_token(token_right);
        }
        rule_list[left_found->second][right_chr].push_back(token_right.size() ? right_found->second : 0);
        return ret;
    }

    dfa<char> lex::get_dfa() const {
        return left_linear ? get_dfa_left() : get_dfa_right();
    }

    inline static std::vector<nfa<char>::Node> create_nodes(nfa<char> &ref, unsigned N) {
        std::vector<nfa<char>::Node> nodes;
        nodes.reserve(N);
        for (unsigned i = 0; i < N; ++i) {
            nodes.push_back(ref.get_new_node());
        }
        return std::move(nodes);
    }

    dfa<char> lex::get_dfa_left() const {
        nfa<char> result;
        auto nodes = create_nodes(result, rule_list.size());
        for (unsigned i = 1; i < rule_list.size(); ++i) {
            for (auto &val : rule_list[i]) {
                if (val.first == -1) {
                    result.add_transform(nodes[0], nodes[i]);
                }
                else {
                    for (auto &con : val.second) {
                        result.add_transform(nodes[con], static_cast<char>(val.first), nodes[i]);
                    }
                }
            }
        }
        result.set_start(nodes[0]);
        for (auto &i : start_token) {
            result.set_end(nodes[i]);
        }
        return dfa<char>(result);
    }

    dfa<char> lex::get_dfa_right() const {
        nfa<char> result;
        auto nodes = create_nodes(result, rule_list.size());
        for (unsigned i = 1; i < rule_list.size(); ++i) {
            for (auto &val : rule_list[i]) {
                if (val.first == -1) {
                    result.add_transform(nodes[i], nodes[0]);
                }
                else {
                    for (auto &con : val.second)
                    result.add_transform(nodes[i], static_cast<char>(val.first), nodes[con]);
                }
            }
        }
        result.set_end(nodes[0]);
        for (auto &i : start_token) {
            result.set_start(nodes[i]);
        }
        return dfa<char>(result);
    }
}

#ifdef DEBUG

int main(int argc, char **argv) {
    parser::lex rule(argv + 1, argv + argc - 1, argv + argc - 1, argv + argc);
    parser::dfa<char> graph = rule.get_dfa();
    return 0;
}

#endif