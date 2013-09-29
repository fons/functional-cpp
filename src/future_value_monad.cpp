#include "proto.hpp"
#include "show.hpp"
#include "future_value_monad.hpp"
#include "curry.hpp"
#include "bracket.hpp"
#if 0
template<>
struct functor<future_value> {

	//curried version
	template<typename Arg, typename Ret, typename lambda>
	static auto fmap(lambda f) {  // -> std::function < FutureValue<T, decltype(f(A()))> (future_value<T,A>)> {
		return [&f](future_value<Arg,Ret> e) {
			return fmap(f, e);
		};
	}
	
	// uncurried, for functions..
	
	template<typename Arg, typename Ret, typename lambda>
	static auto fmap(lambda f, future_value<Arg,Ret> e) -> future_value<Arg, decltype(f(Ret()))> {
		return make_future_value(f) + e;
	};
};


template <> 
struct applicative_functor<future_value> : public functor <future_value>
{

	template <typename Arg, typename Ret> 
	static future_value<Arg, Ret>  pure(Ret val) {
		std::function<Ret(Arg)> F = [val] (Arg arg) {
			return val;
		};
		return make_future_value(F);
	}

	template<typename Arg, typename Ret, typename lambda>
	static auto apply(future_value<Ret, lambda> F , future_value <Arg, Ret> m) {
		return F + m;
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
#endif

int fvm_0()
{
#if 0
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
#endif
	return 0;
}

int fvm_1()
{
#if 0
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

#endif
	return 0;
}

int fvm_2()
{
#if 0
	auto fv = applicative_functor<future_value>::pure<int,int>(4500);

	auto r0 = runFutureValue(fv, 45);
	std::cerr << r0 << std::endl;
	
	std::function<int(int)> func =[](int x) { return 5*x + 34;};
	
	auto fw = applicative_functor<future_value>::pure<int,decltype(func)>(func);

	auto r1 = runFutureValue(fw, 45);
	std::cerr << r1 << std::endl;

#endif

	return 0;
}

int fvm_3()
{
#if 0
	std::function<int(int)> func1 =[](int x) { return 5*x + 34;};
	std::function<int(int)> func2 =[](int x) { return 5-x;};

	auto fw = applicative_functor<future_value>::pure<int,decltype(func1)>(func1);
	future_value<int,int> fvv(func2);	
	auto fv = applicative_functor<future_value>::apply(fw, fvv);

	auto r1 = runFutureValue(fv, 45);
	std::cerr << r1 << std::endl;
#endif

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
