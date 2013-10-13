#include "proto.hpp"
#include "show.hpp"
#include "future_value_monad.hpp"
#include "curry.hpp"
#include "bracket.hpp"

//////////////////////////////////////////////////////////
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

	future_value<std::function<int(int)>, int> fw = applicative_functor<future_value>::pure<decltype(func1), int>(func1);
	std::cerr << " fw : " << fw << std::endl;

	auto ty = runFutureValue(fw, 67);
	std::cerr << " ty : " << ty << " -- (*ty)(45) : " << (*ty)(45) << std::endl;
	
	future_value<int,int> fvv(func2);	
	std::cerr << "fvv : " << fvv << std::endl;

	auto r1 = runFutureValue(fvv, 45);
	std::cerr << " r1 : " << r1 << std::endl;

	auto fv = applicative_functor<future_value>::apply<int,int,int>(fw, fvv);
	auto r0 = runFutureValue(fv, 45);
	std::cerr << "r0 : " << r0 << std::endl;

	auto fv1 = applicative_functor<future_value>::apply<int,int,int>(fw)(fvv);
	auto r01 = runFutureValue(fv, 45);
	std::cerr << r01 << std::endl;
	
	future_value<int,int> fv0 = functor<future_value>::fmap(func1, fvv); 
	auto r02 = runFutureValue(fv0, 45);
	std::cerr << "r02 : " << r02 << std::endl;

	return 0;
}

int fvm_4()
{
	std::function<int(int)> func1 =[](int x) { return 5*x + 34;};	

	std::function<future_value<std::string, int>(int)> func2 = [] (int x) {
		std::function<std::string(int)> f =[] (int x) {
			if (x < 100) return std::string("too small");
			return std::string("just right");
		};
		return make_future_value(f);
	};

	auto fv = make_future_value(func1);
	std::cerr << fv << std::endl;
	std::cerr << func2 << std::endl;
	auto res = runFutureValue(fv, 45); 
	std::cerr << res << std::endl;
	std::cerr << "------------------- ****** -----------------------------" << std::endl;
	auto fy = monad<future_value>::bind<int,int,std::string>(fv, func2);
	auto mres = runFutureValue(fy, -90);
	std::cerr << mres << std::endl;
	return 0;
}

int fvm_5()
{

	std::function<int(int)> func1 =[](int x) { return 5*x + 34;};	

	std::function<future_value<int, int>(int)> func2 = [] (int x) {

		std::function<int(int)> f =[] (int x) {
			return -30*x + 90;
		};

		std::function<int(int)> g =[] (int x) {
			return 90 - 900*x;
		};

		if (x < 100) {
			return make_future_value(f);
		}
		return make_future_value(g);
	};

	auto fv = make_future_value(func1);
	auto fy = monad<future_value>::bind(fv, func2);
	auto mres = runFutureValue(fy, -90);
	std::cerr << "-90 ==> " << mres << std::endl;
	auto mres1 = runFutureValue(fy, 90);
	std::cerr << "90 ==> " << mres1 << std::endl;
	return 0;
}
