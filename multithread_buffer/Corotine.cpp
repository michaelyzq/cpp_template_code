#include "concurrencpp/concurrencpp.h"
#include <format>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

std::string id() {
    std::ostringstream out;
    out << std::this_thread::get_id();
    return out.str();
}


concurrencpp::result<void> sortCorotine(
    std::shared_ptr<concurrencpp::thread_pool_executor> executor,
    std::vector<int>& values
) {
    std::cout << std::format("Thread {} : sortCoroutine started \n\n", id());

    auto sortTask{
        [&](auto begin, auto end) {
            std::cout << std::format("Thread {} Sorting {} elements \n", id(), end - begin);
            std::sort(begin, end);
            std::cout << std::format("Thread {} :Finished sorting \n", id());
    }
    };

    std::vector<concurrencpp::result<void>> results;

    size_t middle{ values.size() / 2 };

    std::cout << std::format(
        "Thread {} : sortCoroutine starting first halfTask\n", id());

    results.push_back(
        executor->submit(
            [&]() { sortTask(values.begin(), values.begin() + middle); }
        )
    );

    std::cout << std::format(
        "Thread {} : sortCoroutine starting second halfTask\n", id());

    results.push_back(
        executor->submit(
            [&]() { sortTask(values.begin() + middle, values.end()); }
        )
    );


    std::cout << std::format("\nThread{} {}\n ", id(),
        "sortCouroutine co_awaiting sortTask completion");

    co_await concurrencpp::when_all(
        executor, results.begin(), results.end());

    std::cout << std::format("\Thread{} {}\n", id(),
        "sortCouroutine merging result\n");


    std:inplace_merge(
        values.begin(), values.begin() + middle, values.end());

    std::cout << std::format("Thread {} : sortCoourtine done", id());
    co_return;

}

int main()
{
    concurrencpp::runtime runtime;

    auto executor{ runtime.thread_pool_executor() };



    std::random_device rd;
    std::default_random_engine engine{rd()};
    std::uniform_int_distribution ints;

    std::cout << std::format("Thread {} : main creating of random ints \n", id());
    std::vector<int> values(1'000'000);
    std::ranges::generate(values, [&]() {return ints(engine); });

    std::cout << std::format("Thread {} main starting sortCoroutine \n", id());

    auto result{ sortCorotine(executor, values) };

    std::cout << std::format("\nThread {} : {} \n", id(),
        "main resumed, waiting for sortcoroutine to complete");


    result.get();

    std::cout << std::format("\nThread {} : main confirminig that vector is sorted \n", id());

    bool sorted{ std::ranges::is_sorted(values) };

    std::cout << std::format("Thread {} values is {} sorted \n",
        id(), sorted ? "" : " not");



    system("pause");

}
