#include "proto.hpp"
#include "show.hpp"
#include "future_value.hpp"
#include "thunk.hpp"

#include <chrono>


template <typename Ret, typename... Arg>
struct future_value {
	explicit future_value(std::function<Ret(Arg...)> f) : F(f){}
	future_value(const future_value<Ret,Arg...>& o) : F(o.F) {}
	const future_value<Ret,Arg...> operator=(const future_value<Ret,Arg...>& o) {
		if (o == &this) return *this;
		F = o.F;
		return *this;
	}

	std::ostream& pp(std::ostream& strm) const {
		strm  << "future_value <" ;
		strm  << typeid(Ret).name() << "," ;
		typeids<Arg...>(strm);
		strm << "> (" << F << ")";
		return strm;
	}

	Maybe<Ret> operator()(Arg... args) {
		try {
			auto th = thunk(F, args...);
			auto f = std::async([&th]{return th();});
			return just(f.get());
		} catch (const std::exception& e) {
			std::cerr << "exception : " << e.what() << std::endl;
		}
		return none<Ret>();
	}

	std::function<Ret(Arg...)> operator*() const {
		return F;
	}

private :
	std::function<Ret(Arg...)> F;
};

template<typename Ret, typename... Arg>
std::ostream& operator<<(std::ostream& strm, const future_value<Ret,Arg...>& fv) 
{
	return fv.pp(strm);;
}


template<typename Ret, typename... Arg>
Maybe<Ret> runFutureValue(future_value<Ret,Arg...> fv, Arg... arg)
{
	return fv(arg...);
}

template <typename A, typename B, typename C>
future_value<A,C> operator+(future_value<B,C> l, future_value<A,B> r)
{
	std::function<C(A)> F = [l, r] (A val) {
		return (*l)((*r)(val));
	};

	return future_value<A,C>(F);
}

template <typename Ret, typename... Arg>
future_value<Ret,Arg...> make_future_value(std::function<Ret(Arg...)> F)
{
	return future_value<Ret,Arg...>(F);
}


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
	//auto res = runFutureValue(fv, 1,2,0.45677); //std::string("hello")); //, 1, 56, 56.89);
	//std::cerr << res << std::endl;
	return 1;
}
