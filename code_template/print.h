#include <string_view>
#include <string>
#include <format>
#include <cstdio>
#include <iostream>


template<typename ...Args>
void print(const std::string_view fmt_str, Args&&...args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

void printc(const auto& v, const std::string_view s = "") {
	if (s.size()) std::cout << std::format("{}: ", s);
	for (auto e : v) std::cout << std::format("{} ", e);
	std::cout << "\n";
}