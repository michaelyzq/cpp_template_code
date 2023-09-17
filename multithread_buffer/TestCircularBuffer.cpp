// CPP20.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <format>
#include <iostream>
#include <random>
#include <thread>
#include "CircularBuffer.h"

void run() {

	std::mutex intsMutex;
	CircularBuffer buffer;
	buffer.displayState("Initial state");


	auto produce{
		[&buffer]() {

			std::random_device rd;
			std::default_random_engine engine{rd()};
			std::uniform_int_distribution ints{0, 6000};
			int sum{ 0 };

			for (int count{ 1 }; count <= 10; ++count) {

				std::chrono::milliseconds sleepTime{ints(engine)};
				std::this_thread::sleep_for(sleepTime);
				//std::cout << std::format("\nbuffer start producing{}", count) << std::endl;
				buffer.put(count);

				sum += count;


			}

			std::cout << std::format("\n{} {} \n{} \n",
				"Procuder done producing", sum, "Terminating Procuder");
	}

	};

	auto consume{
		[&buffer]() {
			int sum{0};
			std::random_device rd;
			std::default_random_engine engine{rd()};
			std::uniform_int_distribution ints{0, 3000};

			for (int count{ 1 }; count <= 10; ++count) {
				std::chrono::milliseconds sleepTime{ints(engine)};
				std::this_thread::sleep_for(sleepTime);

				//std::cout << std::format("\nbuffer start consuming {}", count) << std::endl;
				sum += buffer.get();
			}

			std::cout << std::format("\n{} {} \n{} \n",
				"Consumer read values totaling", sum, "Terminating Consumer");
		}
	};

	//std::cout << std::format("{:<40}{} \t\t{}\n{:<40}{}\t\t{}\n\n",
	//	"Operation", "Buffer", "Occupied",
	//	"---------", "------", "--------");

	//

	std::jthread producer{produce};
	std::jthread consumer{consume};

}
//int main()
//{
//
//	run()
//
//		system("pause");
//
//};
