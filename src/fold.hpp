#ifndef __fold__
#define __fold__
template <typename T, typename U>
U foldl(std::function< U (U, T)>& op, const U& val, const std::forward_list<T>& L)
{
  U acc(val);
  for (auto &v : L) {
    acc = op(acc, v);
  }
  return acc;
}


template <typename T, typename U>
U foldl(std::function< U (U, T)>& op, const std::forward_list<T>& L)
{
  U acc = U();
  for (auto &v : L) {
    acc = op(acc, v);
  }
  return acc;
}

#endif
