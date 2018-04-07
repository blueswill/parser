#ifndef REGEX_MATCHER_HPP
#define REGEX_MATCHER_HPP

#include"regex_parser.hpp"
#include<unordered_map>
#include<stdexcept>
#include<iterator>
#include<queue>
#include<set>
#include<map>

namespace parser {

    template<typename TokenType, typename Hasher> class matcher_builder;

    class no_match_error : public std::exception {
    private:
        std::string _str;
    public:
        no_match_error(const std::string &str = "") : _str(str) {}
        const char *what() const noexcept override {
            return _str.c_str();
        }
    };
    template<typename TokenType, typename Hasher>
    class matcher {
        friend class matcher_builder<TokenType, Hasher>;
    private:
        //following information are all moved from builder
        typedef lexer_graph::node_type node_type;
        typedef std::string::size_type size_type;
        std::vector<lexer_graph> _graphs;
        std::unordered_map<TokenType, size_t, Hasher> _token_type_info;
        std::unordered_multimap<node_type, TokenType> _graph_info;
        Hasher _hasher;

        //the registered data source
        std::string _data;
        size_type _cur_pos;
        size_type _prev_pos;

        matcher(matcher_builder<TokenType, Hasher> &&builder) :
            _graphs(std::move(builder._graphs)),
            _token_type_info(std::move(builder._token_type_info)),
            _graph_info(std::move(builder._graph_info)),
            _hasher(std::move(builder._hasher)),
            _data(), _cur_pos(0), _prev_pos(0) {}
    public:
        class MatchText {
        private:
            TokenType _type;
            std::string _text;
        public:
            MatchText(TokenType type, std::string text) :
                _type(type), _text(text) {}
            TokenType get_type() const { return _type; }
            std::string get_text() const { return _text; }
        };
        void set_source(const std::string &data) {
            _data = data;
            _cur_pos = _prev_pos = 0;
        }

        MatchText next_match() {
            if (_cur_pos == _data.size()) throw no_match_error();
            // token -> forward steps
            std::unordered_map<TokenType, size_type, Hasher> progress(100, _hasher);
            // item : {node, next position}
            std::queue<std::pair<node_type, size_type>> handle_q;
            for (auto &node : _graphs) {
                handle_q.push({node.get_start(), _cur_pos});
            }
            while (handle_q.size()) {
                auto cur = handle_q.front();
                auto cur_node = cur.first;
                auto pos = cur.second;
                handle_q.pop();
                //this matcher mustn't match empty text
                if (pos - _cur_pos) {
                    auto iters = _graph_info.equal_range(cur_node);
                    for (auto iter = iters.first; iter != iters.second; ++iter) {
                        auto inserter = progress.insert({iter->second, pos});
                        if (!inserter.second) {
                            inserter.first->second = pos;
                        }
                    }
                }
                std::vector<node_type> matches;
                if (pos < _data.size()) {
                    cur_node->match(_data[pos++], std::back_inserter(matches));
                    for (auto item : matches) handle_q.push({item, pos});
                }
            }
            if (progress.empty()) throw no_match_error(_data.substr(_cur_pos));
            decltype(progress.begin()) min_iter;
            size_t min_priority = std::numeric_limits<size_t>::max();
            for (auto iter = progress.begin(); iter != progress.end(); ++iter) {
                auto f = _token_type_info.find(iter->first);
                if (f == _token_type_info.end()) continue;
                if (min_priority > f->second) {
                    min_priority = f->second;
                    min_iter = iter;
                }
            }
            _prev_pos = _cur_pos;
            _cur_pos = min_iter->second;
            return MatchText(min_iter->first, _data.substr(_prev_pos, _cur_pos));
        }
    };

    template<typename TokenType, typename Hasher = std::hash<TokenType>>
    class matcher_builder {
    private:
        friend class matcher<TokenType, Hasher>;
        typedef lexer_graph::node_type node_type;
        std::vector<lexer_graph> _graphs;
        // node_ender -> {token type, priority}
        // if p1 < p2 then p1's priority is higher than p2
        std::unordered_multimap<node_type, TokenType> _graph_info;
        std::unordered_map<TokenType, size_t, Hasher> _token_type_info;
        Hasher _hasher;

        template<typename BackInserter>
        void simplify(lexer_graph &&graph, BackInserter inserter) {
            lexer_graph g;
            std::map<std::set<node_type>, std::pair<node_type, bool>> meta;
            std::set<node_type> starter;
            starter.insert(graph.get_start());
            closure(starter);
            meta.insert({starter, {g.get_start(), false}});
            std::queue<std::set<node_type>> q;
            q.push(std::move(starter));

            while (q.size()) {
                auto cur = std::move(q.front());
                q.pop();
                // the existence is guaranteed
                auto found = meta.find(cur);
                if (found->second.second == true) continue;
                found->second.second = true;
                if (cur.find(graph.get_end()) != cur.end())
                    *inserter++ = found->second.first;
                auto cur_node = found->second.first;
                auto items = get_trans(cur);
                for (auto item : items) {
                    node_type des;
                    auto f = meta.find(item.second);
                    if (f == meta.end()) {
                        f = meta.insert({item.second, {g.get_new_node(), false}}).first;
                        q.push(item.second);
                    }
                    des = f->second.first;
                    cur_node->add_connection(des, item.first);
                }
            }

            _graphs.push_back(std::move(g));
        }

        static void closure(std::set<node_type> &nodes) {
            std::queue<node_type> q;
            for (auto item : nodes) {
                q.push(item);
            }
            nodes.clear();
            while (q.size()) {
                auto cur = q.front();
                q.pop();
                if (nodes.insert(cur).second == false) continue;
                for (auto n : cur->get_nils())
                    q.push(n);
            }
            return;
        }

        static std::map<regex_token,std::set<node_type>>
            get_trans(const std::set<node_type> &nodes) {
                std::map<regex_token, std::set<node_type>> ret;
                for (auto node : nodes) {
                    auto trans = node->get_transforms();
                    for (auto tran : trans) {
                        ret[tran.first].insert(tran.second);
                    }
                }
                for (auto iter = ret.begin(); iter != ret.end(); ++iter) {
                    closure(iter->second);
                }
                return ret;
            }

        public:
        matcher_builder(const Hasher hasher = std::hash<TokenType>()) :
            _token_type_info(100, hasher), _hasher(hasher) {}

        bool register_token(const TokenType &token, lexer_graph &&graph, size_t priority) {
            // check whether there is an existing token
            if (_token_type_info.insert({token, priority}).second == false) return false;
            std::vector<node_type> end_nodes;
            simplify(std::move(graph), std::back_inserter(end_nodes));
            for (auto item : end_nodes) {
                _graph_info.insert({item, token});
            }
            return true;
        }

        matcher<TokenType, Hasher> get_matcher() {
            return matcher<TokenType, Hasher>(std::move(*this));
        }
    };
}

#endif
