#include "proto.hpp"
#include "mpc.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "show.hpp"
#include "curry.hpp"

plist_t<char> item_f(std::string s) {
  if (s.length() == 0) {
    return plist_t<char>();
  }
  return plist_t<char>({ppair_t<char>(s.front(), s.substr(1))});
}

int mpc_1()
{
  ppair_t<char> y ('c', "hello");
  std::cout << y << std::endl;
  plist_t<char> L = {y};
  L.push_back(ppair_t<char>('p', "done"));
  std::cout << L << std::endl;
  parser_t<char> F = [=](std::string s) { return L;};
  std::cout << F << std::endl;

  Parser<char> P(F);
  std::cout << P << std::endl;
  
  auto res = parse (P)("hello");
  std::cout << "->" << res << std::endl;
  return 0;
}

int mpc_2()
{
  
  Parser <char> item(item_f);
  auto res = parse (item) ("hello");
  std::cout << res << std::endl;
  auto res1 = parse (item) ("");
  std::cout << res1 << std::endl;
  return 0;
}

int mpc_3 ()
{
  std::function<char(char)> idc = [] (char v) { return v;};

  Parser <char> item(item_f);
  auto res = parse (item) ("hello");
  std::cout << res << std::endl;

  auto res1 = parse (functor<Parser>::fmap(idc)(item)) ("hello");
  std::cout << res1 << std::endl;

  return 1;
}

template<>
struct applicative_functor <Parser> : public functor<Parser>
{
  
  template <typename A, typename B=A> 
  static Parser<A> pure(A val) {
    parser_t<A> rb = [=](std::string s) {
      ppair_t<A> y (val, s);
      plist_t<A> L = {y};
      return L;
    };
    return Parser<A>(rb);
  };

  template<typename A, typename B, typename lambda>
  static std::function< Parser<B> (Parser<A>)> apply(Parser<lambda> F) {
      return [=](Parser<A> L) {
	parser_t<B> rb = [=] (std::string s) {
	  auto ll = parse(F)(s);
	  //we assume only one element; s/b generalized !
	  ppair_t<lambda> el = ll.front();
	  //std::cout << " el 1 : " << std::get<1>(el) << std::endl;
	  //std::cout << " el 0 : " << std::get<0>(el)('r') << std::endl;
	  auto la = parse(L)(std::get<1>(el));
	  //std::cout << "la : " << la << std::endl;
	  std::function< ppair_t<B> (ppair_t<A>)> lab = [=] (ppair_t<A> v) {
	    return ppair_t<B>(std::get<0>(el) (std::get<0>(v)), std::get<1>(v));
	  };
	  auto resb = functor<std::list>::fmap(lab)(la);
	  //std::cout << "resb : " << resb << std::endl;
	  return resb;
	};

	return Parser<B>(rb);
      };
    };
};

int mpc_4()
{
  auto j = applicative_functor<Parser>::pure(1);
  std::cout << j << std::endl;
  auto res = parse(j)("hello"); 
  std::cout << res << std::endl;
  return 0;
}

int mpc_5()
{
  Parser <char> item(item_f);
  typedef char T1;
  auto f = [=] (T1 c){ return std::make_tuple(c);};
  typedef  decltype(f(T1()))  ret_t;
  auto ll1 = applicative_functor<Parser>::pure(f);
  auto ap1 = applicative_functor<Parser>::apply<T1, ret_t>(ll1)(item);
  auto res1 = parse(ap1)("hello");
  std::cout << res1 << std::endl;
  return 0;
}

int mpc_6()
{
  Parser <char> item(item_f);

  typedef char T1;
  typedef char T2;

  auto ff  = [=] (T1 a, T2 b){ return std::make_tuple(a,b);};

  typedef decltype(ff(T1(), T2())) ret_t;

  auto cff = curry<decltype(ff), T1, T2>(ff);
  auto cffl = applicative_functor<Parser>::pure(cff);
  auto C    = applicative_functor<Parser>::apply<T1, decltype(cff(T1())) >(cffl)(item);
  auto J    = applicative_functor<Parser>::apply<T2, ret_t>(C)(item);
  auto res1 = parse(J)("hello");
  std::cout << res1 << std::endl;
  return 0;
}
