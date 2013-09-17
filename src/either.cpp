#include "proto.hpp"
#include "show.hpp"
#include "either.hpp"


///////////////////////////////////////////////////////

int ei_0()
{

	auto val = Either<std::string, std::string>::Left("hello");
	std::cerr << val << std::endl;

	auto v2 = Either<std::string, std::string>::Right("hello");
	std::cerr << v2 << std::endl;

	auto v3 = v2;
	std::cerr << v3 << std::endl;

	return 0;
}

int ei_1()
{
	auto v = Either<std::string, int>::Right(45);
	std::cerr << v << "=>" << Left(v) << ":" << Right(v) << std::endl;
	return 0;
}

int ei_2()
{

	auto v1 = Either<std::string, int>::Right(45);
	auto v2 = Either<std::string, int>::Right(46);
	auto v3 = Either<float, char>::Right('x');
	if (v1 == v1) {
		std::cerr << "v1==v1 : " << v1 << std::endl;
	}

	if (v1 == v2) {
		std::cerr << "v1==v2: " << v1 << " : " << v2 << std::endl;
	}
	
	if (v1 != v2) {
		std::cerr << "v1!=v2: " << v1 << " : " << v2 << std::endl;
	}

	if (v1 == v3) {
		std::cerr << "v1==v3: " << v1 << " : " << v3 << std::endl;
	}
	
	if (v1 != v3) {
		std::cerr << "v1!=v3: " << v1 << " : " << v3 << std::endl;
	}
	
	return 0;
}

int ei_3()
{

	auto f = [](int x){return 2+x;};
	auto val = Either<decltype(f), std::string>::Left(f);
	std::cerr << val.left()(23) << std::endl;
	
	auto v2 = Either<std::string,decltype(f)>::Right(f);
	std::cerr << v2.right()(23) << std::endl;

	return 0;
}

int ei_4()
{

	return 0;
}
