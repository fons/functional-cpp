#ifndef h__maybe__h
#define h__maybe__h



template <typename A>
struct Maybe {
	typedef A value_type;

	Maybe(const Maybe& o) : val(o.val){}

	void operator=(const Maybe& o) = delete;

 	std::ostream& pp(std::ostream& strm) const {
		if (val.second) {
			strm << "Just(" << val.first << ") [" <<  typeid(A).name() << "]";
		}
		else {
			strm << "None[" <<  typeid(A).name() << "]";
		}
		return strm;
	}

	static Maybe Just(const A& a) {
		return Maybe(a);
	}

	static Maybe None() {
		return Maybe();
	}

	bool eq (const Maybe& m) const {
		return (val.second == m.val.second) && (val.first == m.val.first);
	}
	
	const A& operator*() {
		return val.first;
	}

private: 

	Maybe() : val(std::make_pair(A(), false)) {}
	explicit Maybe(const A& a) : val(std::make_pair(a,true)){}
	const std::pair<A, bool> val;

};


template<typename A>
Maybe<A> just(A val) {
	return Maybe<A>::Just(val);
}

template<typename A>
Maybe<A> none() {
	return Maybe<A>::None();
}

template<typename A>
std::ostream& operator<<(std::ostream& strm, const Maybe<A>& M) 
{
	M.pp(strm);
	return strm;
}

template<typename A, typename B>
bool operator==(const Maybe<A>& l, const Maybe<B>& r) {
	if (l.eq(l.None()) && r.eq(r.None())) return true;
	return false;
}

template<typename A>
bool operator==(const Maybe<A>& l, const Maybe<A>& r) {
	return l.eq(r);
}

template<typename A, typename B>
bool operator!=(const Maybe<A>& l, const Maybe<B>& r) {
	return !(l==r);
}




#endif
