#include <cmath>
#include <cfloat>
#include <iostream>
#include <functional>
#include "interfaces.hpp"
#include "print_utils.hpp"
#include "solvers.hpp"
#include "mis.hpp"
#include "mediator.hpp"
#include <chrono>


double func2(double x)
{
    return (std::log(x) - 5.0)* (std::log(x) - 5.0);
}

int main()
{
    ITerminationType<double> stopping = [](double a , double b){return std::abs(b-a)<0.0001;};
    
    IMISType<double> mis  = [](double a, double b, double fa, double fb){};


    double A = 0.01;
    double B = 10.0;

    auto func = &func2;
    GoldenSectionIntervalSolver<double> solver1(func, A, B);
    ThreePointIntervalSolver<double> solver2(func, A, B);
    BrentIntervalSolver<double> solver3(func, A, B);


    auto report = [&func](double a, double b, double fa, double fb) 
                    {std::cout << std::setprecision(16) << "\n Golden Mean  " << (a + b) / 2<<"\n"; 
                     std::cout<<func((a+b)/2)<<std::endl;
                    };


    auto report2 = [&func](double a, double b, double fa, double fb) 
	{std::cout << std::setprecision(16) << "\n Three point  " << (a + b) / 2 << '\n'; 
      std::cout<<func((a+b)/2)<<std::endl;
    };


    ITerminationType<double> stopping3 = [](double a, double b) { return true; };

    auto report3 = [&func](double a, double b, double fa, double fb) 
    {
    std::cout << std::setprecision(16) << "\n  Brent Interval \n " << (a + b) / 2<< '\n'; 
     std::cout<<func((a+b)/2)<<std::endl;
    };
    Mediator<double> med1(func, A, B, stopping, solver1, mis, report);
    Mediator<double> med2(func, A, B, stopping, solver2, mis, report2);
	Mediator<double> med3(func, A, B, stopping3, solver3, mis, report3);


    {
        auto startTime{std::chrono::steady_clock::now()};
        med1.run();
        auto endTime{ std::chrono::steady_clock::now() };
	    auto time{ duration_cast<std::chrono::milliseconds>(endTime - startTime) };
         std::cout<<std::format("process time for  Golden Section Solver {}\n", time);

    }

    {
        auto startTime{std::chrono::steady_clock::now()};
        med2.run();
        auto endTime{ std::chrono::steady_clock::now() };
	    auto  time{ duration_cast<std::chrono::milliseconds>(endTime - startTime) };
        std::cout<<std::setprecision(16)<<std::format("process time for Three Point Solver {}", time);

    }

    {
        auto startTime{std::chrono::steady_clock::now()};
        med3.run();
        auto endTime{ std::chrono::steady_clock::now() };
	    auto time{ duration_cast<std::chrono::milliseconds>(endTime - startTime) };
        std::cout<<std::format("process time for BrentIntervalSolver {}", time);

    }
    std::cout << "\nend\n";




}