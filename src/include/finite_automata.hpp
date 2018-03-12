#ifndef FINITE_AUTOMATA_HPP
#define FINITE_AUTOMATA_HPP

#include<set>
#include<map>
#include<memory>
#include<vector>

namespace parser {

    template<typename T>
    class nfa {
    private:
        typedef int NodeIndex;
        class iterator;
        friend class iterator;
        /*
          null connection
          connection with the same condition
        */
        class node {
        private:
            friend class nfa<T>;
            friend class iterator;
            std::multimap<T, NodeIndex> trans;
            std::set<NodeIndex> nils;
            NodeIndex index;

            void add(const T &cond, NodeIndex ind);
            void add(NodeIndex ind);

            node() = default;
        public:
            std::vector<NodeIndex> transform(const T &cond);
        };
        std::set<NodeIndex> starts;
        std::set<NodeIndex> ends;
        std::vector<std::shared_ptr<node>> nodes;
    public:
        typedef std::shared_ptr<node> Node;
        inline void add_transform(const Node &st, const T &cond, const Node &ed);
        inline void add_transform(const Node &st, const Node &ed); //unconditional
        void set_start(const Node &st);
        void set_end(const Node &ed);

        Node get_new_node();
    };

    template<typename T>
    class nfa<T>::iterator {
    private:
        struct hasher {
            std::size_t operator()(const iterator &iter);
        };

        std::set<NodeIndex> current;
        nfa<T> &nfa_ref;
        friend struct hasher;
    public:
        iterator(nfa<T> &nf, const std::set<NodeIndex> &s) :
            nfa_ref(nf), current(s) {}

        iterator(nfa<T> &nf, std::set<NodeIndex> &&s) :
            nfa_ref(nf), current(s) {}
        inline bool in_end() const;
        /*
          true: there is connection available
          false: otherwise
        */
        bool transform(const T &cond);
    };


}

#endif