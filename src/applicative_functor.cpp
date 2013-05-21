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
  auto val2 = applicative_functor<unary_op>::pure<int,int>(56);
  std::cout << " pure (56) : "<< val2(89) << std::endl;

  auto val3 = applicative_functor<unary_op>::pure<int>(56);
  std::cout << " pure (56) : "<< val3(8990) << std::endl;

  return 0;
}

int apf_4()
{
  std::function<int (int, int)> f = [] (int x, int y) { return x + y + 5 ;};
  std::function<int (int)> g = [] (int x) { return x * 400;};
  auto res = applicative_functor<unary_op>::apply(f,g);
  std::cout << " val : " << res(3) << std::endl;
  return 0;
}

int apf_5()
{
  std::function<int (int, int)> f = [] (int x, int y) { return x + y + 5 ;};
  std::function<int (int)> g = [] (int x) { return x * 400;};

  auto res = applicative_functor<unary_op>::apply(f)(g);
  std::cout << " val : " << res(3) << std::endl;
  return 0;
}

int apf_6()
{

  std::function<std::function<int(int)>(int)> plus = [=] (int x) {
    return [=] (int y) {
      return x + y;
    };
  };

  std::function<std::function<int(int)>(int)> mult = [] (int x) {
    return [=] (int y) {
      return x * y;
    };
  };

  auto m100 = mult(100);
  auto pl3  = plus(3);

  std::cout << "pl3 : " << pl3(90) << std::endl;


  auto r    = functor<unary_op>::fmap(plus,pl3);
  
  std::cout << " val : " << r(3)(3) << std::endl;

  std::function<int(int,int)> uncur = [&r](int x, int y) { return r(x)(y);};

  std::cout << "uncur : " << uncur(3,3) << " compare : " << r(3)(3) << std::endl;

  auto res = applicative_functor<unary_op>::apply(uncur, m100);
  std::cout << " res : " << res(5) << std::endl;

  return 0;
}

int apf_7()
{

  std::function<std::function<int(int)>(int)> plus = [=] (int x) {
    return [=] (int y) {
      return x + y;
    };
  };

  std::function<std::function<int(int)>(int)> mult = [] (int x) {
    return [=] (int y) {
      return x * y;
    };
  };

  auto m100 = mult(100);
  auto pl3  = plus(3);

  std::cout << "pl3 : " << pl3(90) << std::endl;

  auto r    = functor<unary_op>::fmap(plus,pl3);
  
  std::cout << " val : " << r(3)(3) << std::endl;

  auto res2 = applicative_functor<unary_op>::apply(r, m100);
  std::cout << " res2 : " << res2(5) << std::endl;

  return 0;
}

int apf_8()
{

  std::function<std::function<int(int)>(int)> plus = [=] (int x) {
    return [=] (int y) {
      return x + y;
    };
  };

  std::function<std::function<int(int)>(int)> mult = [] (int x) {
    return [=] (int y) {
      return x * y;
    };
  };

  auto m100 = mult(100);
  auto pl3  = plus(3);

  std::cout << "pl3 : " << pl3(90) << std::endl;

  auto res2 = applicative_functor<unary_op>::apply(functor<unary_op>::fmap(plus,pl3), m100);
  std::cout << " res2 : " << res2(5) << std::endl;

  return 0;
}

