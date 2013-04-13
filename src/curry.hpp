#ifndef h__curry__h
#define h__curry__h
#include "curry_helper.hpp"

template <typename R, typename T, typename U>
std::function<std::function<R (U)> (T)> curry (std::function<R (T,U)> op)
{
  return [=] (T x) { return [=] (U y) {return op(x, y);};};
}

template <typename F, typename... args>
auto curry (F op) 
{
  return curry_helper<sizeof...(args), F, args...>()(op);
} 

#endif
