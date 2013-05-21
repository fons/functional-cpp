#include "proto.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "show.hpp"
#include "curry.hpp"
#include "do.hpp"

//ways to set up the do notation..


int do_1()
{
  std::forward_list<int> L = {1,2,3};
  std::function<int (int) > f = [] (int a) { return 2*a;};

  auto K = functor<std::forward_list>::fmap(f, L);
  std::cout << L << std::endl;
  std::cout << K << std::endl;

  std::function<int (int, int) > ff = [] (int a, int b) { return 2*a + 10*b;};

  // 0 : curry ff
  auto cff = curry<decltype(ff), int, int>(ff);
  //1.. fmap cury_ff L
  auto C = functor<std::forward_list>::fmap<int, decltype(cff(int()))>(cff, L);
  //2.. 
  auto J = applicative_functor<std::forward_list>::apply<int, int>(C)(K);

  std::cout << J << std::endl;

  return 0;
}

/*
Remember this ??
--[1,2] >>= \x->['a','b']>>= \ch->return (x,ch)

refC = do
  n <- [1,2]
  ch <- ['a','b']
  return (n, ch)

 */

int do_2()
{
  std::forward_list<int>  L = {1,2};
  std::forward_list<char> K = {'a','b'};


  typedef int  T1;
  typedef char T2;

  auto ff = [] (T1 a, T2 b) { return std::make_tuple(a,b);};
  
  // 0 : curry ff

  auto cff = curry<decltype(ff), int, char>(ff);
  
  //1.. lift cury_ff L
  auto cffl = applicative_functor<std::forward_list>::pure(cff);
  //2 apply applicative
  auto C    = applicative_functor<std::forward_list>::apply<T1, decltype(cff(T1())) >(cffl)(L);
  //3.. apply applicative
  auto J    = applicative_functor<std::forward_list>::apply<T2, decltype(cff(T1())(T2()))>(C)(K);

  std::cout << J << std::endl;

  return 0;
}


template<typename a> using m_t = std::forward_list<a>;

int do_3()
{
  typedef int         T1;
  typedef char        T2;
  typedef std::string T3;
  typedef applicative_functor<std::forward_list> apf_t;

  m_t<T1> L1 = {1,2};
  m_t<T2> L2 = {'a','b'};
  m_t<T3> L3 = {"hello" , "world", "dorks"};


  auto ff = [] (T1 a, T2 b, T3 c) { return std::make_tuple(a,b,c);};


  auto cff   = curry<decltype(ff), T1, T2, T3>(ff);
  auto cffl  = apf_t::pure(cff);
  auto C1    = apf_t::apply<T1, decltype(cff(T1())) >(cffl)(L1);
  auto C2    = apf_t::apply<T2, decltype(cff(T1())(T2()))>(C1)(L2);
  auto C3    = apf_t::apply<T3, decltype(cff(T1())(T2())(T3()))>(C2)(L3);

  std::cout << C3 << std::endl;

  return 0;
}


template<typename a> using s_t = std::shared_ptr<a>;

int do_4()
{

  typedef int         T1;
  typedef char        T2;
  typedef std::string T3;
  typedef applicative_functor<std::shared_ptr> apf_t;

  s_t<T1> L1 = apf_t::pure(1);
  s_t<T2> L2 = apf_t::pure('a');
  s_t<T3> L3 = apf_t::pure(std::string("hello"));

  auto ff = [] (T1 a, T2 b, T3 c) { return std::make_tuple(a,b,c);};

  auto cff   = curry<decltype(ff), T1, T2, T3>(ff);
  auto cffl  = apf_t::pure(cff);
  auto C1    = apf_t::apply<T1, decltype(cff(T1())) >(cffl)(L1);
  auto C2    = apf_t::apply<T2, decltype(cff(T1())(T2()))>(C1)(L2);
  auto C3    = apf_t::apply<T3, decltype(cff(T1())(T2())(T3()))>(C2)(L3);

  std::cout << C3 << std::endl;

  return 0;
}


int do_5 ()
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


  typedef applicative_functor<unary_op> apf_t;

  auto pl10 = plus(10);
  auto m100 = mult(2);

  auto C1  = functor<unary_op>::fmap(plus,pl10);
  auto C2  = apf_t::apply(C1, m100);

  std::cout << " res2 : " << C2(3) << std::endl;

  return 0;
}

int do_6 ()
{

  typedef int         T1;
  typedef char        T2;
  typedef std::string T3;

  m_t<T1> L1 = {1,2};
  m_t<T2> L2 = {'a','b'};
  m_t<T3> L3 = {"hello" , "world", "dorks"};

  auto f1 = [] (T1 a) { return std::make_tuple(a);};
  auto f2 = [] (T1 a, T2 b) { return std::make_tuple(a,b);};
  auto f3 = [] (T1 a, T2 b, T3 c) { return std::make_tuple(a,b,c);};

  auto R1 = do_list::dofor(L1, f1);
  std::cout << R1 << std::endl;

  auto R2 = do_list::dofor(L1, L2, f2);
  std::cout << R2 << std::endl;

  auto R3 = do_list::dofor(L1, L2, L3, f3);
  std::cout << R3 << std::endl;


  return 0;
}

int do_7()
{
  typedef int         T1;
  typedef char        T2;
  typedef std::string T3;
  typedef applicative_functor<std::shared_ptr> apf_t;
  s_t<T1> L1 = apf_t::pure(1);
  s_t<T2> L2 = apf_t::pure('a');
  s_t<T3> L3 = apf_t::pure(std::string("hello"));

  auto f1 = [] (T1 a) { return std::make_tuple(a);};
  auto f2 = [] (T1 a, T2 b) { return std::make_tuple(a,b);};
  auto f3 = [] (T1 a, T2 b, T3 c) { return std::make_tuple(a,b,c);};

  auto R1 = do_sp::dofor(L1, f1);
  std::cout << R1 << std::endl;

  auto R2 = do_sp::dofor(L1, L2, f2);
  std::cout << R2 << std::endl;

  auto R3 = do_sp::dofor(L1, L2, L3, f3);
  std::cout << R3 << std::endl;

  return 0;
}

int do_8()
{
  typedef int         T1;
  typedef char        T2;
  typedef std::string T3;
  //template<typename a> using m_t = std::forward_list<a>;
  typedef applicative_functor<std::forward_list> apf_t;
  m_t<T1> L1 = apf_t::pure(1);
  m_t<T2> L2 = apf_t::pure('a');
  m_t<T3> L3 = apf_t::pure(std::string("hello"));

  auto f1 = [] (T1 a) { return std::make_tuple(a);};
  auto f2 = [] (T1 a, T2 b) { return std::make_tuple(a,b);};
  auto f3 = [] (T1 a, T2 b, T3 c) { return std::make_tuple(a,b,c);};

  auto R1 = do_list::dofor(L1, f1);
  std::cout << R1 << std::endl;

  auto R2 = do_list::dofor(L1, L2, f2);
  std::cout << R2 << std::endl;

  auto R3 = do_list::dofor(L1, L2, L3, f3);
  std::cout << R3 << std::endl;

  return 0;
}

