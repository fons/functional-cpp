#ifndef H__FUNCTOR__H
#define H__FUNCTOR__H


template <template<typename T1, typename... D> class F>
struct functor {

  //curried version
  template<typename A, typename B>
  static std::function < F<B> (F<A>)> fmap(std::function <B (A)> f);

  // uncurried, for functions..
  template<typename A, typename B>
  static F<B> fmap(std::function <B (A)> f, F<A> L) {
    return fmap(f)(L);
  }
};

//----------------------------- Implementation -----------------------
#include "map.hpp"
#include "unary_op.hpp"

template <>
struct functor<std::shared_ptr> {

  template<typename A, typename B>
  static std::function<std::shared_ptr<B> (std::shared_ptr<A>)>  fmap (std::function<B(A)> f) {
    return [f](std::shared_ptr<A> v) {
      if (v) {
	return std::make_shared<B>(f(*v)); 
      }
      return  std::shared_ptr<B>(nullptr);
    };
  }

  template<typename A, typename B, typename F>
  static std::function<std::shared_ptr<B> (std::shared_ptr<A>)>  fmap (F f) {
    return [f](std::shared_ptr<A> v) {
      if (v) {
	return std::make_shared<B>(f(*v)); 
      }
      return  std::shared_ptr<B>(nullptr);
    };
  }
};

template<> struct 
functor<std::forward_list> {
  template<typename A, typename B>
  static std::function < std::forward_list<B> (std::forward_list<A>)> fmap(std::function <B (A)> f) {
    return [f] (std::forward_list<A> L) {
      return map<A,B>(f,L);
    };
  };

  template<typename A, typename B>
  static std::forward_list<B> fmap(std::function <B (A)> f, std::forward_list<A> L) {
    return fmap(f)(L);
  };

  template<typename A, typename B, typename F>
  static std::forward_list<B> fmap(F f, std::forward_list<A> L) {
    return map<A,F>(f, L);
  };

};


template<> struct 
functor<std::list> {

  template<typename A, typename B>
  static std::function < std::list<B> (std::list<A>)> fmap(std::function <B (A)> f) {
    return [f] (std::list<A> L) {
      return map<A,B>(f,L);
    };
  };

  template<typename A, typename B>
  static std::list<B> fmap(std::function <B (A)> f, std::list<A> L) {
    return fmap(f)(L);
  };

  template<typename A, typename F>
  static auto fmap(F f, std::list<A> L) -> std::list<decltype(f(A()))> {
    return map<A,F>(f, L);
  };

};


template<>
struct functor<unary_op>
{

  template<typename A, typename B, typename R>
  static auto  fmap (std::function<B(A)> f) {  
    return [f](std::function<A(R)> g)  {
      return [f,g] (R x) {
	return f(g(x));
      };
    };
  };

  template<typename A, typename B, typename R>
  static std::function<B (R)> fmap (std::function<B(A)> f, std::function<A(R)> g) {  
    return [f,g](R x) -> B {
      return f(g(x));
    };
  };

};


#endif
