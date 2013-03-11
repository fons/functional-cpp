#ifndef H__MONAD_IMPL__H
#define H__MONAD_IMPL__H
#if 0
template<> struct monad<std::forward_list> : public applicative_functor<std::forward_list> {
  
  template<typename A, typename B>
  static std::function < std::forward_list<B> (std::function< std::forward_list<B> (A) > ) > bind(std::forward_list<A> M) {
    return [=](std::function<std::forward_list<B> (A)> f) {
      std::forward_list<B> R;
      std::forward_list<std::forward_list<B>> res = map(f, M);
      for (auto& list : res) {
      	R.splice_after(R.before_begin(), list);//concatenate 
      }
      return R;
    };
  }

};
#endif
#endif
