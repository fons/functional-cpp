#include "proto.hpp"
#include "show.hpp"
#include "future_value.hpp"

#include <chrono>

/////////////////////////////////////////////////////////////////////////////
int fv_0()
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
	future_value<int,int> fv(f);
	std::cerr << fv << std::endl;
	auto res = runFutureValue(fv, 45);
	std::cerr << res << std::endl;

	return 0;
}

int fv_1()
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

	future_value<int,int> fv = make_future_value(g) + make_future_value(f); 

	std::cerr << fv << std::endl;
	auto res = runFutureValue(fv, 45);
	std::cerr << res << std::endl;
	return 0;

	
}

int fv_2()
{
	std::function<float(int, int, float)> f = [](int x, int y, float z) { return z *(34*x - x*y);};
	auto fv = make_future_value(f);
	std::cerr << fv << std::endl;
	auto res = runFutureValue<float,int,int,float>(fv, 1,2,0.45677); 
	std::cerr << res << std::endl;
	return 1;
}

int fv_3()
{
	return 1;
}

int fv_4()
{
	return 1;
}

int fv_5()
{
	return 1;
}
