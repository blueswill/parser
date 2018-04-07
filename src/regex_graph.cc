#include"regex_parser.hpp"
#include<iostream>
#include<map>
#include<queue>

namespace parser {
#ifdef DEBUG
    std::ostream &operator<<(std::ostream &os, const lexer_graph &graph) {
        std::map<lexer_graph::node_type, std::pair<size_t, bool>> vis;
        std::queue<lexer_graph::node_type> q;
        q.push(graph._start);
        while (q.size()) {
            auto cur = q.front();
            q.pop();
            auto iter = vis.insert({cur, {vis.size(), false}}).first;
            if (iter->second.second == true) continue;
            iter->second.second = true;
            for (auto item : iter->first->get_transforms()) {
                auto des = vis.insert({item.second, {vis.size(), false}});
                os << iter->second.first << " -> " << des.first->second.first << " : "
                    << item.first << std::endl;
                if (des.second == true) {
                    q.push(item.second);
                }
            }
            for (auto item : iter->first->get_nils()) {
                auto des = vis.insert({item, {vis.size(), false}});
                os << iter->second.first << " -> " << des.first->second.first << " : "
                    << "nil\n";
                if (des.second == true) {
                    q.push(item);
                }
            }
        }
        return os;
    }
#endif
}
