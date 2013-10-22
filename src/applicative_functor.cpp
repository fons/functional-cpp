#include "proto.hpp"
#include "show.hpp"
#include "w.hpp"
#include "applicative_functor.hpp"
#include "list_of_ptr.hpp"
#include "forward_zip_list.hpp"
#include "zip_list.hpp"

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
typedef int (FP)(int);

int fgy(int x) 
{
	return 200*x-99;
}

int apf_9()
{
  typedef std::tuple<int,std::string> W;
  std::forward_list<std::shared_ptr<W>> L = {std::shared_ptr<W>{new W(10, "a")}, std::shared_ptr<W>{new W(20, "b")}, std::shared_ptr<W>{new W(3467, "mnhjk")}};
  std::function<W (W)> show =[](const W& w) { std::cerr << w << std::endl; return w;};
  auto lifted_show = applicative_functor<std::forward_list>::pure(applicative_functor<std::shared_ptr>::apply(applicative_functor<std::shared_ptr>::pure(show)));
  applicative_functor<std::forward_list>::apply(lifted_show)(L);

  return 0;
}

int apf_10()
{
    forward_list_of_ptr<int> L = {std::make_shared<int>(5),std::make_shared<int>(15),std::make_shared<int>(25),std::make_shared<int>(35)};
    auto f = [](const int& c) { std::cerr << c << ","; return c;};
    functor<forward_list_of_ptr>::fmap<int,int>(f)(L);
	std::cerr << std::endl;
    auto y = applicative_functor<forward_list_of_ptr>::pure(45);
    auto lifted_lambda = applicative_functor<forward_list_of_ptr>::pure(f);
    applicative_functor<forward_list_of_ptr>::apply<int,int>(lifted_lambda)(L);
	std::cerr << std::endl;
    applicative_functor<forward_list_of_ptr>::apply<int,int>(lifted_lambda)(y);
	std::cerr << std::endl;
    return 0;
}


int apf_11()
{
	forward_zip_list<int> L = {2, 5, 10};
	auto f = [](const int& c) { std::cerr << c << ","; return c;};
	functor<forward_zip_list>::fmap<int,int>(f)(L);
	std::cerr << std::endl;

	auto y = applicative_functor<forward_zip_list>::pure(45);
	auto lifted_lambda = applicative_functor<forward_zip_list>::pure(f);

	applicative_functor<forward_zip_list>::apply<int,int>(lifted_lambda)(L);
	std::cerr << std::endl;
	applicative_functor<forward_zip_list>::apply<int,int>(lifted_lambda)(y);
	std::cerr << std::endl;
    return 0;
}

int apf_12()
{

	std::list<int> L = {2, 5, 10};
	auto f = [](const int& c) { std::cerr << c << ","; return c;};
	
	functor<zip_list>::fmap<int,int>(f)(L);
	std::cerr << std::endl;

	auto lifted_lambda_1 = applicative_functor<std::list>::pure(f);
	applicative_functor<std::list>::apply<int,int>(lifted_lambda_1)(L);
	std::cerr << std::endl;

	auto lifted_lambda = applicative_functor<zip_list>::pure(f);
	applicative_functor<zip_list>::apply<int,int>(lifted_lambda)(L);
	std::cerr << std::endl;
	auto y = applicative_functor<zip_list>::pure(45);
	applicative_functor<zip_list>::apply<int,int>(lifted_lambda)(y);
	std::cerr << std::endl;

    return 0;
}


int apf_13()
{
	std::function<int(int)> f = [](int x){ return 45*x - 89;};
	auto F = applicative_functor<raw_pointer>::pure(f);
	auto V = new int (56);
	auto res = applicative_functor<raw_pointer>::apply(F, V);
	std::cerr << "V : " << *V << "==> " <<  *res << std::endl;
	return 0;
}

int apf_14()
{
	std::function<int(int)> f = [](int x){ return 45*x - 89;};
	auto F = applicative_functor<raw_pointer>::pure([](int x){ return 45*x - 89;});
	auto V = new int (56);
	auto res = applicative_functor<raw_pointer>::apply<int,int>(F, V);

	std::cerr << "V : " << *V << "==> " <<  *res << std::endl;

	return 0;
}
