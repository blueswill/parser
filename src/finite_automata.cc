#include"finite_automata.hpp"

#include<stack>
#include<unordered_map>

namespace parser {
    template<typename T>
    void nfa<T>::node::add(const T &cond, NodeIndex ind) {
        trans.insert({cond, ind});
    }

    template<typename T>
    std::vector<typename nfa<T>::NodeIndex>
    nfa<T>::node::transform(const T &cond) const {
        auto res = trans.equal_range(cond);
        std::vector<NodeIndex> v;
        for (auto iter = res.first; iter != res.second; ++iter) {
            v.push_back(iter->second);
        }
        return v;
    }

    template<typename T>
    void nfa<T>::node::add(NodeIndex ind) {
        nils.insert(ind);
    }

    template<typename T>
    void nfa<T>::add_transform(const Node &st, const T &cond, const Node &ed) {
        st->add(cond, ed->index);
    }

    template<typename T>
    void nfa<T>::add_transform(const Node &st, const Node &ed) {
        st->add(ed->index);
    }

    template<typename T>
    void nfa<T>::set_start(const Node &st) {
        auto iter = starts.find(st->index);
        if (iter == starts.end()) {
            starts.insert(st->index);
        }
    }

    template<typename T>
    void nfa<T>::set_end(const Node &ed) {
        auto iter = ends.find(ed->index);
        if (iter == ends.end()) {
            ends.insert(ed->index);
        }
    }

    template<typename T>
    typename nfa<T>::Node nfa<T>::get_new_node() {
        Node new_node(new node);
        new_node->index = nodes.size();
        nodes.push_back(std::move(new_node));
        return nodes.back();
    }

    template<typename T>
    template<typename Iter>
    std::vector<typename nfa<T>::NodeIndex> nfa<T>::get_closure(Iter start, Iter end) const {
        std::stack<NodeIndex> st;
        for (auto &i = start; i != end; ++i) {
            st.push(*i);
        }
        std::set<NodeIndex> res;
        while (st.size()) {
            auto node = st.top();
            st.pop();
            if (res.find(node) != res.end()) {
                continue;
            }
            res.insert(node);
            for (auto &item : nodes[node]->nils) {
                st.push(item);
            }
        }
        return std::vector<NodeIndex>(res.begin(), res.end());
    }

    template<typename T>
    typename nfa<T>::iterator nfa<T>::get_iterator() const {
        if (starts.empty()) {
            throw std::runtime_error("no start point");
        }
        auto &&res = get_closure(starts.begin(), starts.end());
        return std::move(iterator(*this, res.begin(), res.end()));
    }

    template<typename T>
    bool nfa<T>::contain_transform(const Node &st, const T &cond, const Node &ed) const {
        auto &found = st->trans.equal_range(cond);
        for (auto &iter = found.first; iter != found.second; ++iter) {
            if (iter->second == ed) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    typename nfa<T>::iterator &nfa<T>::iterator::operator=(iterator iter) {
        if (&this->nfa_ref != &iter.nfa_ref) {
            throw std::runtime_error("operator= on non-compatible objects");
        }
        this->current = std::move(iter.current);
        return *this;
    }

    template<typename T>
    bool nfa<T>::iterator::in_end() const {
        for (auto &item : current) {
            if (nfa_ref.ends.find(item) == nfa_ref.ends.end()) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool nfa<T>::iterator::transform(const T &cond) {
        auto &&iter = get_transform(cond);
        *this = iter;
        return current.size();
    }

    template<typename T>
    typename nfa<T>::iterator nfa<T>::iterator::get_transform(const T &cond) const {
        std::vector<NodeIndex> res;
        for (auto &item : current) {
            std::vector<NodeIndex> its(std::move(nfa_ref.nodes[item]->transform(cond)));
            res.insert(res.end(), its.begin(), its.end());
        }
        auto &&cur = nfa_ref.get_closure(res.begin(), res.end());
        return iterator(nfa_ref, cur.begin(), cur.end());
    }

    template<typename T>
    typename std::set<T> nfa<T>::iterator::all_transforms() const {
        std::set<T> result;
        auto &ref = nfa_ref.nodes;
        for (auto &item : current) {
            for (auto &con : ref[item]->trans) {
                result.insert(con.first);
            }
        }
        return std::move(result);
    }

    template<typename T>
    inline bool operator==(const typename nfa<T>::iterator &iter1, const typename nfa<T>::iterator &iter2) {
        return iter1.nfa_ref == iter2.nfa_ref && iter1.current == iter2.current;
    }

    template<typename T>
    std::size_t nfa<T>::iterator::hasher::operator()(const iterator &iter) {
        std::size_t seed = iter.current.size();
        for (auto &item : iter.current) {
            seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    template<typename T>
    nfa<T> dfa<T>::transform_to_dfa(const nfa<T> &nfa_ref) {
        std::unordered_map<iterator, std::pair<typename nfa<T>::Node, bool>, typename nfa<T>::iterator::hasher> record;
        nfa<T> result;

        //dfs nfa->dfa
        std::stack<iterator> st;
        auto &&iter = nfa_ref.get_iterator();
        record.insert({iter, {result.get_new_node(), false}});
        st.push(iter);
        while (st.size()) {
            iterator cur = st.top();
            st.pop();
            auto &p = record[cur];
            if (p.second == true) {
                continue;
            }
            for (auto &con : cur.all_transforms()) {
                iterator &&tmp = cur.get_transform(con);
                auto found = record.find(tmp);
                typename nfa<T>::Node node;
                if (found == record.end()) {
                    node = result.get_new_node();
                    record.insert({tmp, {node, false}});
                }
                else {
                    node = found->second.first;
                }
                if (result.contain_transform(cur, con, node)) {
                    continue;
                }
                result.add_transform(cur, con, node);
            }
            record[cur] = std::make_pair(p.first, true);
        }
        return result;
    }
}

#ifdef DEBUG

#include<vector>
#include<iostream>
parser::nfa<int> graph;
std::vector<parser::nfa<int>::Node> nodes(4);

#define ADDNIL(x,y) (graph.add_transform(nodes[x], nodes[y]))
#define ADD(x,y,c) (graph.add_transform(nodes[x],c,nodes[y]))

int main(void) {
    for (auto &i : nodes) {
        i = graph.get_new_node();
    }
    ADD(0,1,1);
    ADD(0,1,3);
    ADDNIL(0,1);
    ADD(0,2,2);
    ADD(0,3,4);
    ADD(0,3,1);
    ADD(1,3,6);
    ADD(3,1,5);
    ADDNIL(1,2);
    graph.set_start(nodes[0]);
    parser::nfa<int>::iterator iter = graph.get_iterator();
    iter.transform(1);
    for (auto i : iter.all_transforms()) {
        std::cout << i << std::endl;
    }
    return 0;
}

#endif