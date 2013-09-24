#ifndef h__future_value__h
#define h__future_value__h
#include <thread>
#include <future>
#include <exception>
#include "maybe.hpp"

template <typename Arg, typename Ret>
struct future_value {
	explicit future_value(std::function<Ret(Arg)> f) : F(f){}
	future_value(const future_value<Arg,Ret>& o) : F(o.F) {}
	const future_value<Arg,Ret> operator=(const future_value<Arg,Ret>& o) {
		if (o == &this) return *this;
		F = o.F;
		return *this;
	}

	std::ostream& pp(std::ostream& strm) const {
		strm << "future_value <" << typeid(Arg).name() 
			 << "," << typeid(Ret).name() << ">";
		return strm;
	}

	Maybe<Ret> operator()(Arg a) {
		try {
			auto f = std::async([=]{return F(a);});
			return just(f.get());
		} catch (const std::exception& e) {
			std::cerr << "exception : " << e.what() << std::endl;
		}
		return none<Ret>();
	}

	std::function<Ret(Arg)> operator*() const {
		return F;
	}

private :
	std::function<Ret(Arg)> F;
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

template <typename A, typename B, typename C>
future_value<A,C> operator+(future_value<B,C> l, future_value<A,B> r)
{
	std::function<C(A)> F = [l, r] (A val) {
		return (*l)((*r)(val));
	};

	return future_value<A,C>(F);
}

template <typename A, typename B>
future_value<A,B> make_future_value(std::function<B(A)> F)
{
	return future_value<A,B>(F);
}


#endif
