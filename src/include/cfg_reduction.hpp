#ifndef CFG_REDUCTION_HPP
#define CFG_REDUCTION_HPP

#include"cfg_graph.hpp"
#include<iosfwd>

namespace parser {
    class LR_reduction {
    public:
        struct reduction_type {
            size_t number;
            Token next;
        };

        typedef ssize_t state;

        ssize_t next_state(const state &st, const Token &tk) const;
        reduction_type reduction(const state &st, const Token &tk) const;

        friend std::ostream &operator<<(std::ostream &os, const LR_reduction &lr_re);
    private:
        std::unordered_set<Token> tokens;
        std::unordered_map<size_t, std::unordered_map<Token, size_t>> tr_state;
        std::unordered_map<size_t, std::unordered_map<Token, reduction_type>> reduction_state;

        LR_reduction(const LR_graph &graph);
    };
}

#endif
