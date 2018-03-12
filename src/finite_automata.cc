#include"finite_automata.hpp"

#include<stack>

namespace parser {
    template<typename T>
    void nfa<T>::node::add(const T &cond, NodeIndex ind) {
        trans.insert({cond, ind});
    }

    template<typename T>
    std::vector<typename nfa<T>::NodeIndex>
    nfa<T>::node::transform(const T &cond) {
        auto res = trans.equal_range(cond);
        std::vector<NodeIndex> v;
        for (auto iter = res.first; iter != res.second; ++iter) {
            v.push_back(iter->second);
        }
        return std::move(v);
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
        std::set<NodeIndex> nodes;
        std::stack<NodeIndex> st;
        auto &ref = nfa_ref.nodes;
        for (auto &item : current) {
            std::vector<NodeIndex> its(std::move(ref[item]->transform(cond)));
            for (auto &i : its) {
                st.push(i);
            }
        }

        //closure dfs
        while (st.size()) {
            NodeIndex cur = st.top();
            st.pop();
            if (nodes.find(cur) != nodes.end()) {
                continue;
            }
            nodes.insert(cur);
            for (auto &item : ref[cur]->nils) {
                st.push(item);
            }
        }

        current = std::move(nodes);
        return current.size();
    }

    template<typename T>
    std::size_t nfa<T>::iterator::hasher::operator()(const iterator &iter) {
        std::size_t seed = iter.current.size();
        for (auto &item : iter.current) {
            seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
}