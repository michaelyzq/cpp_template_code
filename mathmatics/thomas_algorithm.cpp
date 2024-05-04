// Paper :http://www.industrial-maths.com/ms6021_thomas.pdf
// Sample test python code
// import numpy as np
// l = np.array([[0]*3 for i in range(3)])

// for i in range(2):
//     l[i][i+1]=1
// for i in range(3):
//     l[i][i]=-2

// for i in range(2):
//     l[i+1][i]=1
    
// r = np.array([-0.125]*3)

// print(l)
// [[-2  1  0]
//  [ 1 -2  1]
//  [ 0  1 -2]]
// print(r)
// [-0.125 -0.125 -0.125]
// ans =np. linalg.solve(l, r)
//print(ans)
// [0.1875 0.25   0.1875]



#include <vector>
#include <tuple>
#include <iostream>
#include "print_utils.hpp"




template <typename T>
using Vector = std::vector<T>;

template <class T> 
class LUTridiagonalSolver
{
    Vector<T> a;
    Vector<T> b;
    Vector<T> c;
    Vector<T> r;


    Vector<T> beta;
    Vector<T> gamma;
    Vector<T> z;
    Vector<T> solution;

    std::size_t Size;


    void calculateBetaGamma_ZU(){
        beta[0] = b[0];
        gamma[0] = c[0]/beta[0];
        z[0] = r[0]/beta[0];

        for (std::size_t j=1; j<Size-1;++j){
            beta[j]=b[j]-(a[j]*gamma[j-1]);
            gamma[j] = c[j]/beta[j];
            z[j] = (r[j] - (a[j]*z[j-1])) / beta[j];
        }



        
        beta[Size-1] =b[Size-1]-(a[Size-1]*gamma[Size-2]);
        z[Size-1] = (r[Size-1] - (a[Size-1]*z[Size-2])) / beta[Size-1];

        solution[Size-1] = z[Size-1];


        for (long i =Size-2; i>=0;--i){
            solution[i] = z[i] -(gamma[i]*solution[i+1]);
        }


    }

public:
    LUTridiagonalSolver() = delete;
    LUTridiagonalSolver(const LUTridiagonalSolver&source) = delete;
    virtual ~LUTridiagonalSolver()= default;
    LUTridiagonalSolver<T>& operator= (const LUTridiagonalSolver&source) = delete;
    

LUTridiagonalSolver(Vector<T>&lower, Vector<T>& diagonal, Vector<T>&upper, Vector<T>&RHS)
{   
    a = lower;
    b = diagonal;
    c = upper;
    r = RHS;

    Size = diagonal.size();

    beta = Vector<T>(Size);
    gamma = Vector<T>(Size);

    z = Vector<T>(Size);
    solution = Vector<T>(Size);
}

Vector<T> operator()(){
    calculateBetaGamma_ZU();
    return this->solution;
}

bool diagonallyDominant() const
{

    if(std::abs(b[0]) < std::abs(c[0]))
        return false;
    
    if (std::abs(b[Size-1]) <std::abs(a[Size-1]))
        return false;
    
    for (std::size_t j=1; j<Size-1; ++j)
    {
        if (std::abs(b[j]) <std::abs(a[j])+std::abs(c[j]))
            return false;
    }
    return true;
}

};



int main(){
 
   using value_type = double;

   std::size_t J=4;

   double h = 1.0/static_cast<double>(J);

   std::cout<<h<<std::endl;

   std::vector<double> a2(J-1, 1.0);
   std::vector<double> b2(J-1, -2.0);
   std::vector<double> c2(J-1, 1.0);
   std::vector<double> r2(J-1, -2*h*h);

   print(a2);
   print(b2);
   print(c2);
   print(r2);


	LUTridiagonalSolver<double> mySolver2(a2, b2, c2, r2);
	std::cout << "Matrix has a solution? " << mySolver2.diagonallyDominant() << '\n';
   print(mySolver2());


}
 