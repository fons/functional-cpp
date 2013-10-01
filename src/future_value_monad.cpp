#include "proto.hpp"
#include "show.hpp"
#include "future_value_monad.hpp"
#include "curry.hpp"
#include "bracket.hpp"

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
	static future_value<R,A> apply(future_value<std::function<R(B)>,A> F, future_value<B,A> L) {
		std::function<R(A)> Func =  [F,&L] (A x) {
			// this can be run in parallel..
			auto val = runFutureValue(L, x);
			auto rev = runFutureValue(F, x);
			//Need some error checking..
			return (*rev)(*val);
		};
		return make_future_value(Func);
	};
	
	/*
	template<typename T, typename A, typename lambda>
	static auto apply(future_value<T, lambda> F) {
		return [F] (future_value<T, A> m) {
			return apply(F,m);
		};
	};
	*/

};


int fvm_0()
{

	std::function<int(int)> f = [](int x) { 
		std::cerr << "start..." << std::endl;
		int n = 5;
		while (n--) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			std::cerr << ".";
		}
		std::cerr << std::endl;
		return x+45;
	};

	std::function<int(int)> g = [](int x) { 
		std::cerr << "start+++" << std::endl;
		int n = 5;
		while (n--) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			std::cerr << "?";
		}
		std::cerr << std::endl;
		return x+56;
	};

	future_value<int,int> fv = functor<future_value>::fmap(g, make_future_value(f)); 

	std::cerr << fv << std::endl;

	auto r0 = runFutureValue(fv, 45);
	std::cerr << r0 << std::endl;
	auto r1 = runFutureValue(fv, -23);
	std::cerr << r1 << std::endl;

	return 0;
}

int fvm_1()
{

	std::function<int(int)> f = [](int x) { 
		std::cerr << "start..." << std::endl;
		int n = 5;
		while (n--) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			std::cerr << ".";
		}
		std::cerr << std::endl;
		return x+45;
	};

	std::function<int(int)> g = [](int x) { 
		std::cerr << "start+++" << std::endl;
		int n = 5;
		while (n--) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			std::cerr << "?";
		}
		std::cerr << std::endl;
		return x+56;
	};

	std::function<future_value<int,int> (int, future_value<int,int>)> repeat = [&repeat,f,g] (int count, future_value<int,int> fv)  {
		if (count < 1) return fv;
		future_value<int,int> fv1 = functor<future_value>::fmap(g, fv); 
		future_value<int,int> fv2 = functor<future_value>::fmap(g, fv1); 
		future_value<int,int> fv3 = functor<future_value>::fmap(f, fv2); 
		return repeat(count-1, fv3);
	};
	
	future_value<int,int> fv0 = functor<future_value>::fmap(g, make_future_value(f)); 
	future_value<int,int> fv1 = repeat(10, fv0);
	
	auto r0 = runFutureValue(fv1, 45);
	std::cerr << r0 << std::endl;

	return 0;
}

int fvm_2()
{

	auto fv = applicative_functor<future_value>::pure<int,int>(4500);

	auto r0 = runFutureValue(fv, 45);
	std::cerr << r0 << std::endl;
	
	std::function<int(int)> func =[](int x) { return 5*x + 34;};
	
	auto fw = applicative_functor<future_value>::pure<decltype(func), int>(func);

	auto r1 = runFutureValue(fw, 45);

	std::cerr << r1 << " ----> " << (*r1)(70) << std::endl;

	return 0;
}

int fvm_3()
{

	std::function<int(int)> func1 =[](int x) { return 5*x + 34;};
	std::function<int(int)> func2 =[](int x) { return 5-x;};
	//future_value<std::function<R(B)>,A>
	future_value<std::function<int(int)>, int> fw = applicative_functor<future_value>::pure<decltype(func1), int>(func1);
	std::cerr << fw << std::endl;
	auto ty = runFutureValue(fw, 67);
	std::cerr << ty << std::endl;
	
	future_value<int,int> fvv(func2);	
	std::cerr << fvv << std::endl;

	auto r1 = runFutureValue(fvv, 45);
	std::cerr << r1 << std::endl;

	auto fv = applicative_functor<future_value>::apply<int,int,int>(fw, fvv);
	auto r0 = runFutureValue(fv, 45);
	std::cerr << r0 << std::endl;

	return 0;
}
int fvm_4()
{


	return 0;
}
int fvm_5()
{


	return 0;
}
