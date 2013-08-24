#include "proto.hpp"
#include "show.hpp"
#include "state.hpp"

typedef state_computation<int, std::list<int>> stack_comp;
typedef std::list<int>  istack;

int st_0()
{
	istack L = {1,2,3,4};
	
	stack_comp pop = [] (istack s) {
		auto val = s.front();
		s.pop_front();
		return state_tuple<int, istack>(val, s);
	};
	
	state <int, istack> SM(pop);
	std::cerr << SM << std::endl;
	auto R = runState(SM, L);
	std::cerr << R << std::endl;
	
	std::function<stack_comp(int)> push = [](int val) {
		return [val] (istack s) {
			s.push_front(val);
			return state_tuple<int, istack>(s);
		};
	};

	state <int, istack> SP(push(4987678));
	std::cerr << SP << std::endl;
	auto S = runState(SP, L);
	std::cerr << S << std::endl;

	return 0;
}

int st_1()
{
	std::default_random_engine de;

	std::uniform_int_distribution<int> di(10, 20);
	std::cerr << di << std::endl;

	state_computation<int,std::uniform_int_distribution<int>>  getrand = [&de] (std::uniform_int_distribution<int> s) {
		auto val = s(de);
		return state_tuple<int, std::uniform_int_distribution<int> >(val, s);
	}; 

	state <int, std::uniform_int_distribution<int>> ST(getrand);
	int n = 10;
	auto S = runState(ST, di);
	std::cerr << "iter : " << n << " " << S << std::endl;;
	while ( n-- > 0) {
		S = runState(ST, S.state().first);
		std::cerr << "iter : " << n << " " << S << std::endl;;
	}
	return 0;
}
