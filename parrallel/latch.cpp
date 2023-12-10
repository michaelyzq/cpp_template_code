
#include <chrono>
#include <format>
#include <iostream>
#include <latch>
#include <random>
#include <string_view>
#include <thread>
#include <vector>

void test_latch()
{

	std::random_device rd;
	std::default_random_engine engine {rd()};
	std::uniform_int_distribution ints{2000, 18000};



	std::latch mainLatch{3};


	auto task{
			[&](std::string_view name, std::chrono::milliseconds workTime) {
					std::cout << std::format("Processing with {} work for {} ms \n", name, workTime.count());

					std::this_thread::sleep_for(workTime);

					std::cout << std::format("{}  done; signals mainLatch \n", name);
					mainLatch.count_down();
			}
	};

	std::vector<std::jthread> threads;

	std::cout << "Main starting three jthread. \n";

	for (int i{ 1 }; i < 4; ++i) {
		threads.push_back(std::jthread(task,
			std::format("Task {}", i),
			std::chrono::milliseconds{ints(engine)}));
	}

	std::cout << "\n Main waiting for jthreads to reach the latch \n";
	mainLatch.wait();

	std::cout << "\n  All jthreads reached the latch. Main working \n";
	std::cout << "Showing that mainLatch is permantly open \n";

	mainLatch.wait();

	std::cout << "mainLatch is already open. Main continue \n";



};

//int main() {
//	test_latch();
//}