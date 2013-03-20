#include "proto.hpp"


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
  std::function<int (int, int) > max = [] (int x, int y) { return (x > y) ? x : y;};
  auto res = std::accumulate(L.begin(), L.end(), std::numeric_limits<int>::min(), max);
  std::cout << res << std::endl;
  return 0;
}

int trans_3()
{
  typedef std::forward_list<int> list_t;

  list_t L = {1,-6,23,78,45,13};
  list_t K;

  std::function<list_t (list_t, int) > mappy = [] (list_t L, int y) { L.push_front( 2*y + 1); return L;};
  
  auto res = std::accumulate(L.begin(), L.end(), K, mappy);
  for (auto& v: res) {
    std::cout << v << std::endl;
  }
  return 0;
}

int trans_4()
{
  typedef std::forward_list<int> list_t;
  list_t L = {1,-6,23,78,45,13};

  std::function<list_t (int)> l1 = [] (int y) {list_t L; L.push_front(2*y +1); return L;};

  std::function<list_t (list_t, list_t)> concat = [] (list_t A, list_t B) { A.splice_after(A.before_begin(), B); return A;};
  auto op = std::bind(concat, std::placeholders::_1, std::bind(l1, std::placeholders::_2));

  auto res1 = std::accumulate(L.begin(), L.end(), list_t(), op);
  for (auto& v: res1) {
    std::cout << v << std::endl;
  }
  return 0;
}


template<typename A, typename B>
std::forward_list<B> map (std::function<B(A)> f, const std::forward_list<A>& L) 
{
  std::forward_list<B> H;
  std::transform(L.begin(), L.end(), std::front_inserter(H), f);
  H.reverse();
  return H;
}

int trans_5()
{
  std::forward_list<int> L = {1,67,89,23,45,1,3,99,-90};
  std::function<int(int)> show = [] (int v) { std::cout << v << ","; return v;};
  map(show, L);
  std::function<int(int)> op = [] (int y) {return (y + 79) % 45;};
  std::forward_list<int> H2 = map (op, L);

  std::cout << std::endl << "--------------" << std::endl;
  map(show, H2);
  return 0;
}

template<typename A, typename B>
std::forward_list<std::tuple<A,B>> zip (const std::forward_list<A>& L, const std::forward_list<B>& M) 
{
  std::forward_list<std::tuple<A,B>> H;
  auto zipper = [] (const A& a, const B& b) {return std::make_tuple(a,b);};
  std::transform(L.begin(), L.end(), M.begin(), std::front_inserter(H), zipper);
  H.reverse();
  return H;
}

int trans_6()
{
  std::forward_list<int>  L = {1,67,89,23,45,1,3,99,-90};
  std::forward_list<char> M = {'a','b','l','u','t','v','r','6','h'};
  std::function<int(int)> showi = [] (int v) { std::cout << v << ","; return v;};
  std::function<int(char)> showc = [] (char v) { std::cout << v << ","; return v;};
  std::function<std::tuple<int, char> (std::tuple<int,char> v)> show = [] (std::tuple<int,char> v) { std::cout << "(" << std::get<0>(v); 
												     std::cout << "," << std::get<1>(v) << "),"; 
												     return v;};
  map (showi, L);
  std::cout << std::endl;
  map (showc, M);
  std::cout << std::endl;
  std::forward_list<std::tuple<int, char>> H2 = zip (L, M);
  map(show,H2); 
  std::cout << std::endl;
  return 0;
}


