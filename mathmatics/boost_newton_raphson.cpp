#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <cmath>
#include <type_traits>
#include <boost/cstdint.hpp>
#include <boost/math/special_functions.hpp>
#include <tuple>
#include <cmath>

// Newton-Raphson, exp(x) = 148.4131591, x = 5
template <typename T> class NRFunctions
{ // Functions needed for input to NR method
private:
 
public:
    NRFunctions()
    { 
    }
 
    std::tuple<T, T> operator()(const T& x)
    { // x is estimate so far.
       // Return the function and its dervivative.
       return std::make_tuple(std::pow(x,2)-3,2*x);
    }
};

template <typename T>
   std::tuple<T, boost::uintmax_t> NewtonRaphson (const T& L, const T& R)
{
   using namespace std; // for frexp, ldexp, numeric_limits.
   using namespace boost::math::tools;
 
   T min = L;;
   T max = R;
   T guess = (L + R)/ 2; // Rough guess 
   int digits = std::numeric_limits<T>::digits;
   boost::uintmax_t maxIter = 100;
   T val = boost::math::tools::newton_raphson_iterate
            (NRFunctions<T>(), guess,min, max, digits, maxIter);
 
   return std::make_tuple(val, maxIter);
}

int main(){
  double leftBracket = 0.0; double rightBracket = 10.0;
auto result = NewtonRaphson(leftBracket, rightBracket);
std::cout << "NR results: " <<  std::setprecision(8)
          << std::get<0>(result) << ", " << std::get<1>(result);
   }