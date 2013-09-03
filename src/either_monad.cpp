#include "proto.hpp"
#include "show.hpp"
#include "either_monad.hpp"

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
	
	//template<typename A, typename B>
	//static std::function < F<B> (std::function< F<B> (A) > ) > bind(F<A> val);
	template<typename T, typename A, typename lambda> 
	static auto bind(Either<T,A> e, lambda F) {
		if (e.Left()) {
			return Either<T,typename decltype(F(A()))::right_value_type>::Left(e.left());
		}
		return F(e.right());
	}
};

//////////////////////////////////////////////////////
int eim_0()
{
	auto v1 = Either<std::string, int>::Right(45);
	auto v2 = Either<std::string, int>::Left("a value");
	auto l = [](int x) {
		return 2+x;
	};
	std::cerr << v1 << " --> " << functor<Either>::fmap(l, v1) << std::endl;
	std::cerr << v2 << " --> " << functor<Either>::fmap(l, v2) << std::endl;
	return 0;
}

int eim_1()
{
	auto v1 = Either<std::string, int>::Right(45);
	auto v2 = Either<std::string, int>::Left("a value");

	auto l = [](int x) {
		return 2+x*3;
	};

	auto ll = applicative_functor<Either>::pure<std::string>(l);
	std::cerr << v1 << " --> " << applicative_functor<Either>::apply(ll, v1) << std::endl;
	std::cerr << v2 << " --> " << functor<Either>::fmap(l, v2) << std::endl;

	return 0;
}

int eim_2()
{
	std::function< Either<std::string, int> (int)> f = [](int x) {
		if (x < 0) {
			return Either<std::string, int>::Left("smaller than 0");
		}
		return Either<std::string, int>::Right(1.15*x-10);;
	};

	std::function< Either<std::string,int> (int, Either<std::string, int>)> repeat = [&](int n, Either<std::string, int> e) {
		if ( n == 0) return e;
		std::cerr << n << " : " << e << std:: endl << " -> ";
		return repeat(n-1, monad<Either>::bind(e,f));
	}; 
	auto v  = repeat(12, Either<std::string, int>::Right(45));
	std::cerr << v << std::endl;
	return 0;
}

int eim_3()
{
	return 0;
}



