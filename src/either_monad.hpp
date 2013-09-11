#ifndef h__either_monad__h
#define h__either_monad__h
#include "either.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "monad.hpp"

template <>
struct functor<Either> {

	//curried version
	template<typename T, typename A, typename lambda>
	static auto fmap(lambda f) -> std::function < Either<T, decltype(f(A()))> (Either<T,A>)> {
		return [&f](Either<T,A> e) {
			return fmap(f, e);
		};
	}
	
	// uncurried, for functions..
	
	template<typename T, typename A, typename lambda>
	static auto fmap(lambda f, Either<T,A> e) -> Either<T, decltype(f(A()))> {
		if (e.Left()) {
			return Either<T, decltype(f(A()))>::Left(e.left());
		} 
		return Either<T, decltype(f(A()))>::Right(f(e.right()));
	};
};

template <> 
struct applicative_functor<Either> : public functor <Either>
{

	template <typename T, typename A> 
	static Either<T,A> pure(A val) {
		return Either<T,A>::Right(val);
	}

	template<typename T, typename A, typename lambda>
	static auto apply(Either<T, lambda> F , Either <T, A> m) {
		return functor<Either>::fmap(F.right(), m);
	};

	template<typename T, typename A, typename lambda>
	static auto apply(Either<T, lambda> F) {
		return [F] (Either<T, A> m) {
			return apply(F,m);
		};
	};

};

template <>
struct monad<Either> : public applicative_functor <Either>
{
	template <typename T, typename A> 
	static Either<T, A> mreturn (A val) {
		return applicative_functor<Either>::pure<T,A>(val);
	}
	
	template<typename T, typename A, typename lambda>
	static auto bind(Either<T,A> e) {
		return [e](lambda F) {
			return bind(e, F);
		};
	}

	template<typename T, typename A, typename lambda> 
	static auto bind(Either<T,A> e, lambda F) {
		if (e.Left()) {
			return Either<T,typename decltype(F(A()))::right_value_type>::Left(e.left());
		}
		return F(e.right());
	}
};

#endif
