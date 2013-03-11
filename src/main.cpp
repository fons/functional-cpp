#include "proto.hpp"
#include <map>
#include <deque>

#define ADD_FUNC(FN)     std::make_pair(#FN, FN)


int main(int argc, char **argv)
{

  std::map<std::string, std::function<int ()>> M = {ADD_FUNC(lambda_9),
						    ADD_FUNC(trans_1),
						    ADD_FUNC(trans_2),
						    ADD_FUNC(trans_3),
						    ADD_FUNC(trans_4),
						    ADD_FUNC(trans_5),
						    ADD_FUNC(trans_6),
						    ADD_FUNC(m_1),
						    ADD_FUNC(functor_1),
						    ADD_FUNC(functor_2),
						    ADD_FUNC(functor_3),
						    ADD_FUNC(functor_4),
						    ADD_FUNC(functor_5),
						    ADD_FUNC(functor_6),
						    ADD_FUNC(apf_1),
						    ADD_FUNC(apf_2),
						    ADD_FUNC(apf_3),
						    ADD_FUNC(apf_4),
						    ADD_FUNC(apf_5),
						    ADD_FUNC(m_2)};  
  
  auto show_all = [=] (const std::string& ps)  {       
    size_t count = 1;
    std::cout << ps;
    for (auto& p : M) {
      std::cout << p.first << ",";
      if (count++ % 5 == 0) std::cout << std::endl << ps;
    }
    std::cout << std::endl;
    std::cout << ps << "h  : history" << std::endl;
    std::cout << ps << "!  : last command" << std::endl;
    std::cout << ps << "!n : command at position n" << std::endl;
  };

  auto is_number = [] (const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
  }; 


  auto bail = [=] (const std::string& ps) {
    std::cout << ps << "goodbye...." << std::endl;
    exit(0);
  };

  auto exec = [=] (const std::string& ps, const std::string& in) {
    auto it = M.find(in); 
    if(it != M.end()) {
      std::cout << ps << "executing " << it->first << std::endl;
      it->second();
      std::cout << "     ***     " << std::endl;
    }
    else {
      std::cout << ps << "action " << in << " not found" << std::endl;
      show_all(ps);
    }
  };
  std::deque<std::string> H;
  auto show_hist = [&] (const std::string& ps){
    size_t index = 0;
    for (auto& el : H) {
      std::cout << ps << index << ") " << el << std::endl;
      index++;
      if (index > 10) break;
    }
  };
  

  const std::string ps = "-> ";

  auto run_cmd = [&] (const std::string& in) {
    if (in == "q" or in == "Q") bail(ps);
    if (in == "?") {
      show_all(ps);
      return;
    }
    if (in == "h") {
      show_hist(ps);
      return;
    }
    exec(ps, in);
    return;
  };

  bool keep = true;

  while (keep) {
    std::cout << ps ;
    std::string in;
    std::getline(std::cin, in);
    if (in != "?" && in != "h" && in != "!" && !is_number(in)) H.push_front(in);
    std::string cmd(in); 
    if (in == "!") {
      cmd = H.front();
      H.pop_front();
    }
    if (is_number(in)) {
      int index = std::stoi(in);
      if (index > -1 && index < (int) H.size()) {
	cmd = H[index];;
      }
    }
    run_cmd(cmd);

  }
  return 0;
}

