#ifndef H__MONAD__H
#define H__MONAD__H
#include "applicative_functor.hpp"


template <template<typename T1, typename... D> class F> 
struct monad : public applicative_functor <F>
{

  template<typename A, typename B>
  static std::function < F<B> (std::function< F<B> (A) > ) > bind(F<A> val);

  
};
//--------------------------------------- monad implementation-------------------------------
//
//         std::forward_list
//

template<> struct monad<std::forward_list> : public applicative_functor<std::forward_list> {
  template<typename A, typename B>
  static std::function < std::forward_list<B> (std::function< std::forward_list<B> (A) > ) > bind(std::forward_list<A> M) {
    return [M](std::function<std::forward_list<B> (A)> f) {
      std::forward_list<B> R;
      std::forward_list<std::forward_list<B>> res = map(f, M);
      for (auto& list : res) {
      	R.splice_after(R.before_begin(), list);//concatenate 
      }
      return R;
    };
  }

};

//-----------------------------------------------------------------------------------------
// 
//  unary functions
//
template<>
struct monad<unary_op> : public applicative_functor<unary_op> {

  template <typename A, typename B, typename R>
  static std::function<R(A)> bind(std::function<B(A)> h, std::function < R (A,B)> f) {
    return [f,h] (A x) {
      return f(h(x), x);
    };
  };
  
};

#endif
