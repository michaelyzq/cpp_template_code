
#include <string>
#include <format>
#include <cstdio>
#include <iostream>

namespace stdr = std::ranges;
template<typename ...Args>
void print(const std::string_view fmt_str, Args&&...args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

template<class T>
class Seq {
	T _start{};
	T _end{};

public:
	Seq(T start, T end) :_start(start), _end(end) {}
	struct iterator {
		T value{};

		using value_type = std::remove_cv_t<T>;
		using diffrence_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;


		explicit iterator(T v=0) :value(v) {}
		iterator& operator++()
		{
			value++;
			return*this;
		}

		iterator operator++(int) {
			auto t{ *this };
			++*this;
			return t;
		}

		T operator*() const
		{
			return value;
		}

		bool operator !=(const iterator& l) {
			return this->value != l.value;
		}
	};


	iterator begin() const
	{
		return iterator{ _start };
	}


	iterator end() const
	{
		return iterator{ _end };
	}


};


template<class T>
void printc(const Seq<T>& r)
{

	for (const auto& i : r) {
		print("{} \n", i);
	}
	print("\n");

}

int main() {
	Seq<int>r{1, 10};
	
	{
		for (auto i : r) {
			std::cout << i << std::endl;
		}
	}
	{
		for (auto i = r.begin(); i != r.end(); ++i) {
			std::cout << *i << std::endl;
		}
	}
	printc(r);
}
