#include "proto.hpp"
#include <map>
#include <deque>

#define ADD_FUNC(FN)     std::make_pair(#FN, FN)


int main(int argc, char **argv)
{


  std::map<std::string, std::function<int ()>> M = { ADD_FUNC(bracket_1),
						     ADD_FUNC(bracket_2),
						     ADD_FUNC(bracket_3),
						     ADD_FUNC(bracket_4),
						     ADD_FUNC(bracket_5),
						     ADD_FUNC(bracket_6),
						     ADD_FUNC(bracket_7),
						     ADD_FUNC(bracket_8),
						     ADD_FUNC(bracket_9),
						     ADD_FUNC(bracket_10),
						     ADD_FUNC(bracket_11),
							 ADD_FUNC(eim_0),
							 ADD_FUNC(eim_1),
							 ADD_FUNC(eim_2),
							 ADD_FUNC(eim_3),
							 ADD_FUNC(eim_4),
							 ADD_FUNC(th_0),
							 ADD_FUNC(th_1),
							 ADD_FUNC(th_2),
							 ADD_FUNC(ei_0),
							 ADD_FUNC(ei_1),
							 ADD_FUNC(ei_2),
							 ADD_FUNC(ei_3),
							 ADD_FUNC(ei_4),
							 ADD_FUNC(mbm_0),
							 ADD_FUNC(mbm_1),
							 ADD_FUNC(mbm_2),
							 ADD_FUNC(mb_0),
							 ADD_FUNC(mb_1),
							 ADD_FUNC(mb_2),
							 ADD_FUNC(mb_3),
							 ADD_FUNC(mb_4),
							 ADD_FUNC(mb_5),
							 ADD_FUNC(fv_0),
							 ADD_FUNC(fv_1),
							 ADD_FUNC(fv_2),
							 ADD_FUNC(fvm_0),
							 ADD_FUNC(fvm_1),
							 ADD_FUNC(fvm_2),
							 ADD_FUNC(fvm_3),
							 ADD_FUNC(fvm_4),
							 ADD_FUNC(fvm_5),
							 ADD_FUNC(st_0),
							 ADD_FUNC(st_1),
							 ADD_FUNC(stm_0),
							 ADD_FUNC(stm_1),
							 ADD_FUNC(stm_2),
							 ADD_FUNC(stm_3),
							 ADD_FUNC(stm_4),
							 ADD_FUNC(stm_5),
							 ADD_FUNC(stm_6),
							 ADD_FUNC(stm_7),
							 ADD_FUNC(stm_8),
							 ADD_FUNC(stm_9),
							 ADD_FUNC(stm_10),
						     ADD_FUNC(cur_1),
						     ADD_FUNC(mpc_1),
						     ADD_FUNC(mpc_2),
						     ADD_FUNC(mpc_3),
						     ADD_FUNC(mpc_4),
						     ADD_FUNC(mpc_5),
						     ADD_FUNC(mpc_6),
						     ADD_FUNC(lambda_1),
						     ADD_FUNC(lambda_6),
						     ADD_FUNC(lambda_7),
						     ADD_FUNC(lambda_9),
						     ADD_FUNC(bind_1),
						     ADD_FUNC(bind_2),
						     ADD_FUNC(bind_4),
						     ADD_FUNC(bind_5),
						     ADD_FUNC(trans_1),
						    ADD_FUNC(trans_2),
						    ADD_FUNC(trans_3),
						    ADD_FUNC(trans_4),
						    ADD_FUNC(trans_5),
						    ADD_FUNC(trans_6),
						    ADD_FUNC(trans_7),
						    ADD_FUNC(trans_8),
						    ADD_FUNC(trans_9),
						    ADD_FUNC(functor_1),
						    ADD_FUNC(functor_2),
						    ADD_FUNC(functor_3),
						    ADD_FUNC(functor_4),
						    ADD_FUNC(functor_5),
						    ADD_FUNC(functor_6),
						    ADD_FUNC(functor_7),
						    ADD_FUNC(functor_8),
						    ADD_FUNC(functor_9),
						    ADD_FUNC(apf_1),
						    ADD_FUNC(apf_2),
						    ADD_FUNC(apf_3),
						    ADD_FUNC(apf_4),
						     //ADD_FUNC(apf_5),
						    ADD_FUNC(apf_6),
						    ADD_FUNC(apf_7),
						    ADD_FUNC(apf_8),
						    ADD_FUNC(apf_9),
						    ADD_FUNC(apf_10),
						    ADD_FUNC(apf_11),
						    ADD_FUNC(apf_12),
						    ADD_FUNC(m_0),
						    ADD_FUNC(m_1),
						    ADD_FUNC(m_2),
						    ADD_FUNC(m_3),
						    ADD_FUNC(m_4),
							ADD_FUNC(m_5),
							ADD_FUNC(m_6),
							ADD_FUNC(m_7),
							ADD_FUNC(m_8),
							ADD_FUNC(m_9),
							ADD_FUNC(m_10),
							ADD_FUNC(m_11),
							ADD_FUNC(m_12),
							ADD_FUNC(m_13),
						    ADD_FUNC(m_14)};  

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

  auto run_all = [=] (const std::string& ps)  {       
    size_t count = 1;
    for (auto& p : M) {
      std::cout << ps << " [" << count++ << "] " <<p.first << std::endl;
      p.second();
      std::cout << std::endl << "     *done*     " << std::endl;
      
    }
    std::cout << std::endl;
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
      std::cout << "     *done*     " << std::endl;
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
    if (in == "all") {
      run_all(ps);
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
  int index = 1;
  while (argv[index]) {
    run_cmd(argv[index]);
    index++;
  }
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

