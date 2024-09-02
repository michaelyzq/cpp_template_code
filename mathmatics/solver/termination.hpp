#pragma once

#include <functional>
#include <tuple>
#include <cmath>

template <typename T>
class TerminatorI
{
private:
    T tol;

public:
    TerminatorI(): tol(1.0e-6){}
    Terminator(T tolerance)：tol(tolerance){}

    bool operator()(T a, T b){
        T m = 0.5*std::abs(a+b);
        if (m<=1.0){
            if (std::abs(b-a)<tol){
                return true;
            }
            return false;
        }

        if (std::abs(b-a) <tol*m){
            return true;
        }
        return false;
    }

};