#ifndef h__state__h
#define h__state__h
//
//variation on a pair which allows the first element not to be set.
//
template<typename A, typename S>
struct state_tuple {
	explicit state_tuple (S s) : e(std::make_pair(A(), s)), set(false){}
	state_tuple (A a, S s) : e(std::make_pair(a,s)), set(true) {}
	state_tuple(const state_tuple& s) : e(s.e), set(s.set){}
	std::ostream& pp(std::ostream& strm) const {
		if (set) {
			strm << e;
		}
		else {
			strm << "(()," << e.second << ")";
		}
		return strm;
	}

	std::pair<A,bool> value() const {
		return std::make_pair(e.first, set);
	}

	std::pair<S,bool> state() const {
		return std::make_pair(e.second, true);
	}

private :	
	std::pair<A,S> e;
	bool set;
};





template<typename A, typename S>
using state_computation = std::function< state_tuple<A,S> (S)>;

template <typename A, typename S>
struct state 
{
	state(state_computation<A,S> C) : C(C){}
	state(const state& o) : C(o.C){}
	std::ostream& pp(std::ostream& strm) const {
		strm << "state<" << typeid(A).name() << "," << typeid(S).name() << ">";
		return strm;
	}
	
	state_tuple<A,S> runState(S state) {
		return C(state);
	}
	
private:
	state_computation<A,S> C;
};

template<typename A, typename S>
std::ostream& operator<<(std::ostream& strm, const state<A,S>& M) 
{
	M.pp(strm);
	return strm;

}

template<typename A, typename S>
std::ostream& operator<<(std::ostream& strm, const state_tuple<A,S>& ST) 
{
	ST.pp(strm);
	return strm;

}

template<typename A, typename S>
state_tuple<A,S> runState(state<A,S> M, S state)
{
	return M.runState(state);
}


#endif
