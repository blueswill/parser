#include"lex.hpp"

namespace parser {
    const char *not_valid_grammer::what() const throw() {
        return "lex prammer is not valid";
    }

    void lex_builder::set_grammer(const std::vector<std::string> &grams, bool left) {
        for (auto &item : grams) {
            parse(item, left);
        }
    }

    void lex_builder::parse(const std::string &item, bool left) {
        std::string::size_type split = item.find_first_of("->");
        if (split == std::string::npos) {
            throw not_valid_grammer();
        }
        std::string starter, righter;
        char follow;
        lex::TokenIndex starter_index, righter_index;
        starter = item.substr(0, split);
        if (item.length() == split + 2) {
            throw not_valid_grammer();
        }
        if (left) {
            righter = parse_righter(item.substr(split + 2));
            std::string attach;
            if (righter.length()) attach = item.substr(split + 4 + righter.length());
            else attach = item.substr(split + 2);
            if (attach.length() >2 || attach.length() < 1 ||
                attach.length() == 2 && attach[0] != '\\') {
                throw not_valid_grammer();
            }
            else if (attach.length() == 2) {
                follow = attach[1];
            }
        }
        else if (item[split + 2] == '\\') {
            follow = item[split + 3];
            if (follow == '\0') {
                throw not_valid_grammer();
            }
            righter = parse_righter(item.substr(split + 4));
        }
        else {
            follow = item[split + 2];
            righter = parse_righter(item.substr(split + 3));
        }
        //store the data
        auto iter = token_index.find(righter);
        if (iter == token_index.end()) {
            righter_index = token_index.size();
            token_index[righter] = righter_index;
        }
        else {
            righter_index = iter->second;
        }
        iter = token_index.find(starter);
        if (iter == token_index.end()) {
            starter_index = token_index.size();
            token_index[starter] = starter_index;
        }
        else {
            starter_index = iter->second;
        }
        ref->grammer[starter_index][follow].push_back(lex::TokenIndex(righter_index));
    }

    std::string lex_builder::parse_righter(const std::string &item) {
        if (item[0] != '{') {
            throw not_valid_grammer();
        }
        std::string::size_type start_pos = 1, end_pos = 1;
        while (end_pos < item.size() && item[end_pos] && item[end_pos] != '}') {
            ++end_pos;
            if (item[end_pos] == '\\') {
                ++end_pos;
            }
        }
        if (item[end_pos] != '}') {
            throw not_valid_grammer();
        }
        return std::move(item.substr(start_pos, end_pos - start_pos));
    }
}

#ifdef DEBUG
int main(int argc, char **argv) {
    parser::lex_builder builder;
    builder.set_grammer({argv[1]}, true);
    return 0;
}
#endif
