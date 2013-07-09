#ifndef h__forwardziplist__h
#define h__forwardziplist__h

template<typename A> using forward_zip_list = std::forward_list<A>;

template <>
struct functor<forward_zip_list> {

  template<typename A, typename B>
  static std::function<forward_zip_list<B> (forward_zip_list<A>)>  fmap (std::function<B(A)> f) {
	  return [f](forward_zip_list<A> L) {
		  return functor<std::forward_list>::fmap<A,B>(f, L);		  
	  };
  }


  template<typename A, typename B, typename F>
  static std::function<forward_zip_list<B> (forward_zip_list<A>)>  fmap (F f) {
	  return [f](forward_zip_list<A> L) {
		  return functor<std::forward_list>::fmap<A,B,F>(f, L);
	  };
  };

};

template<> struct 
applicative_functor<forward_zip_list> :public functor<forward_zip_list>{

    template<typename A>
    static forward_zip_list<A> pure(A v) {
		return applicative_functor<std::forward_list>::pure<decltype(v)>(v);
    }

    template<typename A, typename B>
    static std::function< forward_zip_list<B> (forward_zip_list<A>)> apply(forward_zip_list<std::function<B(A)>> F) {
		return [F](forward_zip_list<A> L) {
			forward_zip_list<B> acc;
			auto it1 = F.begin();
			auto it2 = L.begin();
			while (it1 != F.end() && it2 != L.end()) {
				std::cerr << "processing.." << std::endl;
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
    static std::function< forward_zip_list<B> (forward_zip_list<A>)> apply(forward_zip_list<lambda> F) {
		return [F](forward_zip_list<A> L) {
			forward_zip_list<B> acc;
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
