#include "proto.hpp"
#include "monad.hpp"
#include "show.hpp"



template<typename A, typename F>
auto mapM (F f, std::forward_list<A> L) -> decltype(f(A()))
{
  typedef typename decltype(f(A()))::value_type ret_t;
  L.reverse();
  auto concat = [] (std::forward_list<ret_t> L, std::forward_list<ret_t> R) { 
    L.splice_after(L.before_begin(), R); 
    return L;
  };
  auto op     = std::bind(concat, std::placeholders::_1, std::bind(f, std::placeholders::_2));
  return std::accumulate(L.begin(), L.end(), std::forward_list<ret_t>(), op);;
}



int m_0()
{
  auto show   = [] (int v) { std::cout << v << ","; return v;};
  typedef std::forward_list<int> list_t;
  list_t L = {1,-6,23,78,45,13};
  auto l1  = [] (int y) {return list_t ({2*y+1, -y, -2*y + 1});};
  map(show, mapM(l1, L));
  return 0;
}

int m_1()
{  
  auto show   = [] (std::tuple<int,char> v) { std::cout << v << ","; return v;};
  static char digits[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
  typedef std::forward_list<std::tuple<int, char>> list_t;
  auto op  = [=] (int y) {return list_t({std::make_tuple(y, digits[abs(y)%10])});};
  map(show, mapM(op, std::forward_list<int>({1,-6,23,78,45,13})));

  auto res = map(op, std::forward_list<int>({1,-6,23,78,45,13}));
  std::cout << std::endl <<  "-----------" << std::endl;
  for (auto& el : res) {
    std::cout << "[";
    map(show, el);
    std::cout << "], ";
  }
  return 0;
}



int m_2()
{

  std::forward_list<int> L = {1,3,45,78};
  auto op = [=](int x) {
    std::forward_list<int> R = {x , -x};
    return R;
  };
  std::forward_list<int> R = monad<std::forward_list>::bind<int,int>(L)(op);
  for (auto& v: R) {
     std::cout<< v << "," ;
  }
  std::cout << std::endl;

  return 0;
}

int m_3()
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

  auto f = mult(5);
  auto g = plus(3);
  auto res = functor<unary_op>::fmap(f, g);
  std::cout << "res(8) : " << res(8) << std::endl;

  std::function<int(int,int)> uncur = [=] (int x, int y) { return plus(x)(y);};
  auto res2 = monad<unary_op>::bind(f, uncur);
  std::cout << " res (10) : " << res(10) << std::endl;
  return 0;
}

int m_4()
{

  std::forward_list<int> L = {1,2};
  auto op = [=](int x) {
    std::forward_list<char> R = {'a' , 'b'};
    return R;
  };

  auto R = monad<std::forward_list>::bind<int,char>(L)(op);
  for (auto& v: R) {
    std::cout << v << ",";
  }
  std::cout << std::endl;

  return 0;
}

int m_5()
{
  
  return 0;
}

