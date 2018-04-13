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
        static bool is_end(const Token &tk) { return tk.name == end_val; }
        static bool is_null(const Token &tk) { return tk.name == null_val; }
        static Token get_end() { return Token(end_val); }
        static Token get_null() { return Token(null_val); }

        bool is_end() const { return is_end(*this); }
        bool is_null() const { return is_null(*this); }
        bool is_terminal() const { return type == TERM; }
        std::string get_name() const { return name; }

        Token() : type(TERM), name(null_val) {}
        Token(const std::string &name, bool isterminal = true) :
            type(isterminal ? TERM : NON_TERM), name(name) {}
        Token(const Token &tk) : type(tk.type), name(tk.name) {}

        friend struct std::hash<Token>;

        friend std::ostream &operator<<(std::ostream &os, const Token &tk);
        friend bool operator<(const Token &t1, const Token &t2) {
            return t1.name < t2.name || (t1.name == t2.name && t1.type < t2.type);
        }
    private:
        enum TYPE { TERM,NON_TERM };
        TYPE type;
        std::string name;

        const static std::string end_val;
        const static std::string null_val;
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
    public:
        typedef decltype(rule_list.cbegin()) RULE;

        std::unordered_set<Token> get_first_set(const Token &tk) const;
        inline const std::unordered_set<Token> &get_terminal_tokens() const {
            return terminal_list;
        }
        inline const std::unordered_set<Token> &get_nonterminal_tokens() const {
            return nonterminal_list;
        }
        inline bool contain(const Token &token) const;
        std::vector<RULE> get_rules() const {
            std::vector<RULE> ret;
            for (auto iter = rule_list.cbegin(); iter != rule_list.cend(); ++iter) {
                ret.push_back(iter);
            }
            return ret;
        }
        Token get_extended_token() const { return extended; }
        LR_graph get_LR_graph() const;

        friend std::ostream &operator<<(std::ostream &os, const cfg &c);
    };

    class cfg_builder {
    private:
        cfg inner;
        Token starter;

        Token get_token(const std::string &token,  bool throw_when_terminal = false);
    public:
        cfg_builder() = default;
        cfg get_cfg();
        bool add_rule(const Token &head, const std::vector<Token> &body);
        bool add_empty_rule(const Token &token);
        template<typename InputIter>
        void add_nonterminal_tokens(InputIter begin, InputIter end) {
            inner.nonterminal_list.insert(begin, end);
        }
        void set_starter(const Token &token) { starter = token; }
    };

    bool operator<(const cfg::RULE &r1, const cfg::RULE &r2);
    std::ostream &operator<<(std::ostream &os, const cfg::RULE &rule);
}

#endif
