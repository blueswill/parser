#include"cfg_reduction.hpp"
#include<iostream>
#include<map>
#include<sstream>
#include<queue>

namespace parser {

    LR_reduction::reduction_type::reduction_type(const reduction_type &red) :
        type(red.type) {
            switch (type) {
                case TRANSFORM: info.state = red.info.state; break;
                case REDUCTION: info.reduction = red.info.reduction; break;
                default: break;
            }
        }

    LR_reduction::LR_reduction(const LR_graph &graph) {
        std::map<LR_graph::node_type, std::pair<state_type, bool>> meta;
        std::queue<LR_graph::node_type> q;
        q.push(graph.starter);
        meta.insert({graph.starter, {meta.size(), false}});
        while (q.size()) {
            auto cur = q.front();
            q.pop();
            auto iter = meta.find(cur);
            if (iter->second.second == true) continue;
            iter->second.second = true;
            auto mp = graph.graph.find(cur);
            auto inserter = table.insert({iter->second.first, {}}).first;
            for (auto item : mp->second) {
                q.push(item.second);
                auto next = meta.insert({item.second, {meta.size(), false}}).first;
                inserter->second.insert({item.first, reduction_type(next->second.first)});
            }
            for (auto item : cur->reduction_list()) {
                if (item.second->first == graph.inner.get_extended_token()) {
                    inserter->second.insert({item.first, reduction_type(reduction_type::ACCEPT)});
                }
                inserter->second.insert({item.first, reduction_type(item.second)});
            }
        }
        for (auto item : meta) {
            this->meta.insert({item.second.first, item.first});
        }
    }

#ifdef DEBUG
    LR_reduction::operator std::string() const {
        std::ostringstream os;
        for (auto trs : table) {
            for (auto item : trs.second) {
                switch (item.second.type) {
                    case reduction_type::TRANSFORM:
                        os << trs.first << " -> " << item.second.info.state
                            << " : " << item.first << std::endl;
                        break;
                    case reduction_type::REDUCTION:
                        os << trs.first << " reduct with " << item.second.info.reduction.rule
                            << " at " << item.first << std::endl;
                        break;
                    case reduction_type::ACCEPT:
                        os << trs.first << " accept with " << item.first << std::endl;
                        break;
                    default:
                        break;
                }
            }
        }

        for (auto item : meta) {
            os << std::endl;
            os << item.first << ": \n";
            os << std::string(*item.second) << std::endl;
        }
        return os.str();
    }
#endif
}
