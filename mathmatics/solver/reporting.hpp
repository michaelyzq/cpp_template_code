#pragma once

#include <functional>
#include <tuple>
#include <cmath>
#include <iostream>

#include <boost/multiprecision/cpp_dec_float.hpp>

template <typename T>
class ReportingDefault
{

private:

public:
    ReportingDefault() {}

    void operator()(T a, T b, T fa, T fb)
    {
		std::cout << "\n** Final report ** \n";
		std::cout << std::setprecision(8) << "Bracket + avg Value:  [" << a << "," << b << "] " << (a+b)/2 << '\n';
		std::cout << std::setprecision(8) << "Averaged function value (~ 0?): " << 0.5*(fa + fb) <<  "\n";
	}
};

