#ifndef h__zipwith__h
#define h__zipwith__h

template<typename A, typename B, typename F>
auto zipWith (F f, const std::forward_list<A>& L, const std::forward_list<B>& R) -> std::forward_list<decltype(f(A(),B()))>
{
  std::forward_list<decltype(f(A(),B()))> H;
  std::transform(L.begin(), L.end(), R.begin(), std::front_inserter(H), f);
  H.reverse();
  return H;
}

template<typename A, typename B>
std::forward_list<std::tuple<A,B>> zip (const std::forward_list<A>& L, const std::forward_list<B>& M) 
{
  return zipWith([] (const A& a, const B& b) {return std::make_tuple(a,b);}, L, M);
}

template<typename A, typename B, typename F>
auto zipWith (F f) {
  return [=](const std::forward_list<A>& L) {
    return [=](const std::forward_list<B>& R) -> std::forward_list<decltype(f(A(),B()))> {
      std::forward_list<decltype(f(A(),B()))> H;
      std::transform(L.begin(), L.end(), R.begin(), std::front_inserter(H), f);
      H.reverse();
      return H;
    };
  };
};

#endif
