#pragma once

#include <random>
#include <functional>
#include <tuple>

template <typename T>
    using IFunctionType = std::function<T (T)>;


template <typename T>
    using ITerminationType = std::function<bool (T a, T b)>;


template <typename T>
    using ISolverType = std::function<void (T&a, T&b, T&fa, T&fb)>;

template <typename T>
    using IMISType = std::function<void (T a, T b, T fa, T fb)>;


template <typename T>
    using IReportType = std::function<void (T a, T b, T fa, T fb)>;













