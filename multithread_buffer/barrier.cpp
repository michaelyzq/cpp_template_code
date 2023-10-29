
#include <barrier>
#include <chrono>
#include <format>
#include <iostream>
#include <random>
#include <string_view>
#include <thread>



void test_barrier()
{
	auto moveCarIntoPosition
	{
		[]() {
			std::cout << "Moving next car into paiting positions \n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "Car ready for paiting \n\n";
		}
	};

	int carsToPaint{ 5 };

	//stop_token used to notify robot assembly line is shutting down
	std::stop_source assemblyLineStopSource;

	//stop token used to paintingRoboTask to determine when to shut donw
	std::stop_token stopToken{assemblyLineStopSource.get_token()};


	//assembly line waits for two painting robots to reach this barrier
	std::barrier  paintingDone{ 3,
		[&]() noexcept {
			static int count{ 0 };
			std::cout << "Painting robots completes tasks \n\n";

			if (++count == carsToPaint) {
				std::cout << "Shutting down assembly line \n\n";
				assemblyLineStopSource.request_stop();
			}
			else {
				moveCarIntoPosition();
			}

	} };

	auto painingRobotTask{
		[&](std::string_view name) {
			//set up random-number generation
			std::random_device rd;
			std::default_random_engine engine{rd()};
			std::uniform_int_distribution ints{2500, 5000};

			//check whether assembly line is shutting down,
			//and if not, do the painting work

			while (!stopToken.stop_requested()) {
				auto workTime{ std::chrono::milliseconds{ints(engine)} };

				std::cout << std::format("{} painting for {} ms \n",
					name, workTime.count());
				std::this_thread::sleep_for(workTime);

				std::cout << std::format(
					"{} doen painting. Waiting for next car \n", name
				);

				paintingDone.arrive_and_wait();


			}
			std::cout << std::format("{} shutdown.\n", name);

		}
	};

	std::cout << "Staring robots \n\n";
	std::jthread leftSideRobot(painingRobotTask, "Left side Robot");
	std::jthread rightSideRobot(painingRobotTask, "Right side Robot");
	std::jthread middleSideRobot(painingRobotTask, "Middle side Robot");

};


int main() {
	test_barrier();
 }