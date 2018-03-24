#ifndef TOOL_HPP
#define TOOL_HPP

#include<vector>
#include<string>
#include<exception>
#include<unordered_map>
#include"finite_automata.hpp"

namespace parser {

    class lex_builder;

    class not_valid_grammer : public std::exception {
    public:
        virtual const char *what() const throw() override;
    };

    class lex {
    public:
        template<typename Iter, typename StartIter>
        lex(Iter start, Iter end, StartIter token_start, StartIter token_end);

        dfa<char> get_dfa() const;
    private:
        typedef unsigned TokenIndex;
        //rule_list[0] indicates nil element
        //-1 to indicate nil
        std::vector<std::map<int, std::vector<TokenIndex>>> rule_list;
        std::unordered_map<std::string, TokenIndex> token_mp;
        std::vector<TokenIndex> start_token;
        /*
          true: A->aB|b
          false: A->Ba|b
        */
        bool left_linear = false;
        bool parse(const std::string &str, bool check_left);
        dfa<char> get_dfa_left() const;
        dfa<char> get_dfa_right() const;
        std::unordered_map<std::string, TokenIndex>::iterator find_token(const std::string &token, bool insertable = true);
    };
}

#endif
