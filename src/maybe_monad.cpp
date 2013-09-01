#include "proto.hpp"
#include "show.hpp"
#include "maybe_monad.hpp"

	

int mbm_0()
{
	auto f = [] (int x) {
		return "xxxxxx";
	};

	auto v1 = just(10);
	auto v2 = functor<Maybe>::fmap(f, v1);
	auto v3 = functor<Maybe>::fmap<int>(f)(v1);
	std::cerr << v1 << std::endl;
	std::cerr << v2 << std::endl;
	std::cerr << v3 << std::endl;
	if (v2 == v3) {
		std::cerr << v2 << " == " << v3 << std::endl;
	}
	return 0;
}

int mbm_1()
{
	typedef std::pair<int, std::string> arg_t;

	Maybe<arg_t> L = just(std::make_pair(9,std::string("hello")));

	auto get_string = [] (arg_t arg) {
		return arg.second;
	};

	auto lm = applicative_functor<Maybe>::pure(get_string);

	auto v1 = applicative_functor<Maybe>::apply(lm, L);
	std::cerr << v1 << std::endl;

	auto v2 = applicative_functor<Maybe>::apply<arg_t>(lm)(L);
	std::cerr << v2 << std::endl;

	return 0;
}

int mbm_2()
{

	auto val1 = just(56);
	auto lambda = [](int x) {
		if ( x > 0) return just(std::string("good !"));
		return none<std::string>();
	};
	auto val2 = monad<Maybe>::bind(val1, lambda);
	std::cerr << val1 << std::endl;
	std::cerr << val2 << std::endl;

	std::cerr << monad<Maybe>::bind(just(-89), lambda) << std::endl;

	std::cerr << monad<Maybe>::bind(monad<Maybe>::bind(just(-89), lambda),
									[](const std::string& val) {
										std::cerr<<"===> val" << std::endl;
										return just(val);
									});
	std::cerr << std::endl;
	std::cerr << monad<Maybe>::bind(monad<Maybe>::bind(just(89), lambda),
									[](const std::string& val) {
										std::cerr<<"===>  val " << val << std::endl;
										return just(val);
									});
									

	return 0;
}


