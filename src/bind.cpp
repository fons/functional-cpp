#include "proto.hpp"

using namespace std;
using namespace std::placeholders;

int bind_1()
{

  auto l = std::bind(plus<int>(), _1, 10);
  cout << l(902) << endl;
  return 0;
}

int bind_2()
{

  auto l = std::bind([=](int x, int y){return 5 * x + y; }, _1, 10);
  cout << l(902) << endl;
  return 0;
}

// Bind is not curry...
int bind_3()
{

  auto l = std::bind([=](int x, int y){return 5 * x + y; }, _2, _1);
  auto r = l(10, 8);
  cout << r << endl;
  return 0;
}

int bind_4()
{
  auto multi  = [](int x, int y) { return x*y;};
  auto repeat = [](int n, int x) { int y = 1;
				   while (n-- > 0) y *= x;
				   return y;};

  auto rpl    = std::bind (repeat,
			   std::placeholders::_1,
			   std::bind(multi,
				     std::placeholders::_2,
				     std::placeholders::_3));

  std::cout << " multi (10,2) : " << multi(10,2) << std::endl; 
  std::cout << " repeat (2,2) : " << repeat(2,2) << std::endl; 
  auto val = rpl(4, 1, 2); //( 1 * 2) ^ 4
  std::cout << " result : " << val << std::endl;
  return 0;
}


int bind_5()
{
  std::function<double (double)> l1     = [](double x) { return 2*x-0.906;};
  auto repeat = [](int n, double y, std::function<double(double)> f) { 
    while (n-- > 0) {
      y = f(y);
    }
    return y;
  };

  auto rpl    = std::bind (repeat,
			   std::placeholders::_1,
			   std::placeholders::_1,
			   std::placeholders::_2);

  auto val = rpl(9, l1); // print 4145.03
  std::cout << " result : " << val << std::endl;
  return 0;
}
