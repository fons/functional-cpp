#include "proto.hpp"
#include "map.hpp"

int map_1()
{
  std::list<int> L = {-1,2,33,4,50,90, -7,23};
  std::for_each(L.begin(), L.end(), [](int elem) { std::cout << elem << std::endl;});
  return 0;
}



int map_2()
{
  std::forward_list<int> K  = {-1,2,33,4,50,90, -7,23};
  std::function<int (int) > f2 = [] (int x) {return 2 * x + 8;};
  std::forward_list<int> L = map(f2, K);
  std::for_each(L.begin(), L.end(), [](int elem) { std::cout << elem << std::endl;});
  std::for_each(K.begin(), K.end(), [](int elem) { std::cout << elem << std::endl;});
  return 0;
  

}
