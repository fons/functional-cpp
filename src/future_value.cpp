#include "proto.hpp"
#include "show.hpp"
#include "future_value.hpp"

#include <chrono>

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
