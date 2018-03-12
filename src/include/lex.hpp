#ifndef TOOL_HPP
#define TOOL_HPP

#include<vector>
#include<string>
#include<utility>
#include<map>
#include<exception>
#include<memory>
#include<list>
#include"finite_automata.hpp"

namespace parser {

    class lex_builder;

    class not_valid_grammer : public std::exception {
    public:
        virtual const char *what() const throw() override;
    };

    class lex {
    public:
        typedef int TokenIndex;
    private:
        friend class lex_builder;

        std::map<TokenIndex, std::map<char, std::list<TokenIndex>>> grammer;
        std::list<TokenIndex> start_points;
        lex_builder &builder;
        lex(lex_builder &builder_) :
            builder(builder_) {}
    };

    class lex_builder {
    private:
        std::map<std::string, lex::TokenIndex> token_index;
        std::unique_ptr<lex> ref;
        bool is_left = true;

        void parse(const std::string &item, bool left);
        std::string parse_righter(const std::string &item);
    public:
        lex_builder() :
            ref(new lex(*this)) {}
        void set_grammer(std::vector<std::string> const &grams, bool left = true);
        void set_start_points(std::vector<std::string> const &start_points);
        lex get_lex();
    };
}

#endif