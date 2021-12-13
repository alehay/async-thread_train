#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <coroutine>

// класс генератоа 
template<class T> struct generator
{
    // класс итератора . 
    class iterator;

    // 
    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;

    generator(coro_handle handler)
        : handle(handler)
    {   
        
    }

    //  Метод передает управление сопрограмме
    bool resume()
    {
        if(!handle.done())handle.resume();
        return !handle.done();
    }

    // провека на готовность
    bool ready()
    {
        return handle.done();
    }

    ~generator()
    {
        handle.destroy();
    }

    bool next()
    {
        return handle ? (handle.resume(), !handle.done()) : false;
    }
    
    
    T get_value()
    {
        return handle.promise().current_value;
    }

    T operator()()
    {
        if(next())
        {
            return get_value();
        }
        return T();
    }

    iterator begin()
    {
        return iterator(this);
    }
    iterator end()
    {
        return iterator(this);
    }


    class iterator
    {
    public:
        iterator(generator<T>* ptr)
        {
            this->ptr = ptr;
        }
        iterator& operator++()
        {
            ptr->next();
        }
        T operator*()
        {
            return this->ptr->get_value();
        }
        bool operator!=(const iterator& other)
        {
            return !(ptr->ready());
        }
    private:
        generator<T>* ptr;
    };

    struct promise_type
    {
        T current_value;
        auto yield_value(T value)
        {
            current_value = value;
            return std::suspend_always{};
        }
        auto get_return_object()
        {
            return coro_handle::from_promise(*this);
        }
        auto initial_suspend()noexcept
        {
            return std::suspend_never{};
        }
        auto final_suspend()noexcept
        {
            return std::suspend_always{};
        }
        void return_void(){}
        void unhandled_exception()
        {

        }
    };

private:
    //
    coro_handle handle;    
};

#endif // __GENERATOR_H__

