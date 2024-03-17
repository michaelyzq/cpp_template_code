#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

//reasearch paper 
//https://www.researchgate.net/publication/23696949_Valuing_Energy_Options_in_a_One_Factor_Model_Fitted_to_Forward_Prices

double price(double K, double T, double S, double sig, double r, double H, int N){

    double dt = T/N;
    double nu = r-0.5*sig*sig;
    double dxu = std::sqrt(sig*sig*dt+(nu*dt)*(nu*dt));
    double dxd = -dxu;

    double pu = 0.5+0.5*(nu*dt/dxu);
    double pd = 1.0-pu;

    double disc = std::exp(-r*dt);
    double dpu = disc*pu;
    double dpd = disc*pd;
    double edxud = std::exp(dxu - dxd);
    double edxd = std::exp(dxd);

    std::vector<double> St(N+1);
    St[0] = S*std::exp(N*dxd);
    for (std::size_t j = 1; j < St.size(); ++j)
    {
        St[j] = edxud*St[j-1];
    }

    std::vector<double> C(N+1);
    for (std::size_t j = 0; j < C.size(); ++j)
    {
    if (St[j] > H)
        {
           C[j] = std::max<double>(St[j] - K, 0.0);
        }
        else
        {
          C[j] = 0.0;
        }
    }
    // Backwards induction phase
    for (int i = N-1; i >= 0; --i) // Can't use std::size_t as index
    {
       for (std::size_t j = 0; j <= i; ++j)
       {
          St[j] /= edxd;
              
       if (St[j] > H)
       {
          C[j] = dpd*C[j] + dpu*C[j+1];
          // Early exercise condition
          C[j] = std::max<double>(C[j], St[j] - K);
       }
       else
        {
            C[j] = 0.0;
        }
        }
    }
    // Early exercise down and out call
    return C[0];

}

int main(){
      double K = 100.0;
    double S = 100.0;
    double T = 1.0;
    double r = 0.06;
    double q = 0.0;
    double sig = 0.2;
    int N = 3;
              
    double H = 95;
    double optionPrice = price(K,T,S,sig,r,H,N);
    std::cout << "Price:" << optionPrice << std::endl;
    return 0;


 }
