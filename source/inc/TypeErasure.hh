#ifndef TOOLS_TYPE_ERASURE_HH
#define TOOLS_TYPE_ERASURE_HH 1

#include <any>
#include <initializer_list>
#include <tuple>
#include <vector>

struct AbstractWrapper {
    virtual void run(const std::initializer_list<std::any>& any_args) = 0;
};

template<typename Func, typename... Args>
class Wrapper: AbstractWrapper {
public:
    explicit Wrapper(Func f): func(f) {}
    void run(const std::initializer_list<std::any>& any_args) override {
        args = make_tuple_impl(any_args, std::make_index_sequence<sizeof...(Args)>());
        std::apply(func, args);
    }

private:
    Func func;
    std::tuple<Args...> args;

    template<size_t... Is>
    std::tuple<Args...> make_tuple_impl(
        std::vector<std::any> any_args, std::index_sequence<Is...>) {
        return std::make_tuple(std::any_cast<std::decay<decltype(std::get<Is>(args))>>(
            any_args.at(Is))...);
    }
};

#endif  // TOOLS_TYPE_ERASURE_HH
