#include "proto.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "show.hpp"
#include "curry.hpp"
 
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
  auto cff = curry<decltype(ff), int, int, int>(ff);
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

  std::function<int (int) > f = [] (int a) { return 2*a;};

  typedef int  T1;
  typedef char T2;

  auto ff = [] (T1 a, T2 b) { return std::make_tuple(a,b);};
  
  // 0 : curry ff

  auto cff = curry<decltype(ff), decltype(ff(T1(), T2())), int, char>(ff);
  
  //1.. lift cury_ff L
  auto cffl = applicative_functor<std::forward_list>::pure(cff);
  //2 apply applicative
  auto C    = applicative_functor<std::forward_list>::apply<T1, decltype(cff(T1())) >(cffl)(L);
  //3.. apply applicative
  auto J    = applicative_functor<std::forward_list>::apply<T2, decltype(cff(T1())(T2()))>(C)(K);

  std::cout << J << std::endl;

  return 0;
}



