#ifndef CFG_HPP
#define CFG_HPP

#include<vector>
#include<string>
#include<unordered_set>
#include<unordered_map>
#include<memory>
#include<iosfwd>
#include<functional>

#include"forward.hpp"

namespace parser {

    class Token {
    public:
        friend bool operator==(const Token &t1, const Token &t2) {
            return t1.name == t2.name && t1.type == t2.type;
        }
        //two special terminal token \epsilon and $
        static bool is_end(const Token &tk);
        static bool is_null(const Token &tk);

        bool is_end() const { return is_end(*this); }
        bool is_null() const { return is_null(*this); }
        bool is_terminal() const { return type == TERM; }
        static Token get_end();
        static Token get_null();
        Token() : type(TERM), name(null_val) {}
        Token(const std::string &name, bool isterminal = true) :
            type(isterminal ? TERM : NON_TERM), name(name) {}

        friend class CFG;
        friend struct std::hash<Token>;

        friend std::ostream &operator<<(std::ostream &os, const Token &tk);
        friend bool operator<(const Token &t1, const Token &t2) {
            return t1.name < t2.name || (t1.name == t2.name && t1.type < t2.type);
        }
    private:
        enum TYPE { TERM,NON_TERM };
        TYPE type;
        std::string name;

        static std::string end_val;
        static std::string null_val;
    private:
        friend class cfg;
    };
}

namespace std {
    template<>
    struct hash<parser::Token> {
        size_t operator()(const parser::Token &token) const {
            auto i = hash<std::string>();
            size_t tmp = i(token.name);
            return token.is_terminal() ? ~tmp : tmp;
        }
    };
}

namespace parser {
    class cfg {
    private:
        std::unordered_set<Token> nonterminal_list, terminal_list;
        std::unordered_multimap<Token, std::vector<Token>> rule_list;

        std::unordered_map<Token, std::unordered_set<Token>> first_set;
        Token starter, extended;

        void compute_first_set();

        cfg() {}

        friend class cfg_builder;
        friend class LR_graph;
        friend class cfg_reduction;
    public:
        //string pointer can't be exposed to user
        std::unordered_set<Token> get_first_set(const Token &tk) const;

        inline std::unordered_set<Token> get_terminal_tokens() const {
            return terminal_list;
        }
        inline std::unordered_set<Token> get_nonterminal_tokens() const {
            return nonterminal_list;
        }
        inline bool contain(const Token &token) const;
        LR_graph get_LR_graph() const;
        explicit operator std::string() const;
    };

    class cfg_builder {
    private:
        cfg inner;
        Token starter;
        void read_nonterminal_list(const std::string &line);
        //only operator>>() function can set nonterminal token list
        void add_nonterminal(const std::vector<std::string> &lst);

        //{token_name, is_terminal}
        static Token get_token(const std::string &token,  bool throw_when_terminal = false);
    public:
        cfg_builder() = default;
        bool add_rule(const Token &head, const std::vector<Token> &body);
        bool add_empty_rule(const Token &token);
        cfg get_cfg();

        friend std::istream& operator>>(std::istream &is, cfg_builder &builder);
    };

    std::istream& operator>>(std::istream &is, cfg_builder &builder);
}

#endif
