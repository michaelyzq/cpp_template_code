#include <atomic>
#include <format>
#include <iostream>
#include <thread>


void test_atomic()
{
	int count1{ 0 };
	std::atomic<int> atomicCount{0};

	int count2{ 0 };

	std::atomic_ref<int> atomicRefCount{count2};

	{
		std::cout << "Two concurent threads incrementing int count1"
			<< "automic Count and atomicRefCount \n\n";

		auto incrementer{
				[&]() { for (int i{0}; i < 10000; ++i) {
								++count1;
								++atomicRefCount;
								++atomicCount;
								std::this_thread::yield();
								}
						}
		};

		std::jthread t1(incrementer);
		std::jthread t2(incrementer);
	}

	std::cout << std::format("Final count 1 {} \n", count1);
	std::cout << std::format("Final atumicCount {} \n", count2);
	std::cout << std::format("Final count2 {} \n", count2);
	system("pause");


};


//int main() {
//	test_atomic()
//}