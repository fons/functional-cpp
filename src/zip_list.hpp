#ifndef h__ziplist__h
#define h__ziplist__h

template<typename A> using zip_list = std::forward_list<A>;

template <>
struct functor<zip_list> {

  template<typename A, typename B>
  static std::function<zip_list<B> (zip_list<A>)>  fmap (std::function<B(A)> f) {
	  return [f](zip_list<A> L) {
		  return functor<std::forward_list>::fmap<A,B>(f, L);		  
	  };
  }


  template<typename A, typename B, typename F>
  static std::function<zip_list<B> (zip_list<A>)>  fmap (F f) {
	  return [f](zip_list<A> L) {
		  return functor<std::forward_list>::fmap<A,B,F>(f, L);
	  };
  };

};

template<> struct 
applicative_functor<zip_list> :public functor<zip_list>{

    template<typename A>
    static zip_list<A> pure(A v) {
		return applicative_functor<std::forward_list>::pure<decltype(v)>(v);
    }

    template<typename A, typename B>
    static std::function< zip_list<B> (zip_list<A>)> apply(zip_list<std::function<B(A)>> F) {
		return [F](zip_list<A> L) {
			zip_list<B> acc;
			auto it1 = F.begin();
			auto it2 = L.begin();
			while (it1 != F.end() && it2 != L.end()) {
				auto func = *it1;
				auto arg  = *it2;
				acc.push_front(func(arg));
				it1++;
				it2++;
			}
			acc.reverse();
			return acc;
		};
	};

    template<typename A, typename B, typename lambda>
    static std::function< zip_list<B> (zip_list<A>)> apply(zip_list<lambda> F) {
		return [F](zip_list<A> L) {
			zip_list<B> acc;
			auto it1 = F.begin();
			auto it2 = L.begin();
			while (it1 != F.end() && it2 != L.end()) {
				auto func = *it1;
				auto arg  = *it2;
				acc.push_front(func(arg));
				it1++;
				it2++;
			}
			acc.reverse();
			return acc;
		};
	};
	
};

#endif
