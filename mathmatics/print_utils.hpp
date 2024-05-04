#include <iostream>
#include <vector>
#include <string_view>
#include <string>
#include <format>
#include <cstdio>
#include <tuple>

template<typename...Args>
void print(const std::string_view fmt_str, Args&&...args){
   auto fmt_args{
      std::make_format_args(args...)
   };
   //std::string outstr{vformat(fmt_str, fmt_args)};
   std::string outstr{ vformat(fmt_str, fmt_args) };
   fputs(outstr.c_str(), stdout);

}


template<typename...T, size_t N = sizeof...(T)>
constexpr void print(const std::tuple<T...>& tup) {
	std::cout << "[";
	[&] <size_t...I>(std::index_sequence<I...>) {
		(..., print((I != N - 1 ? "{}, " : "{} ]"), std::get<I>(tup)));
	}(std::make_index_sequence<N>());
	endl(std::cout);
}

template<typename T>
constexpr void print(const std::vector<T> v) {

	std::cout << "[";
   for (auto i = v.begin(); i !=v.end(); ++i)
      print(i< v.end()-1 ? "{}, " : "{} ]", *i);
	endl(std::cout);
}


template <typename T>
void print(T t){
    std::cout<< t<<std::endl;
}