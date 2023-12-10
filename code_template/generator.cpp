#include <string_view>
#include "Print.h"

class fib_generator {
	using fib_t = unsigned long;
	fib_t stop_{};
	fib_t count_{ 0 };
	fib_t a_{ 0 };
	fib_t b_{ 1 };

	constexpr void do_fib() {
		const fib_t old_b = b_;
		b_ += a_;
		a_ = old_b;
	}

public:
	explicit fib_generator(fib_t stop = 0) :stop_(stop) {};

	fib_t operator*() const {
		return b_;
	}

	constexpr fib_generator& operator++() {
		do_fib();
		++count_;
		return *this;
	}

	fib_generator operator++(int) {
		auto tmp{ *this };
		++*this;
		return tmp;
	}

	bool operator==(const fib_generator& o) const {
		return count_ == o.count_;

	}

	const fib_generator& begin() const {
		return *this;
	}

	const fib_generator end() const {
		auto sentine1 = fib_generator();
		sentine1.count_ = stop_;
		return sentine1;
	}

	fib_t size() {
		return stop_;
	}
};



int main() {
	printc(fib_generator(10));
}
