#ifndef h__future_value_monad__h
#define h__future_value_monad__h

#include "future_value.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "monad.hpp"
#include "maybe.hpp"

template<>
struct functor<future_value> {

	//curried version
	template<typename Ret, typename Arg, typename lambda>
	static auto fmap(lambda f) {  // -> std::function < FutureValue<T, decltype(f(A()))> (future_value<T,A>)> {
		return [&f](future_value<Arg,Ret> e) {
			return fmap(f, e);
		};
	}
	
	// uncurried, for functions..
	
	template<typename Ret, typename Arg, typename lambda>
	static auto fmap(lambda f, future_value<Ret,Arg> e) -> future_value<decltype(f(Ret())), Arg> {
		return make_future_value(f) + e;
	};
};


template <> 
struct applicative_functor<future_value> : public functor <future_value>
{

	template <typename Ret, typename Arg> 
	static future_value<Ret, Arg>  pure(Ret val) {
		std::function<Ret(Arg)> F = [val] (Arg arg) {
			return val;
		};
		return make_future_value(F);
	}
	
	template <typename A, typename B, typename R>
	static future_value<R,A> apply(const future_value<std::function<R(B)>,A>& F, const future_value<B,A>& L) {
		std::function<R(A)> Func =  [F,L] (A x) {
			// this can be run in parallel..
			auto val = runFutureValue(L, x);
			auto rev = runFutureValue(F, x);
			if (val == NONE || rev.none()) {
				return R();
			}
			return (*rev)(*val);
		};
		return make_future_value(Func);
	};
	

	template <typename A, typename B, typename R>
	static auto apply(const future_value<std::function<R(B)>,A>& F) {
		return [F] (const future_value<B,A>& L) {
			return apply(F,L);
		};
	};
	

};

template<>
struct monad<future_value> : public applicative_functor<future_value> 
{
	template <typename Ret, typename Arg> 
	static future_value<Ret, Arg>  mreturn(Ret val) {
		return pure(val);
	};

	template<typename Ret, typename Arg, typename RetB>
	static future_value<RetB,Arg> bind(future_value<Ret,Arg>& F, std::function< future_value<RetB,Ret> (Ret)>& f) {

		std::function<RetB (Arg)> comp =[&F, &f](Arg x) {
			auto val = runFutureValue(F, x);
			if (val == NONE) {
				return RetB();
			}
			
			auto cal = runFutureValue(f(*val), *val);
			if (cal == NONE) {
				return RetB();
			}
			return *cal;
		};

		return make_future_value(comp);
	};

	
};


#endif
