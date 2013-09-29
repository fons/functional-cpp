#include "proto.hpp"
#include "show.hpp"
#include "thunk.hpp"


//////////////////////////////



int th_0()
{
	std::function<int(int)> f = [](int x) { return 34*x - x;};

	auto th = thunk(f, 2);
	std::cerr << "thunk : " << th << " => " << th() << std::endl;

	//exec_pack(f, n);
	//auto nm = value_pack<int,int, int>(1,2,3);
	//std::cerr << nm.val << "," << nm.rest.val << "," << nm.rest.rest.val << std::endl;

	auto thh = thunk(f,1);
	std::cerr << thh << "  ==> " << thh() << std::endl;

	std::function<int(int,int)> f2 = [](int x, int y) { return 34*x - y;};
	auto th2 = thunk(f2,1,4);
	std::cerr << th2 << "  ==> " << th2() << std::endl;

	std::function<float(int,int, float)> f3 = [](int x, int y, float z) { return 34*x - y*z;};
	auto th3 = thunk<float,int,int,float>(f3,1,4,0.989667);
	std::cerr << th3 << "  ==> " << th3() << std::endl;

	std::function<float()> f0 = []() { return 34;};
	//auto th0 = thunk_helper<float>(f0);
	//std::cerr << th0 << "  ==> " << th0() << std::endl;


	return 0;
}

int th_1()
{
	std::function<int(int, int)> f = [](int x, int y) { return 34*x - x*y;};

	auto th = thunk(f, 2, 56);
	std::cerr << "thunk : " << th << " => " << th() << std::endl;

	return 0;
}

int th_2()
{
	std::function<int(std::string, int, int, float)> f = [](std::string s, int x, int y, float f) { return 34*x - x*y;};

	return 0;
}
