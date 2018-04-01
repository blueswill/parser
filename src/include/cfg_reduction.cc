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
        for (auto item : graph.inner.get_rules()) {
            rule_mp.insert({item, rule_mp.size()});
        }

#ifdef DEBUG
        //generate goto table information
        auto non_list = graph.inner.get_nonterminal_tokens();
        for (auto item : non_list) {
            non_terminal_list.insert({item, non_terminal_list.size()});
        }
#endif
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
#ifdef DEBUG
                g_meta[item.first].push_back({iter->second.first, reduction_type(next->second.first)});
                //generate goto table
                if (!item.first.is_terminal()) {
                    goto_table[this->non_terminal_list[item.first]].push_back({iter->second.first, next->second.first});
                }
#endif
            }
            for (auto item : cur->reduction_list()) {
                if (item.second->first == graph.inner.get_extended_token()) {
                    if (inserter->second.insert({item.first, reduction_type(reduction_type::ACCEPT)}).second == false) {
                        contain_conflict = true; //contain move and reduction conflict
                    }
#ifdef DEBUG
                    g_meta[item.first].push_back({iter->second.first, reduction_type(reduction_type::ACCEPT)});
#endif
                    continue; //don't need to reduct if accepted
                }
                inserter->second.insert({item.first, reduction_type(item.second)});
#ifdef DEBUG
                g_meta[item.first].push_back({iter->second.first, reduction_type(item.second)});
#endif
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

    std::string LR_reduction::generate_code() {
        std::map<Token, std::string> mp {
            {Token("|"), "SPLIT"},
                {Token("^"), "NOT"},
                {Token("("), "LEFT_P"},
                {Token(")"), "RIGHT_P"},
                {Token("["), "LEFT_B"},
                {Token("]"), "RIGHT_B"},
                {Token("*"), "ZERO_MORE"},
                {Token("+"), "ONE_MORE"},
                {Token("?"), "ZERO_ONE"},
                {Token("."), "ANY"},
                {Token("c"), "CHAR"},
                {Token("-"), "REGION"},
                {Token::get_end(), "END_TEXT"}
        };
        std::ostringstream os;

        for (auto item : rule_mp) {
            os << "// " << item.second << ": " << item.first << std::endl;
        }

        os << std::endl;
        for (auto item : non_terminal_list) {
            os << "// " << item.second << ": " << item.first << std::endl;
        }

        os << "void Assert(size_t index, const std::vector<Props> &lst) {\n";
        os << "bool notdo = true;\n";
        os << "switch (index) {\n";
        for (auto item : rule_mp) {
            os << "case " << item.second << ":\n{\n";
            os << "notdo =\n(lst.size() == " << item.first->second.size() << ")";
            //because of rules used are not empty, we needn't check
            //iter != item.first->second.end()
            for (auto i = 0u; i < item.first->second.size(); ++i) {
                auto tk = item.first->second[i];
                os << " &&\n(";
                if (tk.is_terminal())
                    os << "lst[" << i << "].is_token() && lst[" << i << "].token.type == lexer_token::" << mp[tk];
                else
                    os << "!lst[" << i << "].is_token() && lst[" << i << "].non_terminal_index == " << non_terminal_list[tk];
                os << ")";
            }
            os << ";\nbreak;\n";
            os << "}\n";
        }
        os << "}\n";
        os << "if (!notdo) {\n";
        os << "switch (index) {\n";
        for (auto item : rule_mp) {
            os << "case " << item.second << ": " << "throw std::runtime_error(\"" << item.first << " error\");\n";
        }
        os << "}\n";
        os << "}\n}\n\n";

        os << "for (;;) {\n";
        os << "auto &current = generator.get();\n";
        os << "if (!current.is_token()) {\n";
        os << "switch (current.non_terminal_index) {\n";
        for (auto item : goto_table) {
            os << "case " << item.first << ":\n{\n";
            os << "switch(state_stack.top()) {\n";
            for (auto tr : item.second) {
                os << "case " << tr.first << ":\n{\n";
                os << "state_stack.push(" << tr.second << ");\n";
                os << "props_stack.push(std::move(current));\n";
                os << "break;\n";
                os << "}\n";
            }
            os << "}\n";
            os << "}\n";
        }
        os << "}\n";
        os << "generator.consume();\n";
        os << "continue;\n";
        os <<"}\n";
        os << "switch (current.token.type) {\n";
        for (auto item : mp) {
            os << "case lexer_token::" << item.second << ":\n{\n";
            os << "switch (state_stack.top()) {\n";
            const auto actions = g_meta[item.first];
            for (auto action : actions) {
                os << "case " << action.first << ":\n{\n";
                switch (action.second.type) {
                    case reduction_type::TRANSFORM:
                        {
                            os << "state_stack.push(" << action.second.info.state << ");\n";
                            os << "props_stack.push(std::move(current));\n";
                            os << "generator.consume();\n";
                            break;
                        }
                    case reduction_type::REDUCTION:
                        {
                            auto size = action.second.info.reduction.rule->second.size();
                            os << "std::list<Props> tmp;\n";
                            //TODO if there is some empty rules, this code is wrong
                            os << "for (auto i = 0; i < " << size << "; ++i) {\n";
                            os << "auto &item = props_stack.top();\n";
                            os << "tmp.push_front(std::move(item));\n";
                            os << "state_stack.pop();\n";
                            os << "props_stack.pop();\n";
                            os << "}\n";
                            os << "generator.push(";
                            os << non_terminal_list[action.second.info.reduction.rule->first] << ",\n";
                            os << "reduction(";
                            os << rule_mp[action.second.info.reduction.rule];
                            os << ", std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),\nstd::move_iterator<iter_t>(tmp.end()))));\n";
                            break;
                        }
                    case reduction_type::ACCEPT:
                        {
                            os << "auto result = std::move(props_stack.top());\n";
                            os << "return std::move(result.graph);\n";
                            break;
                        }
                    default:
                        break;
                }
                os << "break;\n";
                os << "}\n";
            }
            os << "}\n";
            os << "}\n";
        }
        os << "}\n";
        os << "}\n";
        return os.str();
    }
#endif
}
