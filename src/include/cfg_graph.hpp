#ifndef CFG_GRAPH_HPP
#define CFG_GRAPH_HPP

#include<initializer_list>
#include<set>
#include<map>
#include"forward.hpp"
#include"cfg.hpp"

namespace parser {
    class LR_graph {
    private:
        cfg inner;
    public:
        struct Data {
            struct Item {
                cfg::RULE rule;
                size_t dot_pos;
                Item(cfg::RULE rule, size_t pos) :
                    rule(rule), dot_pos(pos) {}
                Item() {}
                friend bool operator==(const Item &i1, const Item &i2) {
                    return i1.rule == i2.rule && i1.dot_pos == i2.dot_pos;
                }
                explicit operator std::string() const;
            };

            struct hasher {
                size_t operator()(const Item &t1) const;
            };

            std::unordered_map<Item, std::set<Token>, hasher> item_list;

            template<typename Iter>
            Data(Iter begin, Iter end) :
                item_list(begin, end) {}
            Data(std::initializer_list<std::pair<Item, std::initializer_list<Token>>> items = {}) :
                Data(items.begin(), items.end()) {}

            template<typename Iter>
            inline void add_rule(const Item &item, Iter begin, Iter end);

            inline void add_rule(const Item &item,
                                 std::initializer_list<Token> tokens = {});

            std::set<Token> all_next() const;

            inline bool add_rule(const Item &item, const Token &tk);

            friend bool operator==(const Data &d1, const Data &d2) {
                return d1.item_list == d2.item_list;
            }

            bool is_conflict() const;
            std::map<Token, cfg::RULE> reduction_list() const;
#ifdef DEBUG
            explicit operator std::string() const;
#endif
        };
        //end Data

        struct hasher {
            size_t operator()(const Data &t) const;
        };
        std::unordered_map<Data, std::unique_ptr<Data>, hasher> meta;
        std::map<Data*, std::map<Token, Data*>> graph;
    public:
        typedef decltype(graph)::key_type node_type;
    private:
        node_type starter;
        inline node_type get_node(const Data &data);

        template<typename Iter>
        std::set<Token> compute_follows(Iter begin, Iter end) const;

        Data closure(const Data &data) const;

        Data get_next_data(const Data &data, const Token &trans);
        void build();

        friend class LR_reduction;

    public:
        LR_graph(const cfg &c) :
            inner(c) {
            build();
        }

        bool is_conflict(Data *data = nullptr) const;
        LR_reduction get_reduction_table() const;

#ifdef DEBUG
        explicit operator std::string() const;
#endif
    };
}

#endif
