#ifndef REGEX_LEX_HPP
#define REGEX_LEX_HPP

#include<string>
#include<stdexcept>

/* regular expression lexer
 */

namespace parser {
    class pattern_error : public std::runtime_error {
    public:
        pattern_error(const char *str) : std::runtime_error(str) {}
    };
    struct lexer_token {
        enum LEXER_TYPE {
            SPLIT,      // |
            ANY,        // .
            ZERO_MORE,  // *
            ONE_MORE,   // +
            ZERO_ONE,   // ?
            LEFT_P,     // (
            RIGHT_P,    // )
            LEFT_B,     // [
            RIGHT_B,    // ]
            NOT,        // ^
            CHAR,       // any character
            REGION,      // in []: -
            END_TEXT
        };

        LEXER_TYPE type;
        char ch;
        lexer_token(char ch) : type(CHAR), ch(ch) {}
        lexer_token(LEXER_TYPE type) : type(type) {}
        lexer_token() = default;
#ifdef DEBUG
        explicit operator std::string() const;
#endif
    };
    class lexer {
    private:
        // to check whether in [] unit
        // in [] unit only -  and ^ has special meaning
        enum STATE { OUT_B, FIRST_B, IN_B };
        STATE state = OUT_B;

        std::string inner;
        std::string::size_type cur;
        lexer_token cur_token;

        void read_next();
    public:
        lexer(std::string input) :
            inner(std::move(input)), cur(0) {
            read_next();
        }
        template<typename Iter>
        lexer(Iter begin, Iter end) :
            inner(begin, end), cur(0) {
            read_next();
        }

        lexer_token get() const { return cur_token; }
        lexer_token consume() {
            auto cur = cur_token;
            read_next();
            return cur;
        }
    };
}

#endif
