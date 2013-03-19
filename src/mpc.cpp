#include "proto.hpp"
#include "mpc.hpp"

plist_t<char> item_f(std::string s) {
  if (s.length() == 0) {
    return plist_t<char>();
  }
  return plist_t<char>({ppair_t<char>(s.front(), s.substr(1))});
}

int mpc_1()
{
  ppair_t<char> y ('c', "hello");
  std::cout << y << std::endl;
  plist_t<char> L = {y};
  L.push_back(ppair_t<char>('p', "done"));
  std::cout << L << std::endl;
  parser_t<char> F = [=](std::string s) { return L;};
  std::cout << F << std::endl;

  Parser<char> P(F);
  std::cout << P << std::endl;
  
  auto res = parse (P)("hello");
  std::cout << "->" << res << std::endl;
  return 0;
}

int mpc_2()
{
  
  Parser <char> item(item_f);
  auto res = parse (item) ("hello");
  std::cout << res << std::endl;
  auto res1 = parse (item) ("");
  std::cout << res1 << std::endl;
  return 0;
}
