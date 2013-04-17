#include "proto.hpp"
#include "map.hpp"
#include "zip.hpp"
#include "show.hpp"

static char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

static char irc (int i)
{
  return digits[ abs(i) % 10 ];
}

int trans_1()
{
  std::forward_list<int> L = {1,2,3,4,5,67,12};
  auto v = irc (2345);
  std::cout << " v : " << v << std::endl;

  std::function <std::string (std::string&, int)> op = [] (std::string& a, int v) { a.append(1, irc(v)); return a;};

  auto op2 = [] (std::string& a, int v) { a.append(1, irc(v)); return a;};

  std::string res = std::accumulate(L.begin(), L.end(), std::string(""), op2);
  std::cout << " res : " << res << std::endl;
  return 0;
}

int trans_2()
{

  std::forward_list<int> L = {1,-6,23,78,45,13};
  auto max = [] (int x, int y) { return (x > y) ? x : y;};
  auto res = std::accumulate(L.begin(), L.end(), std::numeric_limits<int>::min(), max);
  std::cout << "maximum : " << res << std::endl;
  return 0;
}

int trans_3()
{
  auto show   = [] (int v) { std::cout << v << ","; return v;};
  typedef std::list<int> list_t;
  list_t L = {1,-6,23,78,45,13};
  auto m   = [] (list_t L, int y) { L.push_back( 2*y + 1); return L;};
  auto res = std::accumulate(L.begin(), L.end(), list_t(), m);
  map(show,res);
  return 0;
}

int trans_4()
{
  typedef std::forward_list<int> list_t;
  list_t L    = {1,-6,23,78,45,13};
  auto op     = [] (int y) {return list_t({2*y+1});};
  auto concat = [] (list_t A, list_t B) { A.splice_after(A.before_begin(), B); return A;};
  auto bind   = std::bind(concat, std::placeholders::_1, std::bind(op, std::placeholders::_2));
  auto show   = [] (int v) { std::cout << v << ","; return v;};
  auto res    = std::accumulate(L.begin(), L.end(), list_t(), bind);
  map(show, res);
  return 0;
}

int trans_5()
{
  std::forward_list<int> L = {1,67,89,23,45,1,3,99,-90};
  auto show = [] (int v) { std::cout << v << ","; return v;};
  
  auto R1 = map (show, L);
  auto R2 = map ([] (int y) {return (y + 79) % 45;}, R1); 
  map(show, R2);
  return 0;
}

int trans_6()
{
  std::forward_list<int> L = {1,67,89,23,45,1,3,99,-90};
  auto show = [] (int v) { std::cout << v << ","; return v;};
  std::function< std::function <int(int)>(int)> op   = [] (int x) {
    return [=] (int y) {
      return 4 * x + y;
    };
  };
  std::cout << std::endl << "--------------" << std::endl;
  map(show, 
      map([](std::function<int(int)> f){return f(2);}, 
	  map(op, L)));
  return 0;
}

int trans_7()
{
  std::forward_list<int> L = {1,67,89,23,45,1,3,99,-90};
  auto show = [] (int v) { std::cout << v << ","; return v;};
  std::function< std::function <int(int)>(int)> op   = [] (int x) {
    return [=] (int y) {
      return 4 * x + y;
    };
  };
  auto l = std::bind([](std::function<int(int)> f){return f(2);},
		     std::bind(op, std::placeholders::_1));

  map(show, map(l, L));

  return 0;
}


int trans_8()
{
  std::forward_list<int>  L = {1,67,89,23,45,1,3,99,-90};
  std::forward_list<char> R = {'a','b','l','u','t','v','r','6','h'};

  map ([] (int v) { std::cout << v << ","; return v;}, L);
  std::cout << std::endl;

  map ([] (char v) { std::cout << v << ","; return v;}, R);
  std::cout << std::endl;

  auto H2 = zip (L, R);

  map([] (std::tuple<int,char> v) { std::cout << v << ","; return v;},H2); 

  std::cout << std::endl;
  return 0;
}

int trans_9()
{
  std::forward_list<int>  L = {1,67,89,23,45,1,3,99,-90};
  std::forward_list<char> R = {'a','b','l','u','t','v','r','6','h'};

  map ([] (int v) { std::cout << v << ","; return v;}, L);
  std::cout << std::endl;

  map ([] (char v) { std::cout << v << ","; return v;}, R);
  std::cout << std::endl;
  auto op = [] (int x, char z) {
      return std::make_tuple(x,z);
  };
  auto res = zipWith<int,char>(op)(L)(R);
  map([] (std::tuple<int,char> v) { std::cout << v << ","; return v;}, res);

  std::cout << std::endl;
  return 0;
}

