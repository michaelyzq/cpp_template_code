#pragma once

#include <functional>
#include <tuple>
#include <cmath>
#include <limits>
#include "interfaces.hpp"

template <typename T>
class Mediator
{

private:
    T A, B, fA, fB;
    IFunctionType<T> f;
    ITerminationType<T> stop;
    ISolverType<T> sol;
    IMISType<T> mis;
    IReportType<T> rep;

public:
    Mediator(const IFunctionType<T> func, T a, T b,
             const ITerminationType<T> &stopping,
             const ISolverType<T> &solver,
             const IMISType<T> &statistics,
             const IReportType<T> &report)
        : f(func), stop(stopping), sol(solver), rep(report)
    {
        A = a;
        B = b;
        fA = fB = 0.0;
        mis = statistics;
    }

    void run()
    {
        do
        {
            mis(A, B, fA, fB);
            sol(A, B, fA, fB);
        } while (!stop(A, B));

        rep(A, B, fA, fB);
    }

    
    T value() const{
        return 0.5*(A+B);
    }
};