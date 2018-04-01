#ifndef CFG_REDUCTION_HPP
#define CFG_REDUCTION_HPP

#include"cfg_graph.hpp"
#include<iosfwd>

namespace parser {
    class LR_reduction {
        friend class LR_graph;
    private:
        typedef ssize_t state_type;
    public:
        struct reduction_type {
            enum TYPE { TRANSFORM, REDUCTION, ERR, ACCEPT};
            struct INFO {
                state_type state;
                struct reduction {
                    cfg::RULE rule;
                    reduction(const cfg::RULE &rule) :
                        rule(rule) {}
                    reduction() = default;
                } reduction;

                INFO(const state_type &st) : state(st) {}
                INFO(const cfg::RULE &rule) :
                    reduction(rule) {}
                INFO() {}
                ~INFO() {}
            };
            TYPE type;
            INFO info;
            reduction_type(const state_type &state) :
                type(TRANSFORM), info(state) {}
            reduction_type(const cfg::RULE &rule) :
                type(REDUCTION), info(rule) {}
            reduction_type(TYPE type = ERR) :
                type(type) {}
            reduction_type(const reduction_type &red);
        };

        reduction_type next_state(const state_type &state, const Token &tk);
#ifdef DEBUG
        explicit operator std::string() const;
        std::string generate_code();
        bool conflict() { return contain_conflict; }
#endif
    private:
        std::unordered_map<state_type, std::unordered_map<Token, reduction_type>> table;
        std::map<state_type, LR_graph::node_type> meta;
        state_type init;
        //移进归约冲突
        bool contain_conflict = false;

#ifdef DEBUG
        std::map<Token, std::vector<std::pair<state_type, reduction_type>>> g_meta;
        std::map<Token, size_t> non_terminal_list;
        std::map<size_t, std::vector<std::pair<state_type, state_type>>> goto_table;
#endif
        //save rule index
        std::map<cfg::RULE, size_t> rule_mp;

        LR_reduction(const LR_graph &graph);
    };
}

#endif
