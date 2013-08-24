#ifndef H__MONAD__H
#define H__MONAD__H
#include "applicative_functor.hpp"

template <template<typename T1, typename... D> class F> 
struct monad : public applicative_functor <F>
{
	template <typename A> static F<A> mreturn (A val) {
		return applicative_functor<F>::pure(val);
	}

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
			res.reverse();
			for (auto& list : res) {
				R.splice_after(R.before_begin(), list);//concatenate 
			}
			return R;
		};
	}

	template <typename A> static std::forward_list<A> mreturn (A val) {
		return applicative_functor<std::forward_list>::pure(val);
	}

};

//---------------------------------------------------------------------------------------------------------
//
//         std::list
//

template<> struct monad<std::list> : public applicative_functor<std::list> {

	template<typename A, typename B>
	static std::function < std::list<B> (std::function< std::list<B> (A) > ) > bind(std::list<A> M) {
		return [M](std::function<std::list<B> (A)> f) {
			std::list<B> R;
			std::list<std::list<B>> res = map(f, M);
			for (auto& list : res) {
				R.insert(R.end(), list.begin(), list.end());
			}
			return R;
		};
	}

	template <typename A> static std::list<A> mreturn (A val) {
		return applicative_functor<std::list>::pure(val);
	}

};
//---------------------------------------------------------------------------------------------------------
//
//         std::shared_ptr
//

template<> struct monad<std::shared_ptr> : public applicative_functor<std::shared_ptr> {

	template<typename A, typename B>
	static std::function < std::shared_ptr<B> (std::function< std::shared_ptr<B> (A) > ) > bind(std::shared_ptr<A> M) {
		return [M](std::function<std::shared_ptr<B> (A)> f) {
			if (M) {
				return f(*M);
			}
			return  std::shared_ptr<B>();
		};
	}

	template <typename A> static std::shared_ptr<A> mreturn (A val) {
		return applicative_functor<std::shared_ptr>::pure(val);
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
	
	template <typename A, typename F, typename R>
	static auto bind(F h, R f) {
		return [f,h] (A x) {
			return f(h(x), x);
		};
	};
	
	
};

#endif
