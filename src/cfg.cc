#include<iostream>
#include<random>
#include<ctime>

#include"cfg.hpp"
#include"cfg_graph.hpp"

static std::string generate_random_string(size_t length) {
    constexpr char str[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
    const size_t len = sizeof(str);
    std::default_random_engine e(std::time(nullptr));
    std::uniform_int_distribution<size_t> d(0, len - 1);
    std::string ret;
    while (--length > 0) {
        ret.push_back(str[d(e)]);
    }
    ret.push_back(str[d(e)]);
    return ret;
}

namespace parser {
    static std::string get_tmp_string(int num) {
        std::string str;
        while (--num >= 0) {
            str.push_back('\n');
        }
        return str;
    }
    const std::string Token::null_val = get_tmp_string(1);
    const std::string Token::end_val = get_tmp_string(2);

    std::ostream &operator<<(std::ostream &os, const Token &tk) {
        if (tk.is_end()) {
            os << "<<$>>";
        }
        else if (tk.is_null()) {
            os << "<<null>>";
        }
        else {
            os << tk.name;
        }
        return os;
    }

}

namespace parser {
    LR_graph cfg::get_LR_graph() const {
        return LR_graph(*this);
    }

    inline bool cfg_builder::add_rule(const Token &head,
                                      const std::vector<Token> &body) {
        if (!inner.contain(head)) return false;
        for (auto token : body) {
            if (!token.is_terminal() && !inner.contain(token))
            return false;
        }
        inner.rule_list.insert({head, body});
        return true;
    }

    bool cfg_builder::add_empty_rule(const Token &head) {
        if (!inner.contain(head)) return false;
        inner.rule_list.insert({head, {Token::get_null()}});
        return true;
    }

    cfg cfg_builder::get_cfg() {
        if (inner.contain(starter)) {
            for (auto item : inner.nonterminal_list) {
                if (inner.rule_list.find(item) == inner.rule_list.end()) {
                    throw std::runtime_error("ERROR: no producer for nonterminal token "
                                             + item.get_name());
                }
            }
            inner.starter = starter;
            //add extend rule
            while (inner.contain(inner.extended = Token(generate_random_string(5), false)));
            inner.nonterminal_list.insert(inner.extended);
            add_rule(inner.extended, {inner.starter});
            inner.compute_first_set();
            return inner;
        }
        throw std::runtime_error("ERROR: starter is not in cfg");
    }

    Token cfg_builder::get_token(const std::string &token,
                                 bool throw_when_terminal) {
        Token ifnonterminal(token, false);
        if (inner.nonterminal_list.find(ifnonterminal)
            != inner.terminal_list.end()) {
            return ifnonterminal;
        }
        if (throw_when_terminal) {
            throw std::runtime_error("ERROR: " + token + " is terminal");
        }
        auto iter = inner.terminal_list.insert(Token(token));
        return *iter.first;
    }

    std::unordered_set<Token> cfg::get_first_set(const Token &token) const {
        auto iter = first_set.find(token);
        return iter->second;
    }

    //thanks for https://github.com/kartikkukreja/blog-codes/blob/master/src/FIRST%20set%20of%20each%20variable%20in%20a%20CFG.cpp
    void cfg::compute_first_set() {
        for (auto rule : rule_list) {
            if (rule.second.front().is_terminal()) {
                first_set[rule.first].insert(rule.second.front());
            }
            else {
                first_set.insert({rule.first, {}});
            }
        }

        bool complete = false;
        while (!complete) {
            complete = true;
            for (auto rule : rule_list) {
                auto iter = first_set.find(rule.first);
                auto prev_size = iter->second.size();
                bool contain_null = (iter->second.find(Token::get_null()) != iter->second.end());
                auto b_iter = rule.second.begin();
                for (; b_iter != rule.second.end(); ++b_iter) {
                    Token &token = *b_iter;
                    if (token.is_terminal()) {
                        iter->second.insert(token);
                        break;
                    }
                    auto tmp = first_set.find(token);
                    if (tmp->second.size())
                    iter->second.insert(tmp->second.begin(), tmp->second.end());
                    if (!contain_null && tmp->second.find(Token::get_null()) != tmp->second.end()) {
                        iter->second.erase(Token::get_null());
                        continue;
                    }
                    break;
                }
                if (b_iter == rule.second.end()) iter->second.insert(Token::get_null());
                if (prev_size != iter->second.size()) {
                    complete = false;
                }
            }
        }
    }

    bool cfg::contain(const Token &token) const {
        if (token.is_terminal()) {
            return terminal_list.find(token) != terminal_list.end();
        }
        else {
            return nonterminal_list.find(token) != nonterminal_list.end();
        }
    }

    std::ostream &operator<<(std::ostream &os, const cfg &c) {
        for (auto rule : c.rule_list) {
            os << rule.first << " ->";
            for (auto tk : rule.second) {
                os << ' ' << tk;
            }
            os << std::endl;
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const cfg::RULE &rule) {
        os << rule->first << " ->";
        for (auto item : rule->second) {
            os << ' ' << item;
        }
        return os;
    }

    bool operator<(const cfg::RULE &r1, const cfg::RULE &r2) {
        if (!(r1->first == r2->first)) return r1->first < r2->first;
        for (auto i = 0u; i < r1->second.size(); ++i) {
            if (i == r2->second.size()) return true;
            if (!(r1->second[i] == r2->second[i])) return r1->second[i] < r2->second[i];
        }
        return false;
    }
}
