#include"regex_parser.hpp"
#include"cfg.hpp"
#include<vector>
#include<stack>
#include<list>

namespace parser {

    struct Props {
        enum TYPE { TOKEN, NON_TOKEN };
        TYPE type;
        lexer_token token;
        size_t non_terminal_index;
        lexer_graph graph;

        Props(lexer_token token) :
            type(TOKEN), token(token) {}
        Props(lexer_graph &&graph, size_t index) :
            type(NON_TOKEN), non_terminal_index(index), graph(std::move(graph)) {}
        bool is_token() const { return type == TOKEN; }
    };

    class generator {
        private:
            lexer _lexer;
            std::list<Props> q;
        public:
            generator(lexer g) : _lexer(g) {}
            Props &get() {
                if (!q.size())
                    q.push_back(Props(_lexer.consume()));
                return *q.begin();
            }
            Props consume() {
                if (!q.size()) {
                    q.push_back(Props(_lexer.consume()));
                }
                auto ret = std::move(q.front());
                q.pop_front();
                return ret;
            }
            void push(size_t index, lexer_graph &&graph) {
                q.push_front(Props(std::move(graph), index));
            }
    };

#ifdef DEBUG
    void Assert(size_t index, const std::vector<Props> &lst) {
        bool notdo = true;
        switch (index) {
            case 8:
                {
                    notdo =
                        (lst.size() == 3) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 1) &&
                        (lst[1].is_token() && lst[1].token.type == lexer_token::SPLIT) &&
                        (!lst[2].is_token() && lst[2].non_terminal_index == 2);
                    break;
                }
            case 7:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 2);
                    break;
                }
            case 12:
                {
                    notdo =
                        (lst.size() == 3) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::LEFT_P) &&
                        (!lst[1].is_token() && lst[1].non_terminal_index == 1) &&
                        (lst[2].is_token() && lst[2].token.type == lexer_token::RIGHT_P);
                    break;
                }
            case 9:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::ANY);
                    break;
                }
            case 10:
                {
                    notdo =
                        (lst.size() == 3) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::LEFT_B) &&
                        (!lst[1].is_token() && lst[1].non_terminal_index == 5) &&
                        (lst[2].is_token() && lst[2].token.type == lexer_token::RIGHT_B);
                    break;
                }
            case 11:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::CHAR);
                    break;
                }
            case 13:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 6);
                    break;
                }
            case 14:
                {
                    notdo =
                        (lst.size() == 2) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::NOT) &&
                        (!lst[1].is_token() && lst[1].non_terminal_index == 6);
                    break;
                }
            case 16:
                {
                    notdo =
                        (lst.size() == 4) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::CHAR) &&
                        (lst[1].is_token() && lst[1].token.type == lexer_token::REGION) &&
                        (lst[2].is_token() && lst[2].token.type == lexer_token::CHAR) &&
                        (!lst[3].is_token() && lst[3].non_terminal_index == 6);
                    break;
                }
            case 17:
                {
                    notdo =
                        (lst.size() == 2) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::CHAR) &&
                        (!lst[1].is_token() && lst[1].non_terminal_index == 6);
                    break;
                }
            case 15:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (lst[0].is_token() && lst[0].token.type == lexer_token::CHAR);
                    break;
                }
            case 4:
                {
                    notdo =
                        (lst.size() == 2) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 4) &&
                        (lst[1].is_token() && lst[1].token.type == lexer_token::ZERO_MORE);
                    break;
                }
            case 3:
                {
                    notdo =
                        (lst.size() == 2) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 4) &&
                        (lst[1].is_token() && lst[1].token.type == lexer_token::ONE_MORE);
                    break;
                }
            case 2:
                {
                    notdo =
                        (lst.size() == 2) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 4) &&
                        (lst[1].is_token() && lst[1].token.type == lexer_token::ZERO_ONE);
                    break;
                }
            case 1:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 4);
                    break;
                }
            case 6:
                {
                    notdo =
                        (lst.size() == 2) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 3) &&
                        (!lst[1].is_token() && lst[1].non_terminal_index == 2);
                    break;
                }
            case 5:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 3);
                    break;
                }
            case 0:
                {
                    notdo =
                        (lst.size() == 1) &&
                        (!lst[0].is_token() && lst[0].non_terminal_index == 1);
                    break;
                }
        }
        if (!notdo) {
            switch (index) {
                case 0: throw std::runtime_error("<<extender>> -> E error");
                case 1: throw std::runtime_error("M -> F error");
                case 2: throw std::runtime_error("M -> F ? error");
                case 3: throw std::runtime_error("M -> F + error");
                case 4: throw std::runtime_error("M -> F * error");
                case 5: throw std::runtime_error("T -> M error");
                case 6: throw std::runtime_error("T -> M T error");
                case 7: throw std::runtime_error("E -> T error");
                case 8: throw std::runtime_error("E -> E | T error");
                case 9: throw std::runtime_error("F -> . error");
                case 10: throw std::runtime_error("F -> [ K ] error");
                case 11: throw std::runtime_error("F -> c error");
                case 12: throw std::runtime_error("F -> ( E ) error");
                case 13: throw std::runtime_error("K -> K' error");
                case 14: throw std::runtime_error("K -> ^ K' error");
                case 15: throw std::runtime_error("K' -> c error");
                case 16: throw std::runtime_error("K' -> c - c K' error");
                case 17: throw std::runtime_error("K' -> c K' error");
            }
        }
    }
#endif
    lexer_graph reduction(size_t number, std::vector<Props> &&lst) {
#ifdef DEBUG
        Assert(number, lst);
#endif
        switch (number) {
            // E -> E | T
            case 8:
                {
                    lexer_graph ret(std::move(lst.front().graph));
                    auto ast = lst.back().graph.get_start(), aed = lst.back().graph.get_end();
                    auto st = ret.get_start(), ed = ret.get_end();
                    st->add_connection(ast);
                    aed->add_connection(ed);
                    ret.combine(std::move(lst.back().graph));
                    return ret;
                }
                // E -> T
            case 7:
                // F -> ( E )
            case 12:
                // F -> [ K ]
            case 10:
                // K -> K'
            case 13:
                // M -> F
            case 1:
                // T -> M
            case 5:
                // extended -> E
            case 0: return std::move(lst.front().graph);
                    // T -> M T
            case 6:
                    {
                        lexer_graph ret(std::move(lst.front().graph));
                        auto ed = ret.get_end();
                        ed->add_connection(lst.back().graph.get_start());
                        ret.combine(std::move(lst.back().graph));
                        return ret;
                    }
                    // M -> F *
            case 4:
                    {
                        lexer_graph ret(std::move(lst.front().graph));
                        auto st = ret.get_start(), ed = ret.get_end();
                        st->add_connection(ed);
                        ed->add_connection(st);
                        return ret;
                    }
                    // M -> F +
            case 3:
                    {
                        lexer_graph ret(std::move(lst.front().graph));
                        ret.get_end()->add_connection(ret.get_start());
                        return ret;
                    }
                    // M -> F ?
            case 2:
                    {
                        lexer_graph ret(std::move(lst.front().graph));
                        ret.get_start()->add_connection(ret.get_end());
                        return ret;
                    }
                    // F -> c
            case 11:
                    {
                        lexer_graph ret;
                        auto st = ret.get_start();
                        auto ed = ret.get_end();
                        st->add_connection(ed, regex_token(lst[0].token.ch));
                        return ret;
                    }
                    // F -> .
            case 9:
                    {
                        lexer_graph ret;
                        auto st = ret.get_start();
                        auto ed = ret.get_end();
                        st->add_connection(ed, regex_token());
                        return ret;
                    }
                    // K -> ^ K'
            case 14:
                    {
                        auto ret = std::move(lst[1].graph);
                        ret.get_start()->reverse();
                        return ret;
                    }
                    // K' -> c K'
            case 17:
                    {
                        auto ret = std::move(lst[1].graph);
                        ret.get_start()->add_connection(ret.get_end(), regex_token(lst[0].token.ch));
                        return ret;
                    }
                    // K' -> c - c K'
            case 16:
                    {
                        auto ret = std::move(lst[3].graph);
                        ret.get_start()->add_connection(ret.get_end(), regex_token(lst[0].token.ch, lst[2].token.ch));
                        return ret;
                    }
                    // K' -> c
            case 15:
                    {
                        lexer_graph ret;
                        ret.get_start()->add_connection(ret.get_end(), regex_token(lst[0].token.ch));
                        return ret;
                    }
            default:
                    throw std::runtime_error("unknown producer " + std::to_string(number));
                    break;
        }
    }

    lexer_graph regex_parse(lexer &g) {
        generator generator(g);
        std::stack<Props> props_stack;
        std::stack<int> state_stack;
        typedef std::list<Props>::iterator iter_t;
        for (;;) {
            auto &current = generator.get();
            if (!current.is_token()) {
                switch (current.non_terminal_index) {
                    case 1:
                        {
                            switch(state_stack.top()) {
                                case 0:
                                    {
                                        state_stack.push(3);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 1:
                                    {
                                        state_stack.push(11);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 9:
                                    {
                                        state_stack.push(27);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                            }
                        }
                    case 2:
                        {
                            switch(state_stack.top()) {
                                case 0:
                                    {
                                        state_stack.push(6);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 1:
                                    {
                                        state_stack.push(14);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 5:
                                    {
                                        state_stack.push(22);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 9:
                                    {
                                        state_stack.push(14);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 13:
                                    {
                                        state_stack.push(34);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 17:
                                    {
                                        state_stack.push(37);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 29:
                                    {
                                        state_stack.push(45);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                            }
                        }
                    case 3:
                        {
                            switch(state_stack.top()) {
                                case 0:
                                    {
                                        state_stack.push(5);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 1:
                                    {
                                        state_stack.push(13);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 5:
                                    {
                                        state_stack.push(5);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 9:
                                    {
                                        state_stack.push(13);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 13:
                                    {
                                        state_stack.push(13);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 17:
                                    {
                                        state_stack.push(5);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 29:
                                    {
                                        state_stack.push(13);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                            }
                        }
                    case 4:
                        {
                            switch(state_stack.top()) {
                                case 0:
                                    {
                                        state_stack.push(4);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 1:
                                    {
                                        state_stack.push(12);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 5:
                                    {
                                        state_stack.push(21);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 9:
                                    {
                                        state_stack.push(12);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 13:
                                    {
                                        state_stack.push(33);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 17:
                                    {
                                        state_stack.push(36);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 29:
                                    {
                                        state_stack.push(44);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                            }
                        }
                    case 5:
                        {
                            switch(state_stack.top()) {
                                case 7:
                                    {
                                        state_stack.push(23);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 15:
                                    {
                                        state_stack.push(35);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                            }
                        }
                    case 6:
                        {
                            switch(state_stack.top()) {
                                case 7:
                                    {
                                        state_stack.push(24);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 15:
                                    {
                                        state_stack.push(24);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 25:
                                    {
                                        state_stack.push(39);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 26:
                                    {
                                        state_stack.push(42);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 40:
                                    {
                                        state_stack.push(42);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 47:
                                    {
                                        state_stack.push(48);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                                case 49:
                                    {
                                        state_stack.push(42);
                                        props_stack.push(std::move(current));
                                        break;
                                    }
                            }
                        }
                }
                generator.consume();
                continue;
            }
            switch (current.token.type) {
                case lexer_token::END_TEXT:
                    {
                        switch (state_stack.top()) {
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 3:
                                {
                                    auto result = std::move(props_stack.top());
                                    return std::move(result.graph);
                                    break;
                                }
                            case 4:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 5:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(5, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 6:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(7, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 18:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 19:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 20:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 22:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(6, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 36:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 37:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(8, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::LEFT_P:
                    {
                        switch (state_stack.top()) {
                            case 0:
                                {
                                    state_stack.push(1);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 1:
                                {
                                    state_stack.push(9);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 4:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 5:
                                {
                                    state_stack.push(1);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 9:
                                {
                                    state_stack.push(9);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 12:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 13:
                                {
                                    state_stack.push(9);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 17:
                                {
                                    state_stack.push(1);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 18:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 19:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 20:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 29:
                                {
                                    state_stack.push(9);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 30:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 31:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 32:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 36:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::RIGHT_P:
                    {
                        switch (state_stack.top()) {
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 11:
                                {
                                    state_stack.push(28);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 12:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 13:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(5, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 14:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(7, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 27:
                                {
                                    state_stack.push(43);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 30:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 31:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 32:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 34:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(6, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 45:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(8, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::ZERO_MORE:
                    {
                        switch (state_stack.top()) {
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 4:
                                {
                                    state_stack.push(18);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 12:
                                {
                                    state_stack.push(30);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    state_stack.push(18);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    state_stack.push(30);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 36:
                                {
                                    state_stack.push(18);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    state_stack.push(30);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::ONE_MORE:
                    {
                        switch (state_stack.top()) {
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 4:
                                {
                                    state_stack.push(19);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 12:
                                {
                                    state_stack.push(31);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    state_stack.push(19);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    state_stack.push(31);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 36:
                                {
                                    state_stack.push(19);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    state_stack.push(31);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::REGION:
                    {
                        switch (state_stack.top()) {
                            case 26:
                                {
                                    state_stack.push(41);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 40:
                                {
                                    state_stack.push(41);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 49:
                                {
                                    state_stack.push(41);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                        }
                    }
                case lexer_token::ANY:
                    {
                        switch (state_stack.top()) {
                            case 0:
                                {
                                    state_stack.push(2);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 1:
                                {
                                    state_stack.push(10);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 4:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 5:
                                {
                                    state_stack.push(2);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 9:
                                {
                                    state_stack.push(10);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 12:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 13:
                                {
                                    state_stack.push(10);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 17:
                                {
                                    state_stack.push(2);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 18:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 19:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 20:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 29:
                                {
                                    state_stack.push(10);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 30:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 31:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 32:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 36:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::ZERO_ONE:
                    {
                        switch (state_stack.top()) {
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 4:
                                {
                                    state_stack.push(20);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 12:
                                {
                                    state_stack.push(32);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    state_stack.push(20);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    state_stack.push(32);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 36:
                                {
                                    state_stack.push(20);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    state_stack.push(32);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::LEFT_B:
                    {
                        switch (state_stack.top()) {
                            case 0:
                                {
                                    state_stack.push(7);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 1:
                                {
                                    state_stack.push(15);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 4:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 5:
                                {
                                    state_stack.push(7);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 9:
                                {
                                    state_stack.push(15);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 12:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 13:
                                {
                                    state_stack.push(15);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 17:
                                {
                                    state_stack.push(7);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 18:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 19:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 20:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 29:
                                {
                                    state_stack.push(15);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 30:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 31:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 32:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 36:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::RIGHT_B:
                    {
                        switch (state_stack.top()) {
                            case 23:
                                {
                                    state_stack.push(38);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 24:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(5,
                                            reduction(13, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 26:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(6,
                                            reduction(15, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 35:
                                {
                                    state_stack.push(46);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 39:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(5,
                                            reduction(14, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 40:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(6,
                                            reduction(15, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 42:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(6,
                                            reduction(17, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 48:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 4; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(6,
                                            reduction(16, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 49:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(6,
                                            reduction(15, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
                case lexer_token::NOT:
                    {
                        switch (state_stack.top()) {
                            case 7:
                                {
                                    state_stack.push(25);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 15:
                                {
                                    state_stack.push(25);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                        }
                    }
                case lexer_token::CHAR:
                    {
                        switch (state_stack.top()) {
                            case 0:
                                {
                                    state_stack.push(8);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 1:
                                {
                                    state_stack.push(16);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 4:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 5:
                                {
                                    state_stack.push(8);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 7:
                                {
                                    state_stack.push(26);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 9:
                                {
                                    state_stack.push(16);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 12:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 13:
                                {
                                    state_stack.push(16);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 15:
                                {
                                    state_stack.push(26);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 17:
                                {
                                    state_stack.push(8);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 18:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 19:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 20:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 25:
                                {
                                    state_stack.push(40);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 26:
                                {
                                    state_stack.push(40);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 29:
                                {
                                    state_stack.push(16);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 30:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 31:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 32:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 36:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 40:
                                {
                                    state_stack.push(40);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 41:
                                {
                                    state_stack.push(47);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 47:
                                {
                                    state_stack.push(49);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 49:
                                {
                                    state_stack.push(40);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                        }
                    }
                case lexer_token::SPLIT:
                    {
                        switch (state_stack.top()) {
                            case 2:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 3:
                                {
                                    state_stack.push(17);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 4:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 5:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(5, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 6:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(7, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 8:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 10:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(9, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 11:
                                {
                                    state_stack.push(29);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 12:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 13:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(5, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 14:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(7, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 16:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(11, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 18:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 19:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 20:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 21:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 22:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(6, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 27:
                                {
                                    state_stack.push(29);
                                    props_stack.push(std::move(current));
                                    generator.consume();
                                    break;
                                }
                            case 28:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 30:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(4, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 31:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(3, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 32:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(2, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 33:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 34:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 2; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(2,
                                            reduction(6, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 36:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 37:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(8, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 38:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 43:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(12, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 44:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 1; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(3,
                                            reduction(1, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 45:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(1,
                                            reduction(8, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                            case 46:
                                {
                                    std::list<Props> tmp;
                                    for (auto i = 0; i < 3; ++i) {
                                        auto &item = props_stack.top();
                                        tmp.push_front(std::move(item));
                                        state_stack.pop();
                                        props_stack.pop();
                                    }
                                    generator.push(4,
                                            reduction(10, std::vector<Props>(std::move_iterator<iter_t>(tmp.begin()),
                                                    std::move_iterator<iter_t>(tmp.end()))));
                                    break;
                                }
                        }
                    }
            }
        }
        return lexer_graph();
    }
}
