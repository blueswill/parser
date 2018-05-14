#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<vector>

#include"regex_parser.hpp"
#include"regex_matcher.hpp"
#include"cfg.hpp"
#include"cfg_graph.hpp"
#include"cfg_reduction.hpp"

std::set<std::string> term_tokens;

void get_matcher_info(std::istream &is, parser::matcher_builder<std::string> &matcher_builder) {
    std::string line;
    while (std::getline(is, line)) {
        if (line.empty()) continue;
        std::istringstream is(line);
        size_t p;
        std::string type, pattern;
        is >> p >> type;
        std::getline(is, pattern);
        if (!is) pattern = type;
        pattern = pattern.substr(1, pattern.size() - 1);
        parser::lexer lexer(pattern);
        matcher_builder.register_token(type, parser::regex_parse(lexer), p);
        term_tokens.insert(type);
    }
}

auto get_cfg(std::istream &is, parser::matcher_builder<std::string> &matcher_builder) {
    parser::cfg_builder builder;
    std::string line;
    std::getline(is, line);
    std::istringstream in(line);
    std::set<parser::Token> non_terms;
    int first = true;
    auto unary = 
        [&first, &builder](const std::string &s) {
            parser::Token tk(s, false);
            if (first) {
                builder.set_starter(tk);
                first = false;
            }
            return tk;
        };
    std::transform(
            std::istream_iterator<std::string>(in), std::istream_iterator<std::string>(),
            std::inserter(non_terms, non_terms.begin()),
            unary);
    builder.add_nonterminal_tokens(non_terms.begin(), non_terms.end());
    while (std::getline(is, line)) {
        if (line.empty()) continue;
        in = std::istringstream(line);
        std::vector<parser::Token> tks;
        auto unary = [&non_terms](const std::string &s) {
            parser::Token tmp(s, false);
            if (non_terms.find(tmp) != non_terms.end()) return tmp;
            else return parser::Token(s, true);
        };
        std::transform(
                std::istream_iterator<std::string>(in), std::istream_iterator<std::string>(),
                std::back_inserter(tks),
                unary);
        if (tks.empty() || tks.front().is_terminal()) throw std::runtime_error("invalid cfg");
        parser::Token head = tks.front();
        // insert to terminal list
        for (auto iter = tks.begin() + 1; iter != tks.end(); ++iter) {
            if (iter->is_terminal()) {
                if (term_tokens.find(iter->get_name()) == term_tokens.end()) {
                    term_tokens.insert(iter->get_name());
                    matcher_builder.register_token(iter->get_name(), iter->get_name());
                }
            }
        }
        if (tks.size() != 1) {
            auto ret = builder.add_rule(head,
                    std::vector<parser::Token>(std::move_iterator(tks.begin() + 1), std::move_iterator(tks.end())));
            if (!ret) throw std::runtime_error("invalid cfg at: " + line);
        }
        else builder.add_empty_rule(head);
    }
    std::vector<parser::Token> tks;
    std::for_each(term_tokens.begin(), term_tokens.end(),
            [&tks](const std::string &t) { tks.push_back(parser::Token(t));});
    builder.add_terminal_tokens(tks.begin(), tks.end());
    return builder.get_cfg();
}

std::string line;
size_t line_pos = 0;
std::vector<size_t> state_stack {0};
std::vector<parser::Token> term_stack;

class reduction_error : public std::exception {
private:
    std::string _str;
public:
    reduction_error(const std::string &s) : _str(s) {}
    const char *what() const noexcept override {
        return _str.data();
    }
};

void print_status() {
    for (auto item : term_stack) std::cout << item << ' ';
    std::cout << std::endl;
}

bool reduction(const parser::LR_reduction &reduction_table, parser::LR_reduction::reduction_type reduction, const parser::Token &tk) {
    if (state_stack.empty()) throw reduction_error("state_stack empty");
    typedef decltype(reduction.get_type()) TYPE;
    while (TYPE::REDUCTION == reduction.get_type()) {
        int num = reduction.get_rule()->second.size();
        //null producer
        if (reduction.get_rule()->second.front().is_null()) {
            num = 0;
        }
        while (--num >= 0) {
            state_stack.pop_back();
            term_stack.pop_back();
        }
        auto head = reduction.get_rule()->first;
        auto next = reduction_table.next_state(state_stack.back(), head);
        switch (next.get_type()) {
            case TYPE::ACCEPT: std::cout << "ACCEPT\n"; return true;
            case TYPE::REDUCTION:
            case TYPE::ERR: throw reduction_error("GOTO table error");
            case TYPE::TRANSFORM:
                            state_stack.push_back(next.get_state());
                            term_stack.push_back(head);
                            break;
        }
        print_status();
        reduction = reduction_table.next_state(state_stack.back(), tk);
    }
    switch (reduction.get_type()) {
        case TYPE::ACCEPT: std::cout << "ACCEPT\n"; return true;
        case TYPE::ERR: throw reduction_error("ERR type");
        case TYPE::TRANSFORM:
                        state_stack.push_back(reduction.get_state());
                        term_stack.push_back(tk);
                        break;
        default: break;
    }
    print_status();
    return false;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << argv[0] << " <lexial definition file> <cfg definition file> <file to parse>\n";
        return 0;
    }
    std::ifstream pattern_def(argv[1]), cfg_def(argv[2]), parse_def(argv[3]);
    parser::matcher_builder<std::string> builder;
    get_matcher_info(pattern_def, builder);
    auto cfg = get_cfg(cfg_def, builder);
    auto matcher = builder.get_matcher();
    auto graph = cfg.get_LR_graph();
    auto reduction_table = graph.get_reduction_table();

    try {
        while (std::getline(parse_def, line)) {
            std::cout << "parse line " << line_pos << ": " << line << std::endl;
            std::string::iterator prev = line.begin();
            while (prev != line.end()) {
                while(std::isspace(*prev)) ++prev;
                auto ret = matcher.match(prev, line.end());
                prev = ret.second;
                auto cur_state = state_stack.back();
                auto iter = ret.first.begin();
                for (; iter != ret.first.end(); ++iter) {
                    parser::Token tk(*iter);
                    auto ret = reduction_table.next_state(cur_state, tk);
                    if (ret.get_type() != parser::LR_reduction::reduction_type::ERR) {
                        reduction(reduction_table, ret, tk);
                        break;
                    }
                }
                if (iter == ret.first.end()) {
                    throw reduction_error("unexpected token");
                }
            }
            ++line_pos;
        }
        auto tk = parser::Token::get_end();
        do {
            auto ret = reduction_table.next_state(state_stack.back(), tk);
            if (ret.get_type() == parser::LR_reduction::reduction_type::ERR) {
                throw reduction_error("unexpected end of file");
            }
            if (reduction(reduction_table, ret, tk)) break;
        } while (true);
    } catch(parser::no_match_error<std::string::iterator> &e) {
        std::cout << "lexer error at line " << line_pos << ": " << std::string(e.get_iter(), line.end()) << std::endl;
    } catch(reduction_error &e) {
        std::cout << "reduction error at line " << line_pos << ": " << e.what() << std::endl;
    }
    return 0;
}
