#ifndef h__either__h
#define h__either__h
#include "maybe.hpp"


template <typename L, typename R>
struct Either {

	typedef L left_value_type;
	typedef R right_value_type;

	Either(const Either& o) : value(o.value) {}

	void operator=(const Either&) = delete;

	static Either Left(const L& l) {
		return Either(l, true);
	}

	static Either Right(const R& r) {
		return Either(r);
	}

	bool Left() const {
		return std::get<2>(value);
	}

	bool Right()  const {
		return (! Left());
	}

 	std::ostream& pp(std::ostream& strm) const {
		strm << "Either<" <<  typeid(L).name() << "," << typeid(R).name() << ">";;
		if (std::get<2>(value)) {
			strm << "[" << left() << ", null]";
		}
		else {
			strm << "[null," << right() << "]"; 
		}
		return strm;
	}

	bool eq(const Either& o) const {
		auto eqval = [this] (const value_t& l, const value_t& r) {
			if (std::get<2>(value)) return std::get<0>(l) == std::get<0>(r);
			return std::get<1>(l) == std::get<1>(r);
		};
		return ((std::get<2>(value) == std::get<2>(o.value))
				&& eqval(value, o.value));
	}

	const L& left() const {
	   return std::get<0>(value);
	}

	const R& right() const {
		return std::get<1>(value);
	}

private :
	Either (L val, bool left) : value(std::make_tuple(val,R(),true)) {}
	Either (R val) : value(std::make_tuple(L(), val,false)) {}
	
	typedef std::tuple<L,R,bool> value_t;
	value_t value;
	
};


template<typename A, typename B>
std::ostream& operator<<(std::ostream& strm, const Either<A,B>& E) 
{
	E.pp(strm);
	return strm;
}


template<typename A, typename B>
Maybe<A> Left(const Either<A,B>& e)
{
	if (e.Left()) {
		return just(e.left());
	}
	return none<A>();
}

template<typename A, typename B>
Maybe<B> Right(const Either<A,B>& e)
{
	if (e.Right()) {
		return just(e.right());
	}
	return none<B>();
}

template<typename A, typename B,typename C, typename D>
bool operator==(const Either<A,B>& a, const Either<C,D>& b) {
	return false;
}

template<typename A, typename B> 
bool operator==(const Either<A,B>& a, const Either<A,B>& b) {
	return a.eq(b);
}

template<typename A, typename B,typename C, typename D>
bool operator!=(const Either<A,B>& a, const Either<C,D>& b) {
	return !(a==b);
}

#endif
