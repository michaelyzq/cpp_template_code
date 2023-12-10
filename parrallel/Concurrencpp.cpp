// cpp20-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//
#include <chrono>
#include "concurrencpp/concurrencpp.h"
#include <format>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

std::string id() {
    std::ostringstream out;
    out << std::this_thread::get_id();
    return out.str();
}


void printTask(std::string name, std::chrono::milliseconds sleep) {
    std::cout << std::format(
        "{} thread ID: {} going to sleep for {} ms\n ",
        name, id(), sleep.count());

    std::this_thread::sleep_for(sleep);
    std::cout << std::format("{} thread ID: {} done sleeping \n",
        name, id());
}

void test_concurren()
{
    concurrencpp::runtime runtime;

    std::cout << std::format("mains's thread ID : {} \n\n", id());

    std::random_device rd;
    std::default_random_engine engine{rd()};
    std::uniform_int_distribution ints{0, 5000};

    std::vector<concurrencpp::result<void>> results;

    std::cout << "Starting Three Concurrencpp Tasks \n";


    for (int i{ 1 }; i < 10; ++i) {
        std::chrono::milliseconds sleepTime {ints(engine)};
        std::string name{std::format("Task {}", i)};

        results.push_back(runtime.thread_pool_executor()->submit(printTask, name, sleepTime));

    }

    std::cout << "\n ALL TASKS STARTED\n";
    std::cout << "\nWAITING FOR TASKS to COMPLETED \n";

    for (auto& result : results) {
        result.get();
    }

    std::cout << std::format("main thread ID {} MAIN END  \n", id());

    system("pause");

}

int main() {
    test_concurren()
}