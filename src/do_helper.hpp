#ifndef h__do_helper__h
#define h__do_helper__h
template <template<typename Tx, typename... D> class F> 
struct do_helper {

};

//------------------------------------------------------
template <> 
struct do_helper<std::forward_list> {

  template<typename lambda, typename T1>
  static auto dofor(std::forward_list<T1> L) {
    return [=] (lambda f) {
      auto cffl = applicative_functor<std::forward_list>::pure(f);
      auto C    = applicative_functor<std::forward_list>::apply<T1, decltype(f(T1())) >(cffl)(L);
      return C;
    };
  };

  template<typename lambda, typename T1>
  static auto dofor(std::forward_list<T1> L, lambda f) {
      auto cffl = applicative_functor<std::forward_list>::pure(f);
      auto C    = applicative_functor<std::forward_list>::apply<T1, decltype(f(T1())) >(cffl)(L);
      return C;
  };

  template<typename lambda, typename T1, typename T2>
  static auto dofor(std::forward_list<T1> L1, std::forward_list<T2> L2) { 
    return [=] (lambda ff) {
      auto cff = curry<decltype(ff), T1, T2>(ff);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::forward_list>::apply<T2, decltype(cff(T1())(T2()))>(C)(L2);
      return J;
    };
  };
  template<typename lambda, typename T1, typename T2>
  static auto dofor(std::forward_list<T1> L1, std::forward_list<T2> L2, lambda ff) { 
    typedef decltype(ff(T1(), T2())) ret_t;
      auto cff = curry<decltype(ff), T1, T2>(ff);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::forward_list>::apply<T2, ret_t>(C)(L2);
      return J;
  };

  template<typename lambda, typename T1, typename T2, typename T3>
  static auto dofor(std::forward_list<T1> L1, std::forward_list<T2> L2, std::forward_list<T3> L3) { 
    return [=] (lambda f) {
      typedef decltype(f(T1(), T2(), T3())) ret_t;
      auto cff  = curry<decltype(f), T1, T2, T3>(f);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::forward_list>::apply<T2, decltype(cff(T1())(T2()))>(C)(L2);
      auto K    = applicative_functor<std::forward_list>::apply<T3, ret_t>(J)(L3);
      return K;
    };
  };

 template<typename lambda, typename T1, typename T2, typename T3>
 static auto dofor(std::forward_list<T1> L1, std::forward_list<T2> L2, std::forward_list<T3> L3, lambda f) { 
      typedef decltype(f(T1(), T2(), T3())) ret_t;
      auto cff  = curry<decltype(f), T1, T2, T3>(f);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::forward_list>::apply<T2, decltype(cff(T1())(T2()))>(C)(L2);
      auto K    = applicative_functor<std::forward_list>::apply<T3, ret_t>(J)(L3);
      return K;
  };
};

//------------------------------------------------------
template <> 
struct do_helper<std::shared_ptr> {

  template<typename lambda, typename T1>
  static auto dofor(std::shared_ptr<T1> L) {
    return [=] (lambda f) {
      auto cffl = applicative_functor<std::shared_ptr>::pure(f);
      auto C    = applicative_functor<std::shared_ptr>::apply<T1, decltype(f(T1())) >(cffl)(L);
      return C;
    };
  };

  template<typename lambda, typename T1, typename T2>
  static auto dofor(std::shared_ptr<T1> L1, std::shared_ptr<T2> L2) { 
    return [=] (lambda ff) {
      auto cff = curry<decltype(ff), T1, T2>(ff);
      //auto C    = dofor<decltype(cff)>(L1)(cff);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::shared_ptr>::apply<T2, decltype(cff(T1())(T2()))>(C)(L2);
      return J;
    };
  };

  template<typename lambda, typename T1, typename T2, typename T3>
  static auto dofor(std::shared_ptr<T1> L1, std::shared_ptr<T2> L2, std::shared_ptr<T3> L3) { 
    return [=] (lambda f) {
      typedef decltype(f(T1(), T2(), T3())) ret_t;
      auto cff  = curry<decltype(f), T1, T2, T3>(f);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::shared_ptr>::apply<T2, decltype(cff(T1())(T2()))>(C)(L2);
      auto K    = applicative_functor<std::shared_ptr>::apply<T3, ret_t>(J)(L3);
      return K;
    };
  };


  template<typename lambda, typename T1>
  static auto dofor(std::shared_ptr<T1> L, lambda f) {
      auto cffl = applicative_functor<std::shared_ptr>::pure(f);
      auto C    = applicative_functor<std::shared_ptr>::apply<T1, decltype(f(T1())) >(cffl)(L);
      return C;
  };

  template<typename lambda, typename T1, typename T2>
  static auto dofor(std::shared_ptr<T1> L1, std::shared_ptr<T2> L2, lambda f) { 
      typedef decltype(f(T1(), T2())) ret_t;
      auto cff = curry<decltype(f), T1, T2>(f);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::shared_ptr>::apply<T2, ret_t>(C)(L2);
      return J;
  };

  template<typename lambda, typename T1, typename T2, typename T3>
  static auto dofor(std::shared_ptr<T1> L1, std::shared_ptr<T2> L2, std::shared_ptr<T3> L3, lambda f) { 
      typedef decltype(f(T1(), T2(), T3())) ret_t;
      auto cff  = curry<decltype(f), T1, T2, T3>(f);
      auto C    = dofor<decltype(cff)>(L1)(cff);
      auto J    = applicative_functor<std::shared_ptr>::apply<T2, decltype(cff(T1())(T2()))>(C)(L2);
      auto K    = applicative_functor<std::shared_ptr>::apply<T3, ret_t>(J)(L3);
      return K;
  };

};

using do_list = do_helper<std::forward_list>;
using do_sp   = do_helper<std::shared_ptr>;


#endif
