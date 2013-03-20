#ifndef __applicative_functor__
#define __applicative_functor__

#include "functor.hpp"

template <template<typename T1, typename... D> class F> 
struct applicative_functor : public functor <F>
{

  template <typename A> 
  static F<A> pure(A val);

  template<typename A, typename B>
  static std::function < F<B> (F<A>)> apply(F <std::function<B(A)>> f );
};

template<>
struct applicative_functor <std::shared_ptr> : public functor<std::shared_ptr>
{

  template<typename A>
  static std::shared_ptr<A> pure(A val) {
    return std::make_shared<A>(val);
  }
  
  template<typename A, typename B>
  static std::function< std::shared_ptr<B> (std::shared_ptr<A> v)> apply(std::shared_ptr<std::function<B(A)>> f) {
      return [=](std::shared_ptr<A> v) {
	if (v && f) {
	  auto F = *f;
	  return pure (F(*v)); 
	}
	return  std::shared_ptr<B>(nullptr);
      };
    }
    
};


template<> struct 
applicative_functor<std::forward_list> :public functor<std::forward_list>{

  template<typename A>
  static std::forward_list<A> pure(A v) {
    std::forward_list<A> L;
    L.push_front(v);
    return L;
  }
  
  template<typename A, typename B>
  static std::function< std::forward_list<B> (std::forward_list<A>)> apply(std::forward_list<std::function<B(A)>> F) {
      return [=](std::forward_list<A> L) {
	std::forward_list<B> acc;
	for (auto& func : F) {
	  for (auto& arg : L) {
	    acc.push_front(func(arg));
	  }
	 } 
	acc.reverse();
	return acc;
      };
    };

  template<typename A, typename B, typename lambda>
  static std::function< std::forward_list<B> (std::forward_list<A>)> apply(std::forward_list<lambda> F) {
      return [=](std::forward_list<A> L) {
	std::forward_list<B> acc;
	for (auto& func : F) {
	  for (auto& arg : L) {
	    acc.push_front(func(arg));
	  }
	 } 
	acc.reverse();
	return acc;
      };
    };

};

template<>
struct applicative_functor <binary_op> : public functor<binary_op>
{
  
  template <typename A, typename B=A> 
  static std::function<B(A)> pure(B val) {
    return [=] (A x) {
      return val;
    };
  };

  template <typename A, typename B, typename R>
  static std::function<R(A)> apply(std::function < R (A,B)> f, std::function<B(A)> g) {
    return [=] (A x) {
      return f(x, g(x));
    };
  };

  

  template <typename A, typename B, typename R>
  static std::function<R(A)> apply(std::function< std::function<R(B)>(A) > f, std::function<B(A)> g) {
    return [=] (A x) {
      return f(x)(g(x));
    };
  };



  template <typename A, typename B, typename R>
  static std::function< std::function<R(A)> (std::function<B(A)>)> apply(std::function < R (A,B)> f) {
    return [&] (std::function<B(A)> g) -> std::function<R(A)> {
      std::function<B(A)> G(g);
      return [&] (A x) {
	return f(x, G(x));
      };
    };
};

};

#endif
