#ifndef REGEX_MATCHER_HPP
#define REGEX_MATCHER_HPP

#include<unordered_map>
#include<stdexcept>
#include<iterator>
#include<queue>
#include<set>
#include<map>

#include"regex_parser.hpp"

namespace parser {

    template<typename TokenType, typename Hasher> class matcher_builder;

    template<typename Iterator>
    class no_match_error : public std::exception {
    private:
        Iterator _iter;
    public:
        no_match_error(Iterator iter) : _iter(iter) {}
        const char *what() const noexcept override {
            return "no match exists";
        }
        Iterator get_iter() const { return _iter; }
    };

    template<typename TokenType, typename Hasher = std::hash<TokenType>>
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

        matcher(matcher_builder<TokenType, Hasher> &&builder) :
            _graphs(std::move(builder._graphs)),
            _token_type_info(std::move(builder._token_type_info)),
            _graph_info(std::move(builder._graph_info)),
            _hasher(std::move(builder._hasher)) {}
    public:
        template<typename InputIterator>
        std::pair<std::vector<TokenType>, InputIterator> match(InputIterator begin, InputIterator end) {
            InputIterator _cur_pos = begin;
            if (_cur_pos == end) throw no_match_error<InputIterator>(end);
            // token -> forward steps
            std::unordered_map<TokenType, InputIterator, Hasher> progress(100, _hasher);
            // item : {node, next position}
            std::queue<std::pair<node_type, InputIterator>> handle_q;
            for (auto &node : _graphs) {
                handle_q.push({node.get_start(), _cur_pos});
            }
            while (handle_q.size()) {
                auto cur = handle_q.front();
                auto cur_node = cur.first;
                auto pos = cur.second;
                handle_q.pop();
                //this matcher mustn't match empty text
                if (pos != _cur_pos) {
                    auto iters = _graph_info.equal_range(cur_node);
                    for (auto iter = iters.first; iter != iters.second; ++iter) {
                        auto inserter = progress.insert({iter->second, pos});
                        if (!inserter.second) {
                            inserter.first->second = pos;
                        }
                    }
                }
                std::vector<node_type> matches;
                if (pos != end) {
                    cur_node->match(*pos, std::back_inserter(matches));
                    ++pos;
                    for (auto item : matches) handle_q.push({item, pos});
                }
            }
            if (progress.empty()) throw no_match_error<InputIterator>(_cur_pos);
            auto max_iter = std::max_element(progress.begin(), progress.end(),
                                             [](const decltype(*progress.begin()) &t1, const decltype(*progress.begin()) &t2) {
                                                 return t1.second < t2.second;
                                             });
            size_t min_priority = std::numeric_limits<size_t>::max();
            std::vector<TokenType> ret;
            for (auto iter = progress.begin(); iter != progress.end(); ++iter) {
                if (iter->second != max_iter->second) continue;
                auto f = _token_type_info.find(iter->first);
                if (f == _token_type_info.end()) continue;
                if (min_priority > f->second) {
                    min_priority = f->second;
                    ret.clear();
                }
                if (min_priority == f->second) ret.push_back(iter->first);
            }
            _cur_pos = max_iter->second;
            return std::make_pair(std::move(ret), _cur_pos);
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

        bool register_token(const TokenType &token, const std::string &s, size_t priority = 0) {
            return register_token(token, regex_parse(lexer(s)), priority);
        }

        bool register_token(const TokenType &token, lexer_graph &&graph, size_t priority = 0) {
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
