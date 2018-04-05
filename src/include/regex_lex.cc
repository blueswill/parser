#include"regex_lex.hpp"
#include<sstream>

namespace parser {
    void lexer::read_next() {
        switch (inner[cur]) {
        case '\\': {
            if (++cur == inner.size()) {
                throw pattern_error("expected character after \\");
            }
            cur_token = lexer_token(inner[cur]);
            break;
        }
        case '\0': cur_token = lexer_token(lexer_token::END_TEXT); return;
        default:
            if (state == IN_B || state == FIRST_B) {
                switch (inner[cur]) {
                case ']':
                    state = OUT_B;
                    cur_token = lexer_token(lexer_token::RIGHT_B);
                    break;
                case '^':
                    if (state == FIRST_B) {
                        state = IN_B;
                        cur_token = lexer_token(lexer_token::NOT);
                    }
                    else {
                        cur_token = lexer_token('^');
                    }
                    break;
                case '-':
                    state = IN_B;
                    cur_token = lexer_token(lexer_token::REGION);
                    break;
                default:
                    state = IN_B;
                    cur_token = lexer_token(inner[cur]);
                    break;
                }
            }
            else {
                switch (inner[cur]) {
                case '[':
                    state = FIRST_B;
                    cur_token = lexer_token(lexer_token::LEFT_B);
                    break;
                case '*':
                    cur_token = lexer_token(lexer_token::ZERO_MORE);
                    break;
                case '+':
                    cur_token = lexer_token(lexer_token::ONE_MORE);
                    break;
                case '?':
                    cur_token = lexer_token(lexer_token::ZERO_ONE);
                    break;
                case '(':
                    cur_token = lexer_token(lexer_token::LEFT_P);
                    break;
                case ')':
                    cur_token = lexer_token(lexer_token::RIGHT_P);
                    break;
                case '|':
                    cur_token = lexer_token(lexer_token::SPLIT);
                    break;
                case '.':
                    cur_token = lexer_token(lexer_token::ANY);
                    break;
                default:
                    cur_token = lexer_token(inner[cur]);
                    break;
                }
            }
        }
        ++cur;
        return;
    }

#ifdef DEBUG
    lexer_token::operator std::string() const {
        std::ostringstream os;
        switch (type) {
        case SPLIT : os << "SPLIT"; break;
        case ANY: os << "ANY"; break;
        case ZERO_MORE: os << "ZERO_MORE"; break;
        case ONE_MORE: os << "ONE_MORE"; break;
        case ZERO_ONE: os << "ZERO_ONE"; break;
        case LEFT_P: os << "LEFT_P"; break;
        case RIGHT_P: os << "RIGHT_P"; break;
        case LEFT_B: os << "LEFT_B"; break;
        case RIGHT_B: os << "RIGHT_B"; break;
        case NOT: os << "NOT"; break;
        case CHAR: os << "CHAR"; break;
        case REGION: os << "REGION"; break;
        case END_TEXT: os << "END_TEXT"; break;
        }
        if (type == CHAR) {
            os << ": " << ch;
        }
        return os.str();
    }
#endif
}
/*
#ifdef DEBUG
#include<iostream>
int main(int argc, char **argv) {
    if (argc != 2) return 0;
    std::string str(argv[1]);
    parser::lexer lexer(str.begin(), str.end());
    parser::lexer_token token;
    do {
        token = lexer.consume();
        std::cout << std::string(token) << std::endl;
    } while (token.type != parser::lexer_token::END_TEXT);
    return 0;
}
#endif

*/
