#include "proto.hpp"
#include "show.hpp"
#include "either_monad.hpp"
#include "curry.hpp"
#include "bracket.hpp"

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
			return Either<std::string, int>::Left(std::string("smaller than 0"));
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
	std::function< Either<std::string, int> (int)> f = [](int x) {
		if (x < 0) {
			return Either<std::string, int>::Left(std::string("smaller than 0"));
		}
		return Either<std::string, int>::Right(1.15*x-10);;
	};
	auto e  = Either<std::string, int>::Right(-45);
	auto r  = monad<Either>::bind<std::string, int, decltype(f)>(e)(f);
	std::cerr << e << "  ->  " << r << std::endl;
	return 0;
}


int eim_4()
{
	auto f = std::plus<int>();

	std::function<std::function<int(int)> (int)> cf = 	curry<decltype(f),int,int>(f);
	std::cerr << " 23 + 45 = " << f(23,45) << std::endl;
	std::cerr << " 23 + 45 = " << cf(23)(45) << std::endl;

	auto F   = applicative_functor<Either>::pure<std::string>(cf);
	auto V1  = applicative_functor<Either>::pure<std::string>(23);
	auto V2  =  applicative_functor<Either>::pure<std::string>(45);
	std::cerr << V1 << " " << V2 << std::endl;  
	
	auto A1  = applicative_functor<Either>::apply(F,  V1);
	auto A2  = applicative_functor<Either>::apply(A1, V2);
	std::cerr << A2 << std::endl;  
	return 0;
}
