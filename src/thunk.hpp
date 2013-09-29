#ifndef  h__thunk__h
#define  h__thunk__h
#include "value_pack.hpp"

/*
  this is a workaround a bug in gcc which doesn't allow parameter packs to be expanded inside lambda functions.
  
 */
#if 0
template <int n, typename R, typename T1=void, typename T2=T1, typename T3=T2, typename T4=T3, typename T5=T4>
struct thunk_helper 
{
  

};

template <typename R, typename Arg1>
struct thunk_helper<1, R, Arg1>
{
	typedef std::function<R(Arg1)> function_type;
	
	explicit thunk_helper(function_type F, Arg1 arg1) : func(F), val1(arg1){}
	thunk_helper(const thunk_helper& o) : func(o.func), val1(o.val1) {}
	void operator=(const thunk_helper&) = delete;
	R operator()() {
		return func(val1);
	}

	std::ostream& pp(std::ostream& strm) const {
		strm << "thunk <" << 1 
			 << "," << typeid(R).name() 
			 << "," << typeid(Arg1).name() 
			 << ">"
			 << "[ " << func << "(" << val1 << ")]";
		return strm;
	}

private:
	std::function<R(Arg1)> func;
	const Arg1 val1;
};

template <typename R, typename Arg1, typename Arg2>
struct thunk_helper<2, R, Arg1, Arg2>
{
	typedef std::function<R(Arg1, Arg2)> function_type;
	
	explicit thunk_helper(function_type F, Arg1 arg1, Arg2 arg2) : func(F), val1(arg1), val2(arg2){}
	thunk_helper(const thunk_helper& o) : func(o.func), val1(o.val1), val2(o.val2) {}
	void operator=(const thunk_helper&) = delete;
	R operator()() {
		return func(val1, val2);
	}

	std::ostream& pp(std::ostream& strm) const {
		strm << "thunk <" << 2 
			 << ","<< typeid(R).name() 
			 << "," << typeid(Arg1).name() 
			 << "," << typeid(Arg2).name() 
			 << ">"
			 << "[ " << func << "(" << val1 << "," << val2 << ")]";
		return strm;
	}


private:
	std::function<R(Arg1, Arg2)> func;
	const Arg1 val1;
	const Arg2 val2;
};
#endif


template <typename Ret, typename... Arg>
struct thunk_helper
{
  	typedef std::function<Ret(Arg...)> function_type;

	explicit thunk_helper(function_type F,  Arg... args) : func(F), val1(args...) {}
	thunk_helper(const thunk_helper& o) : func(o.func) , val1(o.val1) {}

	void operator=(const thunk_helper&) = delete;
	Ret operator()() {
		return apply_pack<sizeof...(Arg), Ret, Arg...>::apply(func, val1); 
	}	

     std::ostream& pp(std::ostream& strm) const {
		 
		 strm << "thunk <" << 1 
			  << "," << typeid(Ret).name() 
			  << "," ;
		 typeids<Arg...>(strm);
		 strm << ">";
		 strm << "[ " << func << "(";  
		 val1.pp(strm);
		 strm << ")]";
		 return strm;
	 }

private:
	std::function<Ret(Arg...)> func;
	value_pack<Arg...> val1;
};


template <typename Ret, typename... Arg>
std::ostream& operator<<(std::ostream& strm, thunk_helper<Ret, Arg...>& th) 
{
	th.pp(strm);
	return strm; 
}


template <typename R, typename... Args>
thunk_helper<R,Args...> thunk (std::function<R(Args...)> F, Args... args) 
{
	return thunk_helper<R, Args...>(F, args...);
};


#endif
