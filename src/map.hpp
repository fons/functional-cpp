#ifndef __map__
#define __map__

template<typename T, typename U>
std::forward_list<U> map_helper_recur (std::function<U (T)>& op, std::forward_list<T>& L, std::forward_list<U>& acc)
{
  if (L.begin() == L.end()) {
    acc.reverse();
    return acc;
  }
  acc.push_front(op(L.front()));
  L.pop_front();
  return map_helper(op, L, acc);
}

template<typename T, typename U>
std::forward_list<U> map_helper (std::function<U (T)>& op, std::forward_list<T>& L, std::forward_list<U>& acc)
{
  while (! L.empty()) {
    acc.push_front(op(L.front()));
    L.pop_front();
  }
  acc.reverse();
  return acc;
}

//make this a forward list
template<typename T, typename U>
std::forward_list<U> map_fp (std::function<U (T)>& op, std::forward_list<T>& L)
{
  std::forward_list<U> acc;
  return map_helper(op, L, acc);
}

template<typename T, typename U>
std::forward_list<U> map_alt_recur (std::function<U (T)>& op, std::forward_list<T>& L)
{
  std::forward_list<U> acc;
  std::function  <std::forward_list<U> (std::forward_list<U>&) > helper = [&] (std::forward_list<U>& acc) -> std::forward_list<U>  {
    if (L.empty()) {
      acc.reverse();
      return acc;
    }
    acc.push_front(op(L.front()));
    L.pop_front();
    return helper(acc);
  };
  return helper(acc);
}


template<typename A, typename B>
std::forward_list<B> map (std::function<B (A)> op, const std::forward_list<A>& L)
{
  std::forward_list<B> acc;
  for (auto &v : L) {
    acc.push_front(op(v));
  }
  acc.reverse();
  return acc;
}

/*
template<typename A, typename B, typename F>
std::forward_list<B> map (F op, const std::forward_list<A>& L)
{
  std::forward_list<B> acc;
  for (auto &v : L) {
    acc.push_front(op(v));
  }
  acc.reverse();
  return acc;
}
*/

template<typename A, typename B>
std::list<B> map (std::function<B (A)> op, const std::list<A>& L)
{
  std::list<B> acc;
  for (auto &v : L) {
    acc.push_back(op(v));
  }
  return acc;
}

template<typename A, typename B, typename F>
std::list<B> map (F op, const std::list<A>& L)
{
  std::list<B> acc;
  for (auto &v : L) {
    acc.push_back(op(v));
  }
  return acc;
}


template<typename A, typename F>
auto map (F f, const std::forward_list<A>& L) -> std::forward_list<decltype(f(A()))>
{
  std::forward_list<decltype(f(A()))> H;
  std::transform(L.begin(), L.end(), std::front_inserter(H), f);
  H.reverse();
  return H;
}

template<typename A, typename F>
auto map (F f, const std::list<A>& L) -> std::list<decltype(f(A()))>
{
  std::list<decltype(f(A()))> H;
  std::transform(L.begin(), L.end(), std::back_inserter(H), f);
  return H;
}

#endif
