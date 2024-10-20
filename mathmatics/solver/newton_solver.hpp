#include <functional>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <boost/math/tools/roots.hpp>
#include <tuple>
#include <boost/math/tools/minima.hpp>

#include "solvers.hpp"
#include "pricer.hpp"

template <typename T>
using FunctionType = std::function<T(const T &t)>;

template <typename TOL>
std::pair<double, double> TestToms748(const FunctionType<double> &func, double a, double b, const TOL &tol,
                                     boost::uintmax_t& max_iter)
{

   std::pair<double, double> result = boost::math::tools::toms748_solve(func, a, b, tol, max_iter);
	
	return result;
}



template<typename T>
class NRFunctions
{
    OptionData opt;
    T mktV;

public:
    NRFunctions(const OptionData& option, T mktValue):opt(option), mktV(mktValue)
    {

    }

    std::tuple<T,T> operator() (const T&x)
    {

        Pricer option(opt);

        auto callPricer = [&](double x){
            return -mktV +option.CallPrice(x);
        };

        auto vega = std::bind(&Pricer::Vega, option, std::placeholders::_1);
        return std::make_tuple(callPricer(x), vega(x));

    }


};


template <typename T>
class HalleyFunctions{
private:
    OptionData opt;
    T mktV;

public:
    HalleyFunctions(const OptionData& option, T mktValue): opt(option), mktV(mktValue){

    }

    std::tuple<T, T, T> operator()(const T& x){

        Pricer option(opt);

        auto callPricer = [&](double x){
            return -mktV + option.CallPrice(x);
        };

        auto vega = std::bind(&Pricer::Vega, option, std::placeholders::_1);
        auto dVegadVol = std::bind(&Pricer::DVegaDVol, option, std::placeholders::_1);

        return std::make_tuple(callPricer(x), vega(x), dVegadVol(x));

    }

};

template <typename T>
std::tuple<T, boost::uintmax_t>
NewtonRaphson(const T& guess, const T&L, const T&R, 
                const OptionData&opt, T mktValue, int digits)
{
    using namespace boost::math::tools;

    boost::uintmax_t maxIter = 10;

    NRFunctions<T> functions(opt, mktValue);

    T val = boost::math::tools::newton_raphson_iterate(functions, guess, L, R, digits, maxIter);
    return std::make_tuple(val, maxIter);

}

template <typename T>
std::tuple<T, boost::uintmax_t> Halley(const T&guess, const T&L, const T& R,
            const OptionData&opt, T mktValue, int digits)
{
    using namespace boost::math::tools;

    boost::uintmax_t maxIter = 10;

    HalleyFunctions<T> functions(opt, mktValue);

    T val = boost::math::tools::halley_iterate(functions, guess, L, R, digits, maxIter);
    return std::make_tuple(val, maxIter);

}

struct TerminationCondition
{

    bool operator() (double min, double max){
        return std::abs(min-max)<=0.00001;
    }
};







