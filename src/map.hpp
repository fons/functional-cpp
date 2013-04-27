#ifndef __map__
#define __map__


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

template<typename A, typename F>
auto map (F f, const std::forward_list<A>& L) -> std::forward_list<decltype(f(A()))>
{
  std::forward_list<decltype(f(A()))> H;
  std::transform(L.begin(), L.end(), std::front_inserter(H), f);
  H.reverse();
  return H;
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
auto map (F f, const std::list<A>& L) -> std::list<decltype(f(A()))>
{
  std::list<decltype(f(A()))> H;
  std::transform(L.begin(), L.end(), std::back_inserter(H), f);
  return H;
}

#endif
