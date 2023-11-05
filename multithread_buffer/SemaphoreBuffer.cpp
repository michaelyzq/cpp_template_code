
#include <format>
#include <iostream>
#include <semaphore>
#include <string>
#include <chrono>
#include <format>
#include <random>
#include <thread>

using namespace std::string_literals;

class SynchronizedBuffer {

public:
	void put(int value) {

		m_produce.acquire();

		m_buffer = value;
		m_occupied = true;

		std::cout << std::format("{:<40} {} \t\t {} \n",
			"Producer writes "s + std::to_string(value),
			m_buffer, m_occupied);

		m_consume.release();
	}

	int get() {
		int value;
		m_consume.acquire();

		value = m_buffer;
		m_occupied = false;


		std::cout << std::format("{:<40} {} \t\t {} \n",
			"Consumer reads"s + std::to_string(m_buffer),
			m_buffer, m_occupied);

		m_produce.release();
		return value;


	}
private:
	std::binary_semaphore m_produce{ 1 };
	std::binary_semaphore m_consume { 0 };

	bool m_occupied{ false };
	int m_buffer{ -1 };// shared by produer and consumer threads
};


void testSemaphoreBuffer()
{
	std::random_device rd;
	std::default_random_engine engine {rd()};
	std::uniform_int_distribution ints{0, 3000};

	std::mutex intsMutex;

	auto getSleepTime{
		[&]() {
			std::lock_guard lock{intsMutex};
			return std::chrono::milliseconds{ints(engine)};
		}
	};

	SynchronizedBuffer buffer;


	auto produce{
		[&buffer, &getSleepTime]() {
			int sum{0};

			for (int count{ 1 }; count <= 10; ++count) {
				//get random sleep time than sleep

				std::this_thread::sleep_for(getSleepTime());
				buffer.put(count);
				sum += count;

			}

			std::cout << "Procuder done producing \n Terminating Producer \n";
		}

	};

	auto consume{
		[&buffer, &getSleepTime]() {
			int sum{0};

			for (int count{ 1 }; count <= 10; ++count) {
				std::this_thread::sleep_for(getSleepTime());

				sum += buffer.get();
			}
			std::cout << std::format("\n {} {} \n {} \n",
				"Consumer read values totaling", sum, "Terminating Consumer"
			);
			}
	};

	std::cout << std::format("{:<40}{}\t\t{}\n{:<40}{}\t\t{}\n\n",
		"Operation", "Buffer", "Occupied",
		"--------", "-----", "--------");

	std::jthread producer {produce};
	std::jthread consumer {consume};

	system("pause");

};

//int main() {
//	testSemaphoreBuffer();
//}