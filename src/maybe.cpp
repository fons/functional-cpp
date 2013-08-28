#include "proto.hpp"
#include "show.hpp"
#include "maybe.hpp"


//////////////////////////////////////////////////////

int mb_0()
{
	auto val = just("hello");
	std::cerr << val << std::endl;

	auto val2 = none<int>();
	std::cerr << val2 << std::endl;
	return 0;
}

int mb_1()
{

	auto f = [](){return none<int>();};
	auto g = [](){return just(std::make_pair(34,"hello"));};
	auto v1 = f();
	auto v2 = g();
	std::cerr << v1 << "," << v2 << std::endl;

	if (v1 == none<std::string>()) {
		std::cerr << "none : " << v1 << std::endl;
	}

	if (v1 != none<std::string>()) {
		std::cerr << "none : " << v1 << "!=" << none<std::string>() << std::endl;
	}


	if (v1 == v2) {
		std::cerr << "v1==v2 ?? -> " << v1 << " " << v2 << std::endl;
	}
	
	if (v2 != v1) {
		std::cerr << "v1!=v2 ?? -> " << v1 << " " << v2 << std::endl;
	}


	if (v2 != just(1)) {
		std::cerr << v2 << " != " << 1 << std::endl;
	}

	if (v2 == just(std::make_pair(34,"hello"))) {
		std::cerr << v2 << "==" << just(std::make_pair(34,"hello")) << std::endl;
	}

	if (just(std::make_pair(34,"hello")) == v2) {
		std::cerr << just(std::make_pair(34,"hello")) << "==" << v2 << std::endl;
	}

	if (just(std::make_pair(24,"hello")) != v2) {
		std::cerr << just(std::make_pair(24,"hello")) << "!=" << v2 << std::endl;
	}

	if (g() == g()) {
		std::cerr << "g() :" << g() << std::endl;
	}

	if (f() == f()) {
		std::cerr << "f() " << f() << std::endl;
	}

	if (Maybe<int>::None() == Maybe<std::string>::None()) {
		std::cerr << Maybe<int>::None() << " == " << Maybe<std::string>::None() << std::endl;
	}

	if (Maybe<int>::None() != Maybe<std::string>::None()) {
		std::cerr << Maybe<int>::None() << " != " << Maybe<std::string>::None() << std::endl;
	}
	
	return 0;
}

int mb_2()
{

	auto g = [] (int x){
		if (x > 0) return just(x);
		return none<int>();
	};
	
	auto f = [] (Maybe<int> m) {
		if (m != m.None()) {
			std::cerr << "hey this is set : " << m << std::endl;
		}
		return m;
	};

	std::cerr << g(23) << std::endl;
	std::cerr << g(-23) << std::endl;
	f(g(23));
	f(g(-23));

	return 0;
}
