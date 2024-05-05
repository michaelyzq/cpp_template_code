#pragma once
#include <iostream>
#include <vector>
#include <string_view>
#include <string>
#include <format>
#include <cstdio>
#include <tuple>
#include <concepts>
#include <format>
#include <sstream>
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
std::string to_string(const std::vector<T> v){
   std::stringstream ss;
   std::string temp ;
   ss<< "[";
   for (auto i = v.begin(); i !=v.end(); ++i)
   {
      ss<< (i<v.end()-1 ? std::format("{}, ",*i) :std::format("{}, ]",*i));
   }
      //temp =  i<v.end()-1 ? std::format("{}, ",*i) :std::format("{}, ]",*i);

   return ss.str();
}

template<typename T>
void print(const std::string_view fmt_st, const std::vector<T> v) {
   print(fmt_st, to_string<T>(v));
}



template<typename T>
void print(T t){
    std::cout<< t<<std::endl;
}