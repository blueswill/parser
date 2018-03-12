#ifndef FINITE_AUTOMATA_HPP
#define FINITE_AUTOMATA_HPP

#include<set>
#include<map>
#include<memory>
#include<vector>

namespace parser {

    template<typename T>
    class nfa {
    public:
        class iterator;
    private:
        typedef int NodeIndex;
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

            std::vector<NodeIndex> transform(const T &cond) const;
        };
        std::set<NodeIndex> starts;
        std::set<NodeIndex> ends;
        std::vector<std::shared_ptr<node>> nodes;

        template<typename Iter>
        std::vector<NodeIndex> get_closure(Iter start, Iter end) const;
    public:
        typedef std::shared_ptr<node> Node;
        inline void add_transform(const Node &st, const T &cond, const Node &ed);
        inline void add_transform(const Node &st, const Node &ed); //unconditional
        inline void set_start(const Node &st);
        inline void set_end(const Node &ed);

        inline Node get_new_node();

        inline iterator get_iterator() const;
    protected:
        inline bool contain_transform(const Node &st, const T &cond, const Node &ed) const;
    };

    template<typename T>
    bool operator==(const typename nfa<T>::iterator &iter1, const typename nfa<T>::iterator &iter2);

    template<typename T>
    class dfa;

    template<typename T>
    class nfa<T>::iterator {
    private:
        struct hasher {
            std::size_t operator()(const iterator &iter);
        };

        friend struct hasher;
        friend class nfa<T>;
        friend class dfa<T>;

        const nfa<T> &nfa_ref;
        std::vector<NodeIndex> current;

        template<typename Iter>
        iterator(const nfa<T> &nf, Iter start, Iter end) :
            nfa_ref(nf), current(start, end) {}
    public:
        iterator(iterator &&iter) :
            nfa_ref(iter.nfa_ref), current(std::move(iter.current)) {}
        iterator(const iterator &iter) :
            nfa_ref(iter.nfa_ref), current(iter.current) {}
        inline iterator &operator=(iterator right);
        friend bool operator==<T>(const iterator &iter1, const iterator &iter2);
        inline bool in_end() const;
        /*
          true: there is connection available
          false: otherwise
        */
        bool transform(const T &cond);
        iterator get_transform(const T &cond) const;
        inline std::set<T> all_transforms() const;
    };

    template<typename T>
    class dfa : public nfa<T> {
    public:
        class iterator;
    private:
        static nfa<T> transform_to_dfa(const nfa<T> &nfa_ref);
    public:
        dfa(const nfa<T> &nfa_ref) :
            nfa<T>(transform_to_dfa(nfa_ref)) {}
    };
}

#endif