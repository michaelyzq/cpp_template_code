// MIS.hpp
//
// Functions to display the final result.
//
// (C) Datasim Education BV 2017
//

#ifndef MIS_HPP
#define MIS_HPP

#include <functional>
#include <tuple>
#include <cmath>
#include <iostream>
#include <iomanip>

template <typename T>
	class MISDefault
{
private:

public:
	MISDefault() {}

	void operator () (T a, T b, T fa, T fb)
	{
		std::cout << std::setprecision(12) <<  "***Stats [A,B]  [" << a << "," << b << "]\n";
		std::cout << std::setprecision(12) << "***Stats [fA,fB] [" << fa << "," << fb << "]\n";
	}

};



#endif


