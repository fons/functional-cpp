#ifndef h__list_of_ptr__h
#define h__list_of_ptr__h

/*
  implementation of functors and applicative functors for a list of pointers
 */
template<typename A> using forward_list_of_ptr = std::forward_list<std::shared_ptr<A>>;


template <>
struct functor<forward_list_of_ptr> {

  template<typename A, typename B>
  static std::function<forward_list_of_ptr<B> (forward_list_of_ptr<A>)>  fmap (std::function<B(A)> f) {
	  auto F = functor<std::shared_ptr>::fmap(f);
	  return [F](forward_list_of_ptr<A> L) {
		  return functor<std::forward_list>::fmap(F, L);		  
	  };
  }

  template<typename A, typename B, typename F>
  static std::function<forward_list_of_ptr<B> (forward_list_of_ptr<A>)>  fmap (F f) {
	  auto F = functor<std::shared_ptr>::fmap<A,B>(f);
	  return [F](forward_list_of_ptr<A> L) {
		return functor<std::forward_list>::fmap(F, L);
    };
  }

};

template<> struct 
applicative_functor<forward_list_of_ptr> :public functor<forward_list_of_ptr>{

    template<typename A>
    static forward_list_of_ptr<A> pure(A v) {
		auto y = applicative_functor<std::shared_ptr>::pure<A>(v);
		return applicative_functor<std::forward_list>::pure<decltype(y)>(y);
    }
    
    template<typename A, typename B>
    static std::function< forward_list_of_ptr<B> (forward_list_of_ptr<A>)> apply(forward_list_of_ptr<std::function<B(A)>> F) {
		return [F](forward_list_of_ptr<A> L) {
			forward_list_of_ptr<B> acc;
			for (auto& func : F) {
				for (auto& arg : L) {
					auto res = applicative_functor<std::shared_ptr>::apply<A,B>(func)(arg);
					acc.push_front(res);
				}
			} 
			acc.reverse();
			return acc;
		};
	};

    template<typename A, typename B, typename lambda>
    static std::function< forward_list_of_ptr<B> (forward_list_of_ptr<A>)> apply(forward_list_of_ptr<lambda> F) {
		return [F](forward_list_of_ptr<A> L) {
			forward_list_of_ptr<B> acc;
			for (auto& func : F) {
				for (auto& arg : L) {
					auto res = applicative_functor<std::shared_ptr>::apply<A,B,lambda>(func)(arg);
					acc.push_front(res);
				}
			} 
			acc.reverse();
			return acc;
		};
	};
	
};

#endif
