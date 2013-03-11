#include "proto.hpp"
#include "w.hpp"



int w_1()
{
  W id(0,"");
  W val_1(1256, "hello");
  W val_2(-9088, "goodbye");
  W val_3(786, "haha");

  if ((id + val_1) == (val_1 + id)) {
    std::cout << "have indentity element" << std::endl;
  }
  
  if ((val_3 + (val_2 + val_1)) == ((val_3 + val_2) + val_1)) {
    std::cout << "have associativity" << std::endl;
  }
  
  return 0;
}
