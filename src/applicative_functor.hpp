#ifndef h__applicative_functor__h
#define h__applicative_functor__h

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
		return [f](std::shared_ptr<A> v) {
			if (v && f) {
				auto F = *f;
				return pure (F(*v)); 
			}
			return  std::shared_ptr<B>(nullptr);
		};
    }

	template<typename A, typename B, typename lambda>
	static std::function< std::shared_ptr<B> (std::shared_ptr<A> v)> apply(std::shared_ptr<lambda> f) {
		return [f](std::shared_ptr<A> v) {
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
		return std::forward_list<A>(1,v);
	}
	
	template<typename A, typename B>
	static std::function< std::forward_list<B> (std::forward_list<A>)> apply(std::forward_list<std::function<B(A)>> F) {
		return [F](std::forward_list<A> L) {
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
		return [F](std::forward_list<A> L) {
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
struct applicative_functor <unary_op> : public functor<unary_op>
{
	
	template <typename A, typename B=A> 
	static std::function<B(A)> pure(B val) {
		return [val] (A x) {
			return val;
		};
	};
	
	template <typename A, typename B, typename R>
	static std::function<R(A)> apply(std::function < R (A,B)> f, std::function<B(A)> g) {
		return [f,g] (A x) {
			return f(x, g(x));
		};
	};
	
	template <typename A, typename B, typename R>
	static std::function<R(A)> apply(std::function< std::function<R(B)>(A) > f, std::function<B(A)> g) {
		return [f,g] (A x) {
			return f(x)(g(x));
		};
	};
	
	template <typename A, typename B, typename R>
	static std::function< std::function<R(A)> (std::function<B(A)>)> apply(std::function < R (A,B)> f) {
		return [f] (std::function<B(A)> g) -> std::function<R(A)> {
			std::function<B(A)> G(g);
			return [f,G] (A x) {
				return f(x, G(x));
			};
		};
	};
	
};

template<> struct 
applicative_functor<std::list> :public functor<std::list>{
	
	template<typename A>
	static std::list<A> pure(A v) {
		std::list<A> L;
		L.push_front(v);
		return L;
	}
  
  template<typename A, typename B>
  static std::function< std::list<B> (std::list<A>)> apply(std::list<std::function<B(A)>> F) {
      return [F](std::list<A> L) {
		  std::list<B> acc;
		  for (auto& func : F) {
			  for (auto& arg : L) {
				  acc.push_back(func(arg));
			  }
		  } 
		  return acc;
      };
  };

	
	template<typename A, typename B, typename lambda>
	static std::function< std::list<B> (std::list<A>)> apply(std::list<lambda> F) {
		return [F](std::list<A> L) {
			std::list<B> acc;
			for (auto& func : F) {
				for (auto& arg : L) {
					acc.push_back(func(arg));
				}
			} 
			return acc;
		};
    };
	
};

template<>
struct applicative_functor <raw_pointer> : public functor<raw_pointer>
{

	template<typename A>
	static A* pure(A val) {
		return new A(val);
	}

	
	template<typename A, typename B>
	static std::function<B*(A*)> apply(std::function<B(A)>* f) {
		return [f](A* v) {
			if (v && f) {
				auto F = *f;
				return pure (F(*v)); 
			}
			return  static_cast<B*>(nullptr);
		};
	}

	template<typename A, typename B>
	static B* apply(std::function<B(A)>* f, A* v) {
		return apply(f)(v);
	}
	
	template<typename A, typename B, typename lambda>
	static std::function<B* (A*)> apply(lambda* f) {
		return [f](A* v) {
			if (v && f) {
				auto F = *f;
				return pure (F(*v)); 
			}
			return static_cast<B*>(nullptr);
	   };
	};

	template<typename A, typename B, typename lambda>
	static B*  apply(lambda* f, A* v) {
		return apply<A,B,lambda>(f)(v);
	}	
};



#endif
