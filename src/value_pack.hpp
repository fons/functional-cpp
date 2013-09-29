#ifndef h__valaue_pack__h
#define h__valaue_pack__h

template <typename Arg0, typename... Argn>
struct value_pack{
	explicit value_pack(Arg0 val0, Argn... val) : val(val0), rest(val...) {}
	value_pack(const value_pack& o) : val(o.val), rest(o.rest){}
	void operator=(const value_pack&) = delete;

	Arg0 val;
	value_pack<Argn...> rest;

	std::ostream& pp(std::ostream& strm) const {
		strm << "value_pack<";
		typeids<Arg0, Argn...>(strm);
		strm << ">{" ;
		return ppp(strm);
	}

	std::ostream& ppp(std::ostream& strm) const {
		strm << val << ",";
		rest.ppp(strm);
		return strm;
	}
};

template<typename T>
struct value_pack <T>{
	explicit value_pack(T val0) : val(val0){}
	T val;

	std::ostream& pp(std::ostream& strm) const {
		strm << "value_pack<";
		typeids<T>(strm);
		strm << ">{" ;
		return ppp(strm);
	}


	std::ostream& ppp(std::ostream& strm) const {
		strm << val << "}";
		return strm;
	}

};

template<typename Arg0, typename... Arg>
std::ostream& operator<<(std::ostream& strm, value_pack<Arg0, Arg...>& th) 
{
	th.pp(strm);
	return strm; 
}
//////////////////
//......
template<int n, typename R, typename Arg0, typename... Args>
struct apply_pack
{
	//static R apply(std::function<R(Arg0, Args...)> F, value_pack<Arg0, Args...> args) {
	//	return R();
	//}
};

template<typename R>
struct apply_pack<0,R,void>
{
	static R apply(std::function<R()> F) {
		return F();;
	}
};

template<typename R, typename Arg0>
struct apply_pack<1,R,Arg0>
{
	static R apply(std::function<R(Arg0)> F, value_pack<Arg0> args) {

		return F(args.val);;
	}
};

template<typename R, typename Arg0, typename Arg1>
struct apply_pack<2,R,Arg0,Arg1>
{
	static R apply(std::function<R(Arg0, Arg1)> F, value_pack<Arg0, Arg1> args) {

		return F(args.val, args.rest.val);;
	}
};

template<typename R, typename Arg0, typename Arg1, typename Arg2>
struct apply_pack<3,R,Arg0,Arg1,Arg2>
{
	static R apply(std::function<R(Arg0, Arg1, Arg2)> F, value_pack<Arg0,Arg1,Arg2> args) {

		return F(args.val, args.rest.val, args.rest.rest.val);;
	}
};

template<typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
struct apply_pack<4,R,Arg0,Arg1,Arg2,Arg3>
{
	static R apply(std::function<R(Arg0, Arg1, Arg2, Arg3)> F, value_pack<Arg0,Arg1,Arg2, Arg3> args) {

		return F(args.val, args.rest.val, args.rest.rest.val, args.rest.rest.rest.val);
	}
};

#endif
