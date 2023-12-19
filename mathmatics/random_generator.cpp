#include <array>
#include <vector>
#include "Print.h"
#include <random>
#include <array>
#include <map>
#include <string>
using namespace std;

int iround(const double& d) {
	return static_cast<int>(std::round(d));
}

int main() {
	constexpr size_t  n_data{ 10000 };
	constexpr size_t  n_samples{ 300 };

	constexpr int mean{ 0 };
	constexpr size_t dev{ 5 };

	std::random_device rd;
	std::mt19937 rng{rd() };

	std::normal_distribution<>dist{mean, dev};
	std::array<int, n_data> data{};

	for (auto& e : data) e = iround(dist(rng));

	std::array<int, n_samples> samples{};

	std::sample(data.begin(), data.end(), samples.begin(), n_samples, rng);

	std::map<int, size_t> hist{};
	for (const int i : samples)++hist[i];

	constexpr size_t scale{ 3 };

	print("{:>3} {:>5} {:<} / {} \n", "n", "count","graph", "sacle");

	for (const auto& [value, count] : hist) {
		print("{:>3} ({:>3})  {} \n", value, count, std::string(count, '*'));
	}




}
