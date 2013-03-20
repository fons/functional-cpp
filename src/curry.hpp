#ifndef h__curry__h
#define h__curry__h
#include "curry_helper.hpp"

template <typename R, typename T, typename U>
std::function<std::function<R (U)> (T)> curry__ (std::function<R (T,U)> op)
{
  return [=] (T x) { return [=] (U y) {return op(x, y);};};
}
//------------------------------------------------------

//template <typename F, typename R, typename T=R, typename U=T>
//std::function<std::function<R (U)> (T)> curry (F op)
//{
//  return [=] (T x) { return [=] (U y) {return op(x, y);};};
//}

template <typename F, typename R, typename... args>
typename curry_helper<sizeof...(args), F, R, args...>::f1_t curry (F op) 
{
  return curry_helper<sizeof...(args), F, R, args...>()(op);
} 

#endif
