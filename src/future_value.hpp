#ifndef h__future_value__h
#define h__future_value__h
#include <thread>
#include <future>
#include <exception>

#include "maybe.hpp"

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

#endif
