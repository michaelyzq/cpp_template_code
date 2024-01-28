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
    explicit task(executor &ex, fn_t &&fn)
    :fn_{std::forward<fn_t>(fn)}
    ,executor_(ex)
    {
    }

    [[nodiscard]]
    auto get_future(){
        std::cout<<"getting future \n"<<std::endl;
        return promise_.get_future();
    }

    template<UnaryFunction<result> function_type>
    auto then(const function_type &what){
        using r = typename std::invoke_result<function_type, result_t>::type;
        auto tsk = std::make_shared<task<r>>(
            this->executor_,
            [what, parent{shared_from_this()}]()mutable{
                auto future = static_cast<task<result_t>&>(*parent).get_future();
                return what(future.get());
            }
        );
        {
            std::scoped_lock lock{mutex_};
            if(! has_finished){
                next_ = tsk;
                return tsk;
            }
        }

        executor_.schedule(tsk);
        next_ = tsk;
        return tsk;
    }

    private:
        void execute()noexcept override{
            try{
                execute_impl();

            }catch(...){

            }

            std::scoped_lock lock_;
            has_finished = true;
            if (next_)
                next_->execute();
        }

        void execute_impl()
        {
            if constexpr (std::is_same<result_t, void>::value)
            {
                fn_();
                promise_.set_value();
            }
            else
            {
                promise_.set_value(fn_());
            }
        }

        std::function<result_t(void)> fn_;
        std::promise<result_t> promise_;

        executable_ptr next_;
        std::mutex mutex_;
        

        executor &executor_;
        bool has_finished = false;
};

template<typename result>
using task_ptr=std::shared_ptr<task<result>>;

template<NullaryFunction function_type>
inline auto run_task(executor &ex, function_type&&fn){
    auto t = std::make_shared<task<decltype(fn())>>(ex, std::forward<function_type>(fn));
    ex.schedule(t);
    return t;
}


