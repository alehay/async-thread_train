//infiniteDataStream.cpp
#include <coroutine>
#include <memory>
#include <iostream>

template <typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    //Создание генератора
    Generator(handle_type h) : coro(h) {}                       // (3)
    
    
    handle_type coro;
    
    std::shared_ptr<T> value;
    ~Generator() {
        if (coro) {
            coro.destroy();
        }
    }
    Generator(const Generator &) = delete;
    Generator& operator=(const Generator &) = delete;
    Generator(Generator &&other) : coro(other.coro) {
        other.coro = nullptr;
    }
    Generator& operator=(Generator &&other) {
        coro = other.coro;
        other.coro = nullptr;
        return *this;
    }
    T getValue() {
        return coro.promise().current_value;
    }
    // Запрос следующего значения и возврат флага, если генератор исчерпал себя
    bool next() {                                               // (5)
        coro.resume();
        return not coro.done();
    }

    //  Создание promise объекта
    struct promise_type {
        promise_type() = default;                               // (1)
        ~promise_type() = default;
        //Вызов promise.initial_suspend(), т.к. генератор "ленивый", следовательно, suspend_always
        auto initial_suspend() {                                // (4)
            return std::suspend_always{};
        }
        auto final_suspend()  noexcept {
            return std::suspend_always{};
        }
        // Вызов promise.get_return_object() и сохранение результата в локальной переменной
        auto get_return_object() {                              // (2)
            return Generator{handle_type::from_promise(*this)};
        }
        auto return_void() {
            return std::suspend_never{};
        }
        // Действие на co_yield, после чего будет доступно следующее значение
        auto yield_value(T value) {                             // (6)
            current_value = value;
            return std::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
        T current_value;
    };
};
Generator <int> getNext(int start = 0, int step = 1) {
    auto value = start;
    for (int i = 0; ; ++i) {
        co_yield value;
        value += step;
    }
}
int main(int argc , char* argv[]) {
    std::cout << "getNext():";
    auto gen = getNext();
    for (int i = 0; i <= 10; ++i) {
        gen.next();
        // Получение следующего значения
        std::cout << " " << gen.getValue();                     // (7)
    }
    std::cout << "\ngetNext(100, -10):";
    auto gen2 = getNext(100, -10);
    for (int i = 0; i <= 20; ++i) {
        gen2.next();
        std::cout << " " << gen2.getValue();
    }
    std::cout << std::endl;
}