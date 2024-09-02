#pragma once

#include <functional>
#include <tuple>

#include <cmath>
#include <limits>
#include "interfaces.hpp"
#include <format>
#include <cstdint>

#include <boost/math/tools/minima.hpp>

template <typename T>
class ThreePointIntervalSolver
{

private:
    IFunctionType<T> f;

    T h, a1, a2, a3,fa1, fa2, fa3;
    T a, b;


public :


    ThreePointIntervalSolver(const IFunctionType<T>& function, T A, T B)
		: f(function)
	{
	//	std::cout << "3-pt scheme\n";
		// Iniitalisation
		h = 0.25 * std::abs(B-A);
		a = (A < B) ? A : B;
		a1 = a + h;
		a2 = a1 + h;
		a3 = a2 + h;
		b = a3 + h;
		fa1 = f(a1);
		fa2 = f(a2);
		fa3 = f(a3);
	}

    void operator () (T& lower, T& upper, T& funcLower, T& funcUpper){
      funcLower = std::numeric_limits<double>::max();
		funcUpper = std::numeric_limits<double>::max();
		
		if ((fa1 < fa2) && (fa1 < fa3))
		{
			b = a2; a2 = a1; funcUpper = fa2; fa2 = fa1;
		}
		else if ((fa2 < fa1) && (fa2 < fa3))
		{
			a = a1; funcLower = fa1; b = a3; funcUpper = fa3;
		}
		else if ((fa3 < fa1) && (fa3 < fa2))
		{
			a = a2; a2 = a3; funcLower = fa2; fa2 = fa3;
		}

		h *= 0.5;
		a1 = a + h;
		a3 = a2 + h;
		fa1 = f(a1);
		fa3 = f(a3);

		if (funcLower == std::numeric_limits<T>::max()) funcLower = f(a);
		if (funcUpper == std::numeric_limits<T>::max()) funcUpper = f(b);

		lower = a;
		upper = b;

        //std::cout<<std::format("a: {} a1: {} a2: {} a3: {} b: {}  fa1: {} fa2: {} fa3 : {}\n", a, a1, a2, a3, b, fa1, fa2, fa3);

    }

    T value() const{
        return 0.5*(a+b);
    }

};

template <typename T>
class GoldenSectionIntervalSolver

{
private:
    IFunctionType<T> f;
    T lambda, mu, x1, x2, fx1, fx2;
    T a, b;

public:
    GoldenSectionIntervalSolver(const IFunctionType<T>& function, T A, T B):f(function){
        std::cout << "Golden mean scheme\n";

       	lambda = 0.5*(2.236067977499789696 - 1.0);
		mu = 0.5*(3.0 - 2.236067977499789696);

        x1 = B - lambda*(B-A);
        x2 = A + lambda*(B-A);
        fx1 = f(x1);
        fx2 = f(x2);
    }

    void operator()(T&a, T&b, T& fa, T& fb){

        	if (fx1 > fx2) 
			{
				a = x1;
				fa = fx1;
			
				x1 = x2;
				fx1 = fx2;
				x2 = b - mu * (b - a);
				fx2 = f(x2);
			}
			else 
			{
				b = x2;
				fb = fx2;
				
				x2 = x1;
				fx2 = fx1;
				x1 = a + mu * (b - a);
				fx1 = f(x1);
			}

    }

    T value() const{
        return 0.5*(a+b);
    }
};


template <typename T>
class BrentIntervalSolver
{
    IFunctionType<T> f;
    T a, b;
    std::uintmax_t maxIter;

public:
    BrentIntervalSolver(const IFunctionType<T>&function, T A, T B,
                       int maxIerations = 1000):f(function), a(A), b(B), maxIter(maxIerations)
    {

    }

    void operator()(T&a, T&b, T& fa, T&fb){
        int bits = std::numeric_limits<double>::digits;

        std::pair<T, T> result = boost::math::tools::brent_find_minima(f, a, b, bits,maxIter);
        
        a = b = result.first;
        fa = fb=result.second;
    }

    T value() const{
        return 0.5*(a+b);
    }

    int NumberIterations() const{
        return maxIter;
    }

};


#define sqrt5  22.236067977499789696

