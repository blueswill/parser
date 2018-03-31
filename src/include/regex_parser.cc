#include"regex_parser.hpp"
#include"cfg.hpp"
#include<vector>
#include<stack>

namespace parser {

    struct Props {
        enum TYPE { TOKEN, NON_TOKEN };
        TYPE type;
        lexer_token token;
        lexer_graph<char> graph;

        Props(lexer_token token) :
            type(TOKEN), token(token) {}
        Props(lexer_graph<char> &&graph) :
            type(NON_TOKEN), graph(std::move(graph)) {}
        bool is_token() const { return type == TOKEN; }
    };

#ifdef DEBUG
    void Assert(int index, const std::vector<Props> &lst) {
        bool notdo = true;
        switch (index) {
        case 0: notdo = (lst.size() == 1 && !lst[0].is_token()); break;
        case 1: notdo = (lst.size() == 3 && !lst[0].is_token() && lst[1].is_token() &&
                        !lst[2].is_token() && lst[1].token.type == lexer_token::SPLIT); break;
        case 4:
        case 8:
        case 14:
        case 2: notdo = (lst.size() == 1 && !lst[0].is_token()); break;
        case 3: notdo = (lst.size() == 2 && !lst[0].is_token() && !lst[1].is_token()); break;
        case 5: notdo = (lst.size() == 2 && !lst[0].is_token() && lst[1].is_token() && lst[1].token.type == lexer_token::ZERO_MORE); break;
        case 6: notdo = (lst.size() == 2 && !lst[0].is_token() && lst[1].is_token() && lst[1].token.type == lexer_token::ONE_MORE); break;
        case 7: notdo = (lst.size() == 2 && !lst[0].is_token() && lst[1].is_token() && lst[1].token.type == lexer_token::ZERO_ONE); break;
        case 9: notdo = (lst.size() == 3 && !lst[0].is_token() && lst[0].token.type == lexer_token::LEFT_P &&
                        !lst[1].is_token() && lst[2].is_token() && lst[2].token.type == lexer_token::RIGHT_P); break;
        case 11: notdo = (lst.size() == 3 && !lst[0].is_token() && lst[0].token.type == lexer_token::LEFT_B &&
                        !lst[1].is_token() && lst[2].is_token() && lst[2].token.type == lexer_token::RIGHT_B); break;
        case 17:
        case 10: notdo = (lst.size() == 1 && lst[0].is_token() && lst[0].token.type == lexer_token::CHAR); break;
        case 12: notdo = (lst.size() == 1 && lst[0].is_token() && lst[0].token.type == lexer_token::ANY); break;
        case 13: notdo = (lst.size() == 2 && lst[0].is_token() && lst[0].token.type == lexer_token::NOT &&
                         !lst[1].is_token()); break;
        case 15: notdo = (lst.size() == 2 && lst[0].is_token() && lst[0].token.type == lexer_token::CHAR &&
                         !lst[1].is_token()); break;
        case 16: notdo = (lst.size() == 4 && lst[0].is_token() && lst[1].is_token() && !lst[2].is_token() && !lst[3].is_token() &&
                         lst[0].token.type == lexer_token::CHAR && lst[1].token.type == lexer_token::REGION && lst[2].token.type == lexer_token::CHAR); break;
        default:
                 throw std::runtime_error("not a valid producer");
        }
        if (!notdo) {
            throw std::runtime_error("producer " + std::to_string(index) + " error");
        }
    }
#endif
    lexer_graph<char> reduction(size_t number, std::vector<Props> &&lst) {
#ifdef DEBUG
        Assert(number, lst);
#endif
        switch (number) {
            // E -> E | T
            case 1:
                {
                    lexer_graph<char> ret(std::move(lst.front().graph));
                    auto ast = lst.back().graph.get_start(), aed = lst.back().graph.get_end();
                    auto st = ret.get_start(), ed = ret.get_end();
                    st->add_connection(ast);
                    aed->add_connection(ed);
                    ret.combine(std::move(lst.back().graph));
                    return ret;
                }
                //the end of reduction
            case 0:
            case 4:
            case 8:
            case 9:
            case 14:
            case 11:
                // E -> T
            case 2: return std::move(lst.front().graph);
                // T -> M T
            case 3:
                {
                    lexer_graph<char> ret(std::move(lst.front().graph));
                    auto ed = ret.get_end();
                    ed->add_connection(lst.back().graph.get_start());
                    ret.combine(std::move(lst.back().graph));
                    return ret;
                }
                // M -> F *
            case 5:
                {
                    lexer_graph<char> ret(std::move(lst.front().graph));
                    auto st = ret.get_start(), ed = ret.get_end();
                    st->add_connection(ed);
                    ed->add_connection(st);
                    return ret;
                }
                // M -> F +
            case 6:
                {
                    lexer_graph<char> ret(std::move(lst.front().graph));
                    ret.get_end()->add_connection(ret.get_start());
                    return ret;
                }
                // M -> F ?
            case 7:
                {
                    lexer_graph<char> ret(std::move(lst.front().graph));
                    ret.get_start()->add_connection(ret.get_end());
                    return ret;
                }
                // F -> c
            case 10:
                {
                    lexer_graph<char> ret;
                    auto st = ret.get_start();
                    auto ed = ret.get_end();
                    st->add_connection(ed, regex_token(lst[0].token.ch));
                    return ret;
                }
                // F -> .
            case 12:
                {
                    lexer_graph<char> ret;
                    auto st = ret.get_start();
                    auto ed = ret.get_end();
                    st->add_connection(ed, regex_token());
                    return ret;
                }
                // K -> ^ K'
            case 13:
                {
                    auto ret = std::move(lst[1].graph);
                    ret.get_start()->reverse();
                    return ret;
                }
                // K' -> c K'
            case 15:
                {
                    auto ret = std::move(lst[1].graph);
                    ret.get_start()->add_connection(ret.get_end(), regex_token(lst[0].token.ch));
                    return ret;
                }
                // K' -> c - c K'
            case 16:
                {
                    auto ret = std::move(lst[3].graph);
                    ret.get_start()->add_connection(ret.get_end(), regex_token(lst[0].token.ch, lst[2].token.ch));
                    return ret;
                }
            case 17:
                {
                    lexer_graph<char> ret;
                    ret.get_start()->add_connection(ret.get_end(), regex_token(lst[0].token.ch));
                    return ret;
                }
            default:
                throw std::runtime_error("unknown producer " + std::to_string(number));
                break;
        }
    }

    lexer_graph<char> regex_parse(const lexer &generator) {
        std::stack<Props> props_stack;
        std::stack<int> state_stack;
    }
}
