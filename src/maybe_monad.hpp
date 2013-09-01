#ifndef h__maybe_monad__h
#define h__maybe_monad__h
#include "maybe.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "monad.hpp"

template<>
struct functor<Maybe> {

	template<typename A, typename lambda>
	static auto fmap(lambda f) ->std::function<Maybe<decltype(f(A()))> (Maybe<A>) > {
		return [f](Maybe<A> m) -> Maybe<decltype(f(A()))> {
			if (m == m.None()) {
				return Maybe<decltype(f(A()))>::None();
			}
			return just(f(*m));
		};
	}

	template<typename A, typename lambda>

	static auto retval(lambda f, Maybe<A> m) ->  Maybe<decltype(f(A()))>;

	template<typename A, typename lambda>
	static auto fmap(lambda f, Maybe<A> m) -> Maybe<decltype(f(A()))> {
		return fmap<A,lambda>(f)(m);
	}

};

template<>
struct applicative_functor<Maybe> : public functor <Maybe>
{
	template <typename A> 
	static Maybe<A> pure(A val) {
		return just(val);
	};
	
	template<typename A, typename lambda>
	static auto apply(Maybe<lambda> F , Maybe<A> m) -> decltype(functor<Maybe>::retval(*F, m)) { 
		// I can't call F.None() because lambda's don't have a default constructor..;
		//std::function<typename decltype(functor<Maybe>::retval(*F, m))::value_type (A)> ff = *F;
		return functor<Maybe>::fmap(*F, m);
	};

	template<typename A, typename lambda>
	static auto apply(Maybe<lambda> F) -> std::function<decltype(functor<Maybe>::retval(*F, Maybe<A>::Just(A()))) (Maybe<A>)> {
		return [F] (Maybe<A> m) {
			return apply(F,m);
		};
	};
};

template <> 
struct monad<Maybe> : public applicative_functor <Maybe>
{
	template <typename A> 
	static Maybe<A> mreturn (A val) {
		return applicative_functor<Maybe>::pure(val);
	}


	template<typename A, typename lambda>
	static auto bind(Maybe<A> m) {
		return [&m](lambda F) {
			if (m == m.None()) {
				//constructors are private so I do this in a bit of a rond-about way.
				return Maybe<typename decltype(F(A()))::value_type>::None();
			}
			return F(*m);		
		};
	};

	template<typename A, typename lambda>
	static auto bind(Maybe<A> m, lambda F) ->  decltype(Maybe<typename decltype(F(A()))::value_type>::None()) {
		return bind<A,lambda>(m)(F);
	};
};

#endif
