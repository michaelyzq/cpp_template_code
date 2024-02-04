#include <iostream>
#include <memory>
#include <coroutine>


class coroutine_result{
    public:
    int value()  const{

        if(auto handle =shared_state_->handle){
            handle.resume();
        }
        return shared_state_->value;
    };
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    private:
    coroutine_result(handle_type handle):
        shared_state_{std::make_shared<state>()}
        {
            shared_state_->handle = handle;
        }

    struct state{
        int value =0;
        handle_type handle;
    };

    std::shared_ptr<state> shared_state_;
};

struct coroutine_result::promise_type{
    std::weak_ptr<state> shared_state_;
    auto get_return_object(){
        std::cout<<"promise type: get return object()"<<std::endl;

        auto handle = handle_type::from_promise(*this);

        auto return_object = coroutine_result{handle};
        shared_state_ = return_object.shared_state_;
        return return_object;
    }

    auto initial_suspend(){
        std::cout<<"promise type:: initial suspend()"<<std::endl;
        return std::suspend_never{};
    }

    auto final_suspend() noexcept{
        std::cout<<"promise type ::final suspend()"<<std::endl;
        return std::suspend_never{};
    }

    auto return_value(int value){
        std::cout<<"promise type: return value()"<<std::endl;

        if(auto state = shared_state_.lock())
            state->value = value;
    }

    auto unhandled_exception(){
        std::cout<<"promise type:: unhandled exception"<<std::endl;
        std::terminate();
    }
};


coroutine_result my_core() {

    std::cout<< "In corontinue"<<std::endl;
    std::cout<< "suspending.."<<std::endl;
    co_await std::suspend_always{};
    std::cout<<"The corotine has resume"<<std::endl;
    co_return 42;
}

int main()
{
    std::cout<<"Calling the corouting ..."<<std::endl;
    auto result = my_core();
    std::cout<<"Corontine has returned ..."<< result.value()<<std::endl;

}