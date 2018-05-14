#ifndef CFG_REDUCTION_HPP
#define CFG_REDUCTION_HPP

#include<iosfwd>

#include"cfg_graph.hpp"

namespace parser {
    class LR_reduction {
        friend class LR_graph;
    private:
        typedef ssize_t state_type;
    public:
        class reduction_type {
        public:
            enum TYPE { TRANSFORM, REDUCTION, ACCEPT, ERR };
        private:
            TYPE _type;
            state_type _state;
            cfg::RULE _rule;
        public:
            reduction_type(const state_type &state) :
                _type(TRANSFORM), _state(state) {}
            reduction_type(const cfg::RULE &rule) :
                _type(REDUCTION), _rule(rule) {}
            reduction_type(TYPE type = ACCEPT) : _type(type) {}
            reduction_type(const reduction_type &red);
            TYPE get_type() const { return _type; }
            state_type get_state() const { return _state; }
            cfg::RULE get_rule() const { return _rule; }
        };

        reduction_type next_state(const state_type &state, const Token &tk) const;
        bool conflict() { return _contain_conflict; }
        size_t state_count() const { return _state_meta_info.size(); }
        friend std::ostream &operator<<(std::ostream &os, const LR_reduction &r);
    private:
        std::unordered_map<state_type, std::unordered_map<Token, reduction_type>> _table;
        std::map<state_type, LR_graph::node_type> _state_meta_info;
        state_type _init_state;
        //save rule index
        std::map<cfg::RULE, size_t> _rule_mp;
        //move/reduct conflict
        bool _contain_conflict = false;
        LR_reduction(const LR_graph &graph);
#ifdef CODE_GENERATE
        std::map<Token, std::vector<std::pair<state_type, reduction_type>>> g_meta;
        std::map<Token, size_t> non_terminal_list;
        std::map<size_t, std::vector<std::pair<state_type, state_type>>> goto_table;
    public:
        std::string generate_code();
#endif
    };
}

#endif
