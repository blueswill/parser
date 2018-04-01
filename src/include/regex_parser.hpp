#ifndef REGEX_PARSER_HPP
#define REGEX_PARSER_HPP

#include<utility>
#include<functional>
#include<vector>
#include<unordered_set>
#include"regex_lex.hpp"
#include"utility.hpp"

namespace parser {
    template<typename T, typename Less = std::less<T>>
    class basic_regex_token {
        public:
            enum MODE { DIRECT, INVERSE };
        private:
            enum TYPE { ANY, SINGLE, REGION };
            TYPE type;
            MODE mode;

            T single;
            std::pair<T, T> region;
            Less less;

        public:
            basic_regex_token(T ch, MODE mode = DIRECT, Less less = Less()) :
                type(SINGLE), mode(mode), single(std::move(ch)), less(std::move(less)) {}

            basic_regex_token(T ch1, T ch2, MODE mode = DIRECT, Less less = Less()) :
                type(REGION), mode(mode), region(std::move(ch1), std::move(ch2)), less(std::move(less)) {}
            //refuse anything is not a good idea
            basic_regex_token(MODE mode = DIRECT, Less less = Less()) :
                type(ANY), mode(mode), less(std::move(less)) {}

            void reverse() {
                mode == DIRECT ? (mode = INVERSE) : (mode = INVERSE);
            }

            inline bool match(const T &ch) const {
                bool m = false;
                switch (type) {
                    case ANY: m = true; break;
                    case SINGLE: m = (!less(ch, single) && !less(single, ch)); break;
                    case REGION: m = (!less(ch, region.first) && !less(region.second, ch)); break;
                }
                return mode == DIRECT ? m : !m;
            }
    };

    typedef basic_regex_token<char> regex_token;
    typedef basic_regex_token<wchar_t> regex_wtoken; //TODO for unicode parser

    class lexer_graph {
    public:
        class node {
        private:
            std::vector<std::pair<basic_regex_token<char>, node*>> trans;
            std::vector<node*> nils;
        public:
            std::vector<node*> get_nils() const {
                return nils;
            }
            void add_connection(node *n) {
                nils.push_back(n);
            }
            void add_connection(node *n, basic_regex_token<char> tk) {
                trans.push_back({std::move(tk), n});
            }
            void reverse() {
                for (auto &item : trans) {
                    item.first.reverse();
                }
            }
        };
        typedef node *node_type;
    private:
        std::unordered_set<unique_ptr<node>> _meta;
        node_type _start = nullptr;
        node_type _end = nullptr;
    public:
        lexer_graph() = default;
        lexer_graph(lexer_graph &&graph) :
            _meta(std::move(graph._meta)), _start(graph._start), _end(graph._end) {}
        lexer_graph &operator=(lexer_graph &&graph) {
            _meta = std::move(graph._meta);
            _start = std::move(graph._start);
            _end = std::move(graph._end);
            return *this;
        }
        node_type get_start() {
            return _start ? _start : (_start = get_new_node());
        }
        node_type get_end() {
            return _end ? _end : (_end = get_new_node());
        }
        node_type get_new_node() {
            auto iter = _meta.insert(make_unique<node>());
            return iter.first->get();
        }
        void combine(lexer_graph &&graph) {
            _meta.merge(std::move(graph._meta));
        }
        friend lexer_graph regex_parse(const lexer &_lexer);
    };

    lexer_graph regex_parse(const lexer &_lexer);
}

#endif
