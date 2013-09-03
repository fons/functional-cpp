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
		return value.second;
	}

	bool Right()  const {
		return (! Left());
	}

	const L& left() const {
		return value.first.first;
	}

	const R& right() const {
		return value.first.second;
	}

 	std::ostream& pp(std::ostream& strm) const {
		strm << "Either<" <<  typeid(L).name() << "," << typeid(R).name() << ">";;
		if (value.second) {
			strm << "[" << value.first.first << ", null]";
		}
		else {
			strm << "[null," << value.first.second << "]"; 
		}
		return strm;
	}

	bool eq(const Either& o) const {
		auto eqval = [this] (const value_t& l, const value_t& r) {
			if (value.second) return l.first == r.first;
			return l.second == r.second;
		};
		return ((value.second == o.value.second)
				&& eqval(value.first, o.value.first));
	}

private :
	Either (L val, bool left) : value(std::make_pair(std::make_pair(val,R()) , true)) {}
	Either (R val) : value(std::make_pair(std::make_pair(L(),val) , false)) {}

	//Either (L val, bool left) : value(std::make_pair(value_t(val) , true)) {}
	//Either (R val) : value(std::make_pair(value_t(val) , false)) {}

	/*
	typedef struct {
		L  first;
		R  second;
	} value_t;
	*/
	
	typedef std::pair<L,R> value_t;
	std::pair<value_t, bool> value;
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
