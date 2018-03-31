#ifndef UTILITY_HPP
#define UTILITY_HPP

#include<memory>

namespace parser {
    template<typename T>
        struct deleter{
            bool _delete;
            deleter(bool to_delete = true) : _delete(to_delete) {}
            void operator()(T *ptr) {
                if (_delete) delete ptr;
            }
        };

    template<typename T>
        using unique_ptr = std::unique_ptr<T, deleter<T>>;

    template<typename T>
        auto unique(T *ptr, bool to_delete = true) {
            return unique_ptr<T>(ptr, deleter<T>(to_delete));
        }

    template<typename T, typename ... Args>
        auto make_unique(Args &&... args) {
            return unique(new T(std::forward<Args>(args)...));
        }
}

#endif
