
#include <vector>
#include <iostream>
#include <vector>
#include <tuple>
#include "print_utils.hpp"

//Left
// 0	0	0	0	0
// 1	-2	1	0	0
// 0	1	-2	1	1
// 0	0	1	-2	0
// 0	0	0	0	0


//Right      
// 0
// -0.125
// -0.125
// -0.125
// 0

// Solution
// 0
// 0.1875
// 0.25
// 0.1875
// 0

// Using template template parameters to model vectors
template <typename T, 
			template <typename T1, typename Alloc> class Container = std::vector,
			typename Alloc = std::allocator<T>>
class DoubleSweep
{ // The Balayage method from Godounov

private:
	
		// The vectors of length J and start index 0
		Container<T, Alloc> a, b, c, f;

		// Dirichlet boundary conditions
		T left;		// Left boundary condition
		T right;	// Right boundary condition

		// Work arrays
		Container<T, Alloc> L;
		Container<T, Alloc> K;

public:
		// Constructors and destructor
		DoubleSweep() = delete;
		DoubleSweep(const DoubleSweep<T,Container, Alloc>& s2) = delete;

		// Create members to initialise input for AU = F, A = (a,b,c)
		DoubleSweep(const Container<T, Alloc>& lowerDiagonal, const Container<T, Alloc>& diagonal,
			const Container<T, Alloc>& upperDiagonal, const Container<T, Alloc>& F,
			const T& bc_left, const T& bc_right)
		{

			// Vectors are copied
			a = lowerDiagonal;
			b = diagonal; 
			c = upperDiagonal;
			f = F;

			left = bc_left;
			right = bc_right;

			std::size_t N = a.size();

			// Work arrays
			L = Container<T, Alloc>(N, 0);
			K = Container<T, Alloc>(N, 0);
		}
		virtual ~DoubleSweep() = default;


		// Operator overloading
		DoubleSweep<T,Container, Alloc>& operator = (const DoubleSweep<T, Container>& i2) = delete;

		Container<T, Alloc> solve() 
		{ // Result; this is a vector in closed range [0, J], a vector of size J+1.
			std::size_t N = a.size();
		
			// equation 13.7	
			L[0] = 0.0;
			K[0] = left;
			print("L: {}", L);
			print("K: {} ", K);

			// Equation 13.6
			std::size_t SZ = L.size();
			for (std::size_t j = 1; j < SZ; ++j)
			{ // L

				double tmp = b[j] + (a[j] * L[j - 1]);

				L[j] = -c[j] / tmp;
				K[j] = (f[j] - (a[j] * K[j - 1])) / tmp;
			}
			print("L: {}", L);
			print("K: {} ", K);
				
			// Equation 13.5. Recycle array f for u
			f[0] = left;
			f[N - 1] = right;
			for (std::size_t j = f.size() - 2; j >= 1; --j)
			{ // U

				f[j] = (L[j] * f[j + 1]) + K[j];
			}
			print("f {}", f);
			return f;
		}

		Container<T, Alloc> operator () ()
		{
			return solve();
		}

};

int main(){
 
   std::size_t J = 20;
	std::cout << "Number of subdivisions J ";
	std::cin >> J;
	
	double h = 1.0 / static_cast<double>(J);

	// Boundary conditions
	double BCL = 0.0;	// LHS
	double BCR = 0.0;	// RHS

	// Double Sweep
	std::vector<double> a(J+1,1.0);
	std::vector<double> b(J+1,-2.0);
	std::vector<double> c(J+1,1.0);
	std::vector<double> r(J+1, -2.0*h*h);	// Right-hand side
	


	DoubleSweep<double> mySolver(a, b, c, r, BCL, BCR);


	std::vector<double> result = std::move(mySolver());

}







 