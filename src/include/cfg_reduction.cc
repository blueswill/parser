#include"cfg_reduction.hpp"
#include<iostream>

namespace parser {
    LR_reduction::LR_reduction(const LR_graph &graph) {
        auto t1 = graph.inner.get_terminal_tokens();
        auto t2 = graph.inner.get_nonterminal_tokens();
        tokens.insert(t1.begin(), t1.end());
        tokens.insert(t2.begin(), t2.end());

        std::map<LR_graph::node_type, size_t> nodes;

        std::queue<LR_graph::node_type> q;
        q.push(graph.starter);
    }
}
