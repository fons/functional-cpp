#include "proto.hpp"
#include "show.hpp"
#include "maybe.hpp"
#include "future_value.hpp"
#include "maybe.hpp"

#include <thread>
#include <future>
#include <exception>

#include <chrono>

template <typename A, typename B>
struct future_value {
	explicit future_value(std::function<B(A)> f) : F(f){}
	future_value(const future_value<A,B>& o) : F(o.F) {}
	const future_value<A,B> operator=(const future_value<A,B>& o) {
		if (o == &this) return *this;
		F = o.F;
		return *this;
	}

	std::ostream& pp(std::ostream& strm) const {
		strm << "future_value <" << typeid(A).name() 
			 << "," << typeid(B).name() << ">";
		return strm;
	}

	Maybe<B> operator()(A a) {
		try {
			auto f = std::async([=]{return F(a);});
			return just(f.get());
		} catch (const std::exception& e) {
			std::cerr << "exception : " << e.what() << std::endl;
		}
		return none<B>();
	}
private :
	std::function<B(A)> F;

};
template<typename A, typename B>
std::ostream& operator<<(std::ostream& strm, const future_value<A,B>& fv) 
{
	return fv.pp(strm);;
}

template<typename A, typename B>
Maybe<B> runFutureValue(future_value<A,B> fv, A arg)
{
	return fv(arg);
}

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
