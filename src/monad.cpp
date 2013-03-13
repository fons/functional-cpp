#include "proto.hpp"
#include "monad.hpp"



// [a] -> (a ->[b]) ->[b]
template<typename A>
std::forward_list<A> bind_list (std::forward_list<A> L, std::function<std::forward_list<A> (A)> f)
{
  std::function<std::forward_list<A> (std::forward_list<A>, std::forward_list<A>)> concat = [] (std::forward_list<A> L, std::forward_list<A> R) { L.splice_after(L.before_begin(), R); return L;};
  auto op = std::bind(concat, std::placeholders::_1, std::bind(f, std::placeholders::_2));
  return std::accumulate(L.begin(), L.end(), std::forward_list<A>(), op);;
}


int m_1()
{

  typedef std::forward_list<int> list_t;
  list_t L = {1,-6,23,78,45,13};
  std::function<list_t (int)> l1 = [] (int y) {list_t K; K.push_front(2*y +1); return K;};  
  auto res1 = bind_list(L, l1);
  std::cout << "results : " << std::endl;
  for (auto& v: res1) {
    std::cout << v << std::endl;
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
  auto res = functor<binary_op>::fmap(f, g);
  std::cout << "res(8) : " << res(8) << std::endl;

  std::function<int(int,int)> uncur = [=] (int x, int y) { return plus(x)(y);};
  auto res2 = monad<binary_op>::bind(f, uncur);
  std::cout << " res (10) : " << res(10) << std::endl;
  return 0;
}

int m_4()
{
  /*
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
  */
  return 0;
}

int m_5()
{

  return 0;
}

