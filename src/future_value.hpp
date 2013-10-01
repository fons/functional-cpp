#ifndef h__future_value__h
#define h__future_value__h
#include <thread>
#include <future>
#include <exception>
#include "maybe.hpp"
#include "thunk.hpp"


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
		if (F != nullptr) {
			strm << "> (" << F << ")";
		}
		return strm;
	}

	Maybe<Ret> operator()(Arg... args) {
		try {
			auto th = thunk(F, args...);
			auto f  = std::async([&th]{return th();});
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


#endif
