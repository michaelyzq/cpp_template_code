#include  "tasks.h"
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <Vector>
#include <future>

using namespace std::chrono_literals;
int main(){
    using namespace std;
    vector<double> daily_price = {100.3, 101.5, 99.2, 105.1, 101.93, 96.7, 97.6,
                                    103.9, 105.8, 101.2};

    constexpr auto parallelism_level = 4;
    executor ex{parallelism_level};

    auto future = run_task(ex, [&daily_price](){{
        auto average = 0.0;
        for(auto p:daily_price)
            average+=p;
        average /=daily_price.size();
        return average;
    }})->get_future();


    std::cout<<"Executing ....."<<std::endl;
    std::cout<<"Average:"<<future.get()<<std::endl;
    cout<<"Finshed "<<std::endl;


}