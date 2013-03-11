#include "proto.hpp"
#include "w.hpp"
#include "applicative_functor.hpp"

int apf_1()
{

  std::function<int (W)> show =[](W w) { w.pp(std::cerr) << std::endl; return w.ssn();};

  auto p = applicative_functor<std::shared_ptr>::pure(W(1090867, "hello_kitty"));
  auto s = applicative_functor<std::shared_ptr>::pure(show);
  std::cout << p << std::endl;
  applicative_functor<std::shared_ptr>::fmap(show)(p);
  applicative_functor<std::shared_ptr>::apply(s)(p);

  return 0;
}


int apf_2()
{

  std::forward_list<int> K = {2, 5, 10};
  std::forward_list<int> L = {8, 10, 11};
  std::function<int(int)> show = [=](int v) { 
    std::cout << v << ",";
    return v;
  };
  std::function < std::function < int (int) > (int)> plus = [] (int x) {
    return [=] (int y) {
      return x + y;
    };
  };

  auto ls  = applicative_functor<std::forward_list>::pure(show);
  auto lp  = applicative_functor<std::forward_list>::pure(plus);
  auto kl  = applicative_functor<std::forward_list>::apply(lp)(K);
  auto M   = applicative_functor<std::forward_list>::apply(kl)(L);

  applicative_functor<std::forward_list>::apply(ls)(K);
  std::cout << std::endl;
  applicative_functor<std::forward_list>::apply(ls)(L);
  std::cout << std::endl;
  applicative_functor<std::forward_list>::apply(ls)(M);
  std::cout << std::endl;

  return 0;
}

int apf_3()
{
  auto val2 = applicative_functor<binary_op>::pure<int,int>(56);
  std::cout << " pure (56) : "<< val2(89) << std::endl;

  auto val3 = applicative_functor<binary_op>::pure<int>(56);
  std::cout << " pure (56) : "<< val3(8990) << std::endl;

  return 0;
}

int apf_4()
{
  std::function<int (int, int)> f = [] (int x, int y) { return x + y + 5 ;};
  std::function<int (int)> g = [] (int x) { return x * 400;};
  auto res = applicative_functor<binary_op>::apply(f,g);
  std::cout << " val : " << res(3) << std::endl;
  return 0;
}

int apf_5()
{
  std::function<int (int, int)> f = [] (int x, int y) { return x + y + 5 ;};
  std::function<int (int)> g = [] (int x) { return x * 400;};

  auto res = applicative_functor<binary_op>::apply(f)(g);
  std::cout << " val : " << res(3) << std::endl;
  return 0;
}

