#include<getopt.h>
#include<iostream>
#include<memory>
#include<fstream>
#include<cerrno>
#include<cstring>
#include<sstream>
#include<iomanip>

#include"regex_parser.hpp"
#include"regex_matcher.hpp"
#include"cfg.hpp"
#include"cfg_graph.hpp"
#include"cfg_reduction.hpp"

#define TABLE_WIDTH 20

std::set<std::string> term_tokens;

std::string line;
size_t line_pos = 0;
std::vector<size_t> state_stack {0};
std::vector<parser::Token> term_stack;

struct option longopts[] = {
    {"cfg", 1, nullptr, 'c'},
    {"pattern", 1, nullptr, 'p'},
    {"token", 1, nullptr, 'k'},
    {"reduction", 1, nullptr, 'r'},
    {"table", 1, nullptr, 't'},
    {"help", 0, nullptr, 'h'},
    {"file", 1, nullptr, 'f'},
    {nullptr, 0, nullptr, 0}
};

template<typename Type>
class Deleter {
private:
    bool done;
public:
    Deleter(bool d = true) : done(d) {}
    void operator()(Type *ptr) {
        if (done) delete ptr;
    }
};

class reduction_error : public std::exception {
private:
    std::string _str;
public:
    reduction_error(const std::string &s) : _str(s) {}
    const char *what() const noexcept override {
        return _str.data();
    }
};

void usage(const char *cmd_name) {
    fprintf(stdout, "A simple parser for specified pattern definition and LR(1) grammar\n");
    fprintf(stdout, "usage: %s [options] [-c|--cfg cfg_deffile] [-p|--pattern pattern_deffile]\n", cmd_name);
    fprintf(stdout, "options:\n");
    fprintf(stdout, "-f, --file=parsefile         set file to parse, necessary if -r or -k is given\n");
    fprintf(stdout, "-t, --table=outfile          set produced reduction table output file\n");
    fprintf(stdout, "-k, --token=outfile          set token output file\n");
    fprintf(stdout, "-r, --reduction=outfile      set reduction result output file\n");
    fprintf(stdout, "-h, --help                   print this help message\n");
    exit(EXIT_SUCCESS);
}

void file_open_err(const char *file) {
    fprintf(stderr, "%s: %s\n", file, strerror(errno));
    exit(EXIT_FAILURE);
}

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

void print_status(std::ostream *os) {
    for (auto item : term_stack) os && (*os) << item << ' ';
    os && (*os) << std::endl;
}

bool reduction(
        const parser::LR_reduction &reduction_table,
        parser::LR_reduction::reduction_type reduction,
        const parser::Token &tk,
        std::ostream *os) {
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
            case TYPE::ACCEPT: os && (*os) << "ACCEPT\n"; return true;
            case TYPE::REDUCTION:
            case TYPE::ERR: throw reduction_error("GOTO table error");
            case TYPE::TRANSFORM:
                            state_stack.push_back(next.get_state());
                            term_stack.push_back(head);
                            break;
        }
        print_status(os);
        reduction = reduction_table.next_state(state_stack.back(), tk);
    }
    switch (reduction.get_type()) {
        case TYPE::ACCEPT: os && (*os) << "ACCEPT\n"; return true;
        case TYPE::ERR: throw reduction_error("ERR type");
        case TYPE::TRANSFORM:
                        state_stack.push_back(reduction.get_state());
                        term_stack.push_back(tk);
                        break;
        default: break;
    }
    print_status(os);
    return false;
}

void print_table(parser::cfg &c, parser::LR_reduction &table, std::ostream &os) {
#define OS (os << std::setw(TABLE_WIDTH))
    std::map<parser::cfg::RULE, size_t> rule_list;
    auto &&rs = c.get_rules();
    std::for_each(rs.begin(), rs.end(),
            [&rule_list](parser::cfg::RULE r) { rule_list.insert({r, rule_list.size()});});
    typedef parser::LR_reduction::reduction_type TYPE;
    OS << "";
    auto &&terms = c.get_terminal_tokens();
    auto &&nonterms = c.get_nonterminal_tokens();
    std::vector<parser::Token> tks;
    std::copy(terms.begin(), terms.end(), std::back_inserter(tks));
    tks.push_back(parser::Token::get_end());
    std::copy(nonterms.begin(), nonterms.end(), std::back_inserter(tks));
    for (auto &item : tks) {
        OS << item;
    }
    os << std::endl;
    for (auto i = 0u; i < table.state_count(); ++i) {
        OS << i;
        for (auto &term : tks) {
            auto next = table.next_state(i, term);
            switch (next.get_type()) {
                case TYPE::ACCEPT: OS << "accept"; break;
                case TYPE::ERR: OS << ""; break;
                case TYPE::REDUCTION: 
                            OS << "r" + std::to_string(rule_list.find(next.get_rule())->second);
                            break;
                case TYPE::TRANSFORM: OS << "s" + std::to_string(next.get_state()); break;
            }
        }
        os << std::endl;
    }
#ifdef OS
#undef OS
#endif
}

void set_in(std::unique_ptr<std::istream, Deleter<std::istream>> &f) {
    if (std::string("-") == optarg) {
        f = std::unique_ptr<std::istream, Deleter<std::istream>>(&std::cin, false);
    }
    else f = std::unique_ptr<std::istream, Deleter<std::istream>>(new std::ifstream(optarg));
    if (!*f) {
        std::cerr << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void set_out(std::unique_ptr<std::ostream, Deleter<std::ostream>> &f) {
    if (std::string("-") == optarg) {
        f = std::unique_ptr<std::ostream, Deleter<std::ostream>>(&std::cout, false);
    }
    else f = std::unique_ptr<std::ostream, Deleter<std::ostream>>(new std::ofstream(optarg));
    if (!*f) {
        std::cerr << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    int optc;
    std::unique_ptr<std::istream, Deleter<std::istream>> cfg_definition, pattern_definition, parse_file;
    std::unique_ptr<std::ostream, Deleter<std::ostream>> table_out, token_out, reduction_out;
    while ((optc = getopt_long(argc, argv, "c:p:k:r:t:hf:", longopts, nullptr)) != -1) {
        switch (optc) {
            case 'c': set_in(cfg_definition); break;
            case 'p': set_in(pattern_definition); break;
            case 'f': set_in(parse_file); break;
            case 'k': set_out(token_out); break;
            case 'r': set_out(reduction_out); break;
            case 't': set_out(table_out); break;
            case 'h': usage(argv[0]);
            default:
                fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argc != optind) {
        fprintf(stderr, "invalid argument found\n");
        exit(EXIT_FAILURE);
    }

    if (!cfg_definition || !pattern_definition) {
        fprintf(stderr, "cfg or pattern is not defined\n");
        fprintf(stderr, "Try '%s -h' for help\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    parser::matcher_builder<std::string> builder;
    get_matcher_info(*pattern_definition, builder);
    auto cfg = get_cfg(*cfg_definition, builder);
    auto matcher = builder.get_matcher();
    auto graph = cfg.get_LR_graph();
    auto reduction_table = graph.get_reduction_table();

    if (table_out)
        print_table(cfg, reduction_table, *table_out);
    if ((token_out || reduction_out) && !parse_file) {
        std::cerr << "file to parse is expected\n";
        exit(EXIT_FAILURE);
    }
    if (!parse_file) return 0;
    try {
        while (std::getline(*parse_file, line)) {
            reduction_out && (*reduction_out) << "parse line " << line_pos << ": " << line << std::endl;
            std::string::iterator prev = line.begin();
            while (prev != line.end()) {
                while(std::isspace(*prev)) ++prev;
                auto ret = matcher.match(prev, line.end());
                auto cur_state = state_stack.back();
                auto iter = ret.first.begin();
                for (; iter != ret.first.end(); ++iter) {
                    parser::Token tk(*iter);
                    auto rn = reduction_table.next_state(cur_state, tk);
                    if (rn.get_type() != parser::LR_reduction::reduction_type::ERR) {
                        reduction(reduction_table, rn, tk, reduction_out.get());
                        token_out && (*token_out) << tk << ": " << std::string(prev, ret.second) << std::endl;
                        break;
                    }
                }
                if (iter == ret.first.end()) {
                    throw reduction_error("unexpected token");
                }
                prev = ret.second;
            }
            ++line_pos;
        }
        auto tk = parser::Token::get_end();
        do {
            auto ret = reduction_table.next_state(state_stack.back(), tk);
            if (ret.get_type() == parser::LR_reduction::reduction_type::ERR) {
                throw reduction_error("unexpected end of file");
            }
            if (reduction(reduction_table, ret, tk, reduction_out.get())) break;
        } while (true);
    } catch(parser::no_match_error<std::string::iterator> &e) {
        std::cerr << "lexer error at line " << line_pos << ": " << std::string(e.get_iter(), line.end()) << std::endl;
    } catch(reduction_error &e) {
        std::cerr << "reduction error at line " << line_pos << ": " << e.what() << std::endl;
    }
    return 0;
}
