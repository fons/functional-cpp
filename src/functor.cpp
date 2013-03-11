#include "proto.hpp"
#include "w.hpp"
#include "map.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"


int functor_1()
{
  std::forward_list<W> L = {W(10, "a"), W(20, "b"), W(3467, "mnhjk")};
  std::function<int (W)> show =[](W w) { w.pp(std::cerr) << std::endl; return w.ssn();};
  functor<std::forward_list>::fmap(show)(L);

  return 0;
}

int functor_2()
{
  std::function<int(int)> f = [=](int x) { return 3 * x;};
  std::function<int(int)> g = [=](int x) { return 100 + x;};
  auto res = functor<binary_op>::fmap(f,g);
  std::cout << " res : " << res(1) << std::endl; 
  return 0;
}

int functor_3()
{
  std::function<int(int)> f = [](int x) { return 3 * x;};
  std::function<int(int)> g = [](int x) { return 100 + x;};
  std::function<int(int)> h = [](int x) { return 5 * x - 700;};
  auto r   = functor<binary_op>::fmap(f,g);
  auto res = functor<binary_op>::fmap(h, r);
  std::cout << " res : " << res(1) << std::endl; 
  return 0;
}

int functor_4()
{
  std::function<int(int)> f = [](int x) { return 3 * x;};
  std::function<int(int)> g = [](int x) { return 100 + x;};
  auto res   = functor<binary_op>::fmap<int,int,int>(f)(g);

  std::cout << " res : " << res(1) << std::endl; 
  return 0;
}

int functor_5()
{
  std::forward_list<W> L = {W(10, "a"), W(20, "b"), W(3467, "mnhjk")};
  std::function<int (W)> show =[](W w) { w.pp(std::cerr) << std::endl; return w.ssn();};
  functor<std::forward_list>::fmap(show, L);

  return 0;
}

int functor_6()
{
  std::function<std::function<int(int)>(int)> plus = [] (int x) {
    return [=] (int y) {
      return x + y;
    };
  };

  auto pl3 = plus (3);

  std::cout << " 3 + 9 : " << pl3(9) << std::endl;

  auto r   = functor<binary_op>::fmap(plus,pl3);
  int v = r(0)(0) ;
  std::cout << " r (5) : " << v << std::endl;
  return 0;
}
