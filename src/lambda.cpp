#include "proto.hpp"

int lambda_1()
{
  int x = 0;
  int y = 42;
  std::cerr << "hello  world" << std::endl;
  auto func = [x, &y] () { std::cout << "Hello world from lambda : " << x << "," << y << std::endl; };
  auto inc = [&y] () { y++; };
  auto inc_alt = [y]  () mutable { y++; };
  auto inc_alt_alt = [&]  () { y++; x++; };
  func();
  
  y = 900;
  func();
  inc();
  func();
  inc_alt();
  func();
  inc_alt_alt();
  func();//notice what gets outputted here !
  std::cout << " x :" << x << "; y :" << y << std::endl; 
  return 0;
  
}

int lambda_2()
{

  auto f = [] (int x) { return [=] (int y) {return x + y;};};
  std::cout <<  f(2)(3) << std::endl;
  return 0;
}

// (x,y) => (x)->(y) 
int lambda_3()
{
  std::function<int (int,int)> op = [] (int x, int y) { return x * y;};
  std::cout <<  op(5,2) << std::endl;
  auto f = [=] (int x) { return [=] (int y) {return op(x, y);};};
  std::cout <<  f(5)(2) << std::endl;

  return 0;
  
}


int lambda_4()
{
  std::function<int (int,int)> op = [] (int x, int y) { return x * y;};
  std::function<std::function<int (int)> (int)>l = [=] (int x) { return [=] (int y) {return op(x, y);};};
  std::cout << l(8)(9) << std::endl;
  return 0;
  
}

std::function<std::function<int (int)> (int)> cy (std::function<int (int,int)> op)
{
  return [=] (int x) { return [=] (int y) {return op(x, y);};};
}

int lambda_5 ()
{
  auto l = cy ([](int x, int y) { return x * y;});
  std::cout << l(8)(9) << std::endl;
  return 0;
  
}

template <typename T, typename U, typename R>
std::function<std::function<int (U)> (T)> curry (std::function<R (T,U)> op)
{
  return [=] (T x) { return [=] (U y) {return op(x, y);};};
}

int lambda_6 ()
{
  auto l = curry<int,int, int> ([](int x, int y) { return (5 + x) * y;});
  std::cout << l(1)(1) << std::endl;
  return 0;
}

int lambda_7 ()
{
  // as opposed to [=] or [] or [l]
  std::function<int (int)> l = [&l] (int x) ->int {
    std::cout << x << ",";
    if (x < 0) return 0;
    x--;
    return l(x);
    
  };
  l(25);
  return 0;
}

int lambda_8()
{
  auto l1 = [] (int x) {
    return 5*x;
  };

  auto l2 = [] (decltype(l1) f, int x) {
    return f(x);
  };

  std::cout << l2(l1, 67) << std::endl; 

  return 0;
}

//p f g x = f x (g x)
//p (\x-> (\y-> x + y-90)) (\x-> 2 * x) 6
//Prelude> :t p2
//p2 :: (t2 -> t1 -> t) -> (t2 -> t1) -> t2 -> t

template<typename A, typename B, typename C>
C p2 (std::function<C(A,B) > f, std::function<B(A)> g, const A& x)
{
  return f(x, g(x));
} 

template<typename A, typename B, typename C>
std::function<C(A)> p22 (std::function<C(A,B) > f, std::function<B(A)> g)
{
  return [&](A x) { return f(x, g(x)); };
} 

template<typename A, typename B, typename C>
std::function<std::function<C(A)> (std::function<B(A)>) > p2233 (std::function<C(A,B) > f)
{
  return [=](std::function<B(A)> g)->std::function<C(A)> {return [=](A x)->C { return f(x, g(x)); }; };
} 

int lambda_9()
{
  std::function<int(int, int)> f = [](int x, int y) {return x + y - 90;};
  std::function<int (int)>     g = [](int x) {return 2 * x ;};
  auto val = p2(f, g,  6);
  std::cout << "val : " << val << std::endl;

  auto F = p22 (f, g);
  auto val2 = F(6);
  std::cout << "val1 : " << val << "  ";
  std::cout << "val2 : " << val2 << " ";
  std::cout << "val3 : " << p2233(f)(g)(6) << " ";
  std::cout << std::endl;
  return 1;
}
