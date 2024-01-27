#pragma once

#include "executor.h"

#include <future>
#include <type_traits>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>


template<typename F>
concept NullaryFunction = std::is_invocable<F>::value;

template<typename F, typename ArgumentType>
concept UnaryFunction = std::is_invocable<F, ArgumentType>::value;

template<typename T>
concept SupportsPromise = 
    std::is_copy_constructible<T>::value||
    std::is_move_constructible<T>::value ||
    std::is_same<T, void>::value ||
    std::is_reference<T>::value;


template<SupportsPromise result>
class task: public executable{
    public:
    using result_t = result;

    template<NullaryFunction fn_t>
    explicit task(fn_t &&fn):fn_{std::forward<fn_t>(fn)}{

    }

    [[nodiscard]]
    auto get_future(){
        std::cout<<"getting fugure"<<std::endl;
        return promise_.get_future();
    }

    private:
        void execute()noexcept override{
            try{
                execute_impl();

            }catch(...){

            }
        }

        void execute_impl()
        {
            if constexpr (std::is_same<result_t, void>::value)
            {
                fn_();
                promise_.set_value();
            }
            {
                promise_.set_value(fn_());
            }
        }

        std::function<result_t(void)> fn_;
        std::promise<result_t> promise_;
};

template<typename result>
using task_ptr=std::shared_ptr<task<result>>;

template<NullaryFunction function_type>
inline auto run_task(executor &ex, function_type&&fn){
    auto t = std::make_shared<task<decltype(fn())>>(std::forward<function_type>(fn));
    ex.schedule(t);
    return t;
}


