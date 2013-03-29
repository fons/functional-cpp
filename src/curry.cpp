#include "proto.hpp"
#include "show.hpp"
#include "curry.hpp"


int cur_1()
{
  auto plus = [](int x, int y) { return x+y;};
  auto pl = curry<decltype(plus), int,int>(plus);
  
  std::cout << pl(1)(2) << " , " << plus(1,2) << std::endl;

  auto f = [] (int a, int b, float c, char d, char e) {
    return (a+b) * c - ((int) d - (int) e);
  };
  auto cf = curry_helper<5, decltype(f), int, int,float,char,char>()(f);
  //auto var = cf(f);
  std::cout << f(1,2,6.89,'a','z') << "," << cf(1)(2)(6.89)('a')('z') << std::endl;
  auto cf2 = curry<decltype(f), int, int, float, char, char>(f);
  std::cout << f(1,2,6.89,'a','z') << "," << cf2(1)(2)(6.89)('a')('z') << std::endl;

  return 0;

}
