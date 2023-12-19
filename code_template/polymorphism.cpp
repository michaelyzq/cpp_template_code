#include <iomanip>
#include <iostream>
#include <chrono>
#include <ranges>
#include <string>
#include <deque>
#include <list>
#include <vector>
#include <iostream>
#include <functional>
#include <format>
#include "Print.h"
using namespace std;



int main() {
	deque<int> d;
	list<int> l;
	vector<int> v;

	auto print_c = [](const auto& containter){
		for (const auto& i : containter) print("{} ", i);
		print ("\n");
	};


	auto pushc = [](auto& container) {
		return [&container](auto value) {
			container.push_back(value);
		};
	};


	const vector<std::function<void(int)>> consumer{pushc(d),
		pushc(l), pushc(v)};


	consumer[0](10);
	consumer[0](29);

	consumer[1](30);
	consumer[1](20);

	consumer[2](1);
	consumer[2](2);


	print_c(d);
	print_c(l);
	print_c(v);

}
