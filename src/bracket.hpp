#ifndef h__mbracket__h
#define h__mbracket__h

#include "bracket_helper.hpp"


template <typename F, template<typename Tx, typename... D> class Cont, typename... T> 
auto bracket (F f, Cont<T>... L) 
{
	auto cf = curry<decltype(f), T...  >(f);	
	return bracket_helper<sizeof...(T), Cont , decltype(cf), T...>::bracket(cf, L...);
} 

#endif
