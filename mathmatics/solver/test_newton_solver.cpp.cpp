#include <cmath>
#include <cfloat>
#include <iostream>
#include <functional>
#include <chrono>
#include <tuple>
#include <boost/math/tools/minima.hpp>
#include "newton_solver.hpp"

#include "interfaces.hpp"
#include "print_utils.hpp"
#include "solvers.hpp"
#include "mis.hpp"
#include "mediator.hpp"
#include "pricer.hpp"
#include "termination.hpp"
#include "reporting.hpp"



using value_type = double;
//int main()
int mainbak()
{

	auto terminate = [](double min, double max){
		return std::abs(min-max)<=1.0e-3;
	};


	OptionData opt;
	opt.K  = 165.0;
	opt.T = 0.25;
	opt.r = 0.08;
	opt.b =0.0;
	opt.sig = 0.03;
	opt.S = 60.0;

	Pricer option(opt);

	double mktValue = option.CallPrice(opt.sig);

	auto callPricer = [&](double x){
		return -mktValue+option.CallPrice(x);
	};

	auto callPricerLS = [&](double x){
		return callPricer(x)*callPricer(x);
	};

	auto callPricerFP = [&] (double x){
		return x - callPricer(x);
	};

	double vL = 0.001;
	double vH = 4.0;
	
	OptionData optL = { opt.K,1,0,0,vL,100 }; // K,T,r,b,sig,S
	double cL = callPricer(optL.sig);

//	OptionData optH = { 65,0.25,0.08,0.08,vH,60 }; // K,T,r,b,sig,S
	OptionData optH = { opt.K,1,0,0,vH,100 }; // K,T,r,b,sig,S
	double cH = callPricer(optH.sig);

	boost::uintmax_t max_iter =100;

	std::cout << "vL, vH, cL, cH: " << vL << "," << vH << ", " << cL << ", " << cH << '\n';
	std::cout << "# iters needed: " << (std::log(vH - vL) - std::log(1.0e-1)) / std::log(2.0) << '\n';

	int DIGITS = 128;	// Desired number of binary digits
		double leftBracket = 0.001395; double rightBracket = 4.0;
		double root = 0.03000003139099;
		auto resultNR = NewtonRaphson<double>(root, leftBracket, rightBracket, opt, mktValue, DIGITS);
		std::cout << "NR results: " << std::setprecision(12)
			<< std::get<0>(resultNR) << ", " << std::get<1>(resultNR) << '\n';

		auto resultHalley = Halley<double>(root, leftBracket, rightBracket, opt, mktValue, DIGITS);
		std::cout << "Halley results: " << std::setprecision(12)
			<< std::get<0>(resultNR) << ", " << std::get<1>(resultNR) << '\n';

	

}

