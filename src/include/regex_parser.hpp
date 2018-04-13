#ifndef REGEX_PARSER_HPP
#define REGEX_PARSER_HPP

#include<utility>
#include<functional>
#include<set>
#include<unordered_set>
#include<memory>

#include"regex_lex.hpp"

#ifdef DEBUG
#include<iostream>
#endif

namespace parser {
    template<typename T, typename Less = std::less<T>>
    class basic_regex_token {
    public:
        enum MODE { DIRECT, INVERSE };
    private:
        struct unit {
            enum UNIT_TYPE { SINGLE, PAIR };
            UNIT_TYPE type;
            std::pair<T, T> region;
            Less less;

            unit(const T &t1, const T &t2, Less less) : type(PAIR), region({t1, t2}), less(less) {}
            unit(const T &t, Less less) : type(SINGLE), region({t, T()}), less(less) {}

            bool is_single() { return type == SINGLE; }
            friend bool operator<(const unit &u1, const unit &u2) {
                if (u1.type != u2.type) return u1.type < u2.type;
                bool flag = u1.less(u1.region.first, u2.region.first);
                if (u1.type == SINGLE || flag) return flag;
                return u1.less(u1.region.second, u2.region.second);
            }
        };

        enum TYPE { ANY, SET};
        TYPE type;
        MODE mode;
        std::set<unit> set;
        Less less;

        bool item_equal(const T &t1, const T &t2) const {
            return (!less(t1, t2) && !less(t2, t1));
        }
        bool item_in_region(const T &t1, const T &t2, const T &target) const {
            return (!less(target, t1) && !less(t2, target));
        }
    public:
        basic_regex_token(T ch, MODE mode = DIRECT, Less less = Less()) :
            type(SET), mode(mode), set({unit(std::move(ch), less)}), less(less) {}

        basic_regex_token(T ch1, T ch2, MODE mode = DIRECT, Less less = Less()) :
            type(SET), mode(mode), set({unit(std::move(ch1), std::move(ch2), less)}), less(less) {}
        //refuse anything is not a good idea
        basic_regex_token(MODE mode = DIRECT, Less less = Less()) :
            type(ANY), mode(mode), less(std::move(less)) {}

        void reverse() {
            mode == DIRECT ? (mode = INVERSE) : (mode = INVERSE);
        }

        inline bool match(const T &ch) const {
            bool m = false;
            switch (type) {
            case ANY:
                m = true; break;
            case SET:
            {
                for (auto item : set) {
                    if (item.is_single()) {
                        m = item_equal(item.region.first, ch);
                    }
                    else {
                        m = item_in_region(item.region.first, item.region.second, ch);
                    }
                    if (m) break;
                }
                break;
            }
            }
            return mode == DIRECT ? m : !m;
        }

        void add_set(const T &ch) {
            if (type != SET) throw std::runtime_error("expected SET property");
            set.insert(unit(ch, less));
        }

        void add_set(const T &t1, const T &t2) {
            if (type != SET) throw std::runtime_error("expected SET property");
            set.insert(unit(t1, t2, less));
        }

        friend bool operator<(const basic_regex_token<T, Less> &t1, const basic_regex_token<T, Less> &t2) {
            if (t1.mode != t2.mode) return t1.mode < t2.mode;
            if (t1.type != t2.type) return t1.type < t2.type;
            return t1.set < t2.set;
        }

#ifdef DEBUG
        friend std::ostream &operator<<(std::ostream &os, const basic_regex_token<T, Less> &tk) {
            switch (tk.type) {
            case basic_regex_token<T, Less>::SET:
            {
                if (tk.mode == basic_regex_token<T, Less>::DIRECT) os << '[';
                else os << "[^";
                for (auto item : tk.set) {
                    if (item.is_single()) {
                        os << item.region.first;
                    }
                    else {
                        os << item.region.first << '-' << item.region.second;
                    }
                }
                os << ']';
                break;
            }
            case basic_regex_token<T, Less>::ANY:
                os << '.'; break;
            }
            return os;
        }
#endif
    };

    typedef basic_regex_token<char> regex_token;
    typedef basic_regex_token<wchar_t> regex_wtoken; //TODO for unicode parser

    class lexer_graph {
    public:
        class node {
        private:
            std::vector<std::pair<basic_regex_token<char>, node*>> trans;
            std::vector<node*> nils;
            friend class lexer_graph;
        public:
            const std::vector<node*> &get_nils() const {
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
            template<typename Inserter>
            void match(char c, Inserter inserter) {
                for (auto item : trans) {
                    if (item.first.match(c)) *inserter++ = item.second;
                }
            }
            const std::vector<std::pair<basic_regex_token<char>, node*>> &get_transforms() const {
                return trans;
            }
        };
        typedef node *node_type;
    private:
        std::unordered_set<std::unique_ptr<node>> _meta;
        node_type _start = nullptr;
        node_type _end = nullptr;

    public:
        lexer_graph() = default;
        lexer_graph(lexer_graph &&graph, const node_type &start = NULL, const node_type &end = NULL) :
            _meta(std::move(graph._meta)),
            _start(start ? start : graph._start),
            _end(end ? end : graph._end) {}
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
            auto iter = _meta.insert(std::make_unique<node>());
            return iter.first->get();
        }
        void combine(lexer_graph &&graph) {
            _meta.merge(std::move(graph._meta));
        }

        bool is_single_connection(bool throwable = false) {
            return _meta.size() == 2 && _start && _end && _start->trans.size() == 1;
            if (throwable) throw std::runtime_error("expected single connection graph");
        }

        void add_set(char ch) {
            is_single_connection(true);
            _start->trans.front().first.add_set(ch);
        }

        void add_set(char c1, char c2) {
            is_single_connection(true);
            _start->trans.front().first.add_set(c1, c2);
        }

        void inverse() {
            is_single_connection(true);
            _start->trans.front().first.reverse();
        }
#ifdef DEBUG
        friend std::ostream &operator<<(std::ostream &os, const lexer_graph &graph);
#endif
        friend lexer_graph regex_parse(const lexer &_lexer);
    };
    lexer_graph regex_parse(lexer _lexer);
}

#endif
