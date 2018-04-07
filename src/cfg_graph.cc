#include"cfg_graph.hpp"
#include<queue>
#include<sstream>
#include"cfg_reduction.hpp"
#include<iostream>

namespace parser {
#ifdef DEBUG
    LR_graph::Data::Item::operator std::string() const {
        std::ostringstream os;
        os << rule->first << " ->";
        for (auto i = 0ul; i < rule->second.size(); ++i) {
            if (i == dot_pos) {
                os << " *";
            }
            os << ' ' << rule->second[i];
        }
        if (dot_pos == rule->second.size()) {
            os << " *";
        }
        return os.str();
    }
#endif

    std::size_t LR_graph::Data::hasher::operator()(const Item &t1) const {
        size_t seed = t1.rule->second.size() + t1.dot_pos;
        auto h = std::hash<Token>();
        seed ^= h(t1.rule->first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        for (auto item : t1.rule->second) {
            seed ^= h(item) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    template<typename Iter>
    void LR_graph::Data::add_rule(const Item &item, Iter begin, Iter end) {
        auto iter = item_list.insert({item, {}}).first;
        iter->second.insert(begin, end);
    }

    void LR_graph::Data::add_rule(const Item &item, std::initializer_list<Token> tokens) {
        add_rule(item, tokens.begin(), tokens.end());
    }

    bool LR_graph::Data::add_rule(const Item &item, const Token &tk) {
        auto iter = item_list.insert({item, {}}).first;
        return iter->second.insert(tk).second;
    }

    std::set<Token> LR_graph::Data::all_next() const {
        std::set<Token> ret;
        for (auto item : item_list) {
            if (item.first.dot_pos < item.first.rule->second.size())
            ret.insert(item.first.rule->second[item.first.dot_pos]);
        }
        return ret;
    }

    bool LR_graph::Data::is_conflict() const {
        std::map<Token, cfg::RULE> mmp;
        for (auto item : item_list) {
            if (item.first.dot_pos == item.first.rule->second.size()) {
                for (auto tk : item.second) {
                    if (!mmp.insert({tk, item.first.rule}).second) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    std::map<Token, cfg::RULE> LR_graph::Data::reduction_list() const {
        std::map<Token, cfg::RULE> ret;
        for (auto item : item_list) {
            if (item.first.rule->second.size() > item.first.dot_pos)
                continue;
            for (auto tk : item.second) {
                ret.insert({tk, item.first.rule});
            }
        }
        return ret;
    }

#ifdef DEBUG
    LR_graph::Data::operator std::string() const {
        std::ostringstream os;
        for (auto item : item_list) {
            os << std::string(item.first) << "\t";
            for (auto token : item.second) {
                os << token << ' ';
            }
            os << "\n";
        }
        return os.str();
    }
#endif

    size_t LR_graph::hasher::operator()(const Data &t) const {
        size_t seed = t.item_list.size();
        Data::hasher h;
        std::hash<Token> hh;
        for (auto item : t.item_list) {
            seed ^= h(item.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            for (auto token : item.second) {
                seed ^= hh(token) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
        }
        return seed;
    }

    LR_graph::node_type LR_graph::get_node(const Data &d) {
        auto iter = meta.find(d);
        if (iter == meta.end()) {
            iter = meta.insert({d, std::make_unique<Data>(d)}).first;
        }
        return iter->second.get();
    }

    template<typename Iter>
    std::set<Token> LR_graph::compute_follows(Iter begin, Iter end) const {
        std::set<Token> ret;
        for (auto iter = begin; iter != end; ++iter) {
            if (iter->is_terminal()) {
                ret.insert(*iter);
                break;
            }
            auto first = inner.get_first_set(*iter);
            bool hasnull = first.find(Token::get_null()) != first.end();
            ret.insert(first.begin(), first.end());
            if (!hasnull) {
                ret.erase(Token::get_null());
                break;
            }
        }
        return ret;
    }

    LR_graph::Data LR_graph::closure(const Data &data) const {
        Data ret;
        typedef std::pair<Data::Item, Token> Item;
        auto hasher = [](const Item &t) {
            auto hasher1 = Data::hasher();
            auto hasher2 = std::hash<Token>();
            return hasher1(t.first) ^ hasher2(t.second);
        };

        auto equal = [](const Item &t1, const Item &t2) {
            return t1.first == t2.first && t1.second == t2.second;
        };

        std::unordered_set<Item, decltype(hasher), decltype(equal)> record(10, hasher, equal);
        std::queue<Item> q;

        for (auto item : data.item_list) {
            for (auto tk : item.second) {
                q.push({item.first, tk});
            }
        }

        while (q.size()) {
            auto cur = q.front();
            q.pop();
            if (!ret.add_rule(cur.first, cur.second)) {
                continue;
            }
            auto item = cur.first;
            if (item.dot_pos < item.rule->second.size() &&
                    !item.rule->second[item.dot_pos].is_terminal()) {
                auto rules = inner.rule_list.equal_range(item.rule->second[item.dot_pos]);
                auto follows = compute_follows(item.rule->second.begin() + item.dot_pos + 1, item.rule->second.end());
                if (follows.empty() ||
                        follows.find(Token::get_null()) != follows.end()) {
                    follows.erase(Token::get_null());
                    follows.insert(cur.second);
                }
                for (auto iter = rules.first; iter != rules.second; ++iter) {
                    for (auto tk : follows) {
                        if (iter->second.size() == 1 && iter->second[0].is_null()) {
                            q.push({Data::Item(iter, 1), tk});
                        }
                        else {
                            q.push({Data::Item(iter, 0), tk});
                        }
                    }
                }
            }
        }
        return ret;
    }

    LR_graph::Data LR_graph::get_next_data(const Data &data, const Token &trans) {
        Data ret;
        for (auto item : data.item_list) {
            if (item.first.rule->second.size() > item.first.dot_pos &&
                    item.first.rule->second[item.first.dot_pos] == trans) {
                ret.add_rule(Data::Item(item.first.rule, item.first.dot_pos + 1), item.second.begin(), item.second.end());
            }
        }
        return closure(ret);
    }

    void LR_graph::build() {
        cfg::RULE extended_rule = inner.rule_list.find(inner.extended);
        //TODO
        if (extended_rule == inner.rule_list.end()) return;
        auto first = closure(Data({{Data::Item(extended_rule, 0), {Token::get_end()}}}));
        node_type node = get_node(first);
        starter = node;
        std::queue<node_type> wait;
        std::set<node_type> vis;
        wait.push(node);
        while (wait.size()) {
            node_type cur = wait.front();
            wait.pop();
            if (vis.insert(cur).second == false) continue;
            auto iter = graph.insert({cur, {}}).first;
            auto data = *cur;
            for (auto token : data.all_next()) {
                Data next = get_next_data(data, token);
                auto node = get_node(next);
                iter->second.insert({token, node});
                wait.push(node);
            }
        }
    }

    bool LR_graph::is_conflict(Data *data) const {
        for (const auto &item : meta) {
            if (item.first.is_conflict()) {
                if (data) {
                    *data = item.first;
                }
                return true;
            }
        }
        return false;
    }

    LR_reduction LR_graph::get_reduction_table() const {
        return LR_reduction(*this);
    }

#ifdef DEBUG
    LR_graph::operator std::string() const {
        std::map<node_type, int> nodes;
        std::ostringstream os;
        for (const auto &item : meta) {
            auto iter = nodes.insert({item.second.get(), nodes.size()});
            os << iter.first->second << std::endl;
            os << std::string(item.first) << std::endl;
        }
        return os.str();
    }
#endif
}
