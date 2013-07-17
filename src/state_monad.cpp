#include "proto.hpp"
#include "show.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "state_monad.hpp"

typedef state_computation<int, std::list<int>> stack_comp;
typedef std::list<int>  istack;


template <>
struct applicative_functor<state> : public functor <state>
{

	template <typename A, typename S> static  state<A,S> pure(A val) {
		state_computation<A,S> comp =[val](S s) {
			return state_tuple<A, S>(val, s);
		};
		state <A, S> ST(comp);
		return ST;	
	}
	
	template<typename S, typename A, typename B>
	static state<B,S> apply ( std::function<B(A)> F, state<A,S> M) {
		state_computation<B,S> comp =[F,&M](S s) {
			auto res       = runState(F, s);
			auto resv      = res.value();
			if (resv.second) {
				auto f = resv.first;
				auto st = functor<state>::fmap<A,S>(f, M);
				return st;
			}
			return state_tuple<B, S>(s); 
		};
		state <B, S> ST(comp);
		return ST;	
	}

};

int stm_3()
{
	istack L = {1,2,3,4};

	std::function<char(int)> f = [] (int i) {
		if (i < 3) {
			return 'A';
		}
		return 'Z';
	};

	auto F = applicative_functor<state>::pure<int, istack>(f);
	std::cerr << F << std::endl;

	//auto S = runState(ST, L);
	//std::cerr << S << std::endl;;

	return 0;
}

int stm_2()
{
	istack L = {1,2,3,4};

	auto ST = applicative_functor<state>::pure<int, istack>(5);
	std::cerr << ST << std::endl;

	auto S = runState(ST, L);
	std::cerr << S << std::endl;;

	return 0;
}

int stm_0()
{
	std::default_random_engine de;
	std::uniform_int_distribution<int> di(10, 20);
	state_computation<int,std::uniform_int_distribution<int>>  getrand = [&de] (std::uniform_int_distribution<int> s) {
		auto val = s(de);
		return state_tuple<int, std::uniform_int_distribution<int> >(val, s);
	}; 

	state <int, std::uniform_int_distribution<int>> ST(getrand);

	std::function<char(int)> f = [] (int i) {
		if (i < 15) {
			return 'A';
		}
		return 'Z';
	};


	int n = 10;
	while (n-- > 0) { 
		auto SRT = functor<state>::fmap(f, ST);
		auto S = runState(SRT, di);
		std::cerr << S << std::endl;;
	}

	//auto SRT = functor<state>::fmap<std::uniform_int_distribution<int>>(f)(ST);
	auto SRT = functor<state>::fmap<std::uniform_int_distribution<int>>(f, ST);
	auto S = runState(SRT, di);
	std::cerr << S << std::endl;;

	return 0;
}



int stm_1()
{
	istack L = {1,2,3,4};

	std::function<char(int)> f = [] (int i) {
		if (i < 3) {
			return 'A';
		}
		return 'Z';
	};

	
	stack_comp pop = [] (istack s) {
		auto val = s.front();
		s.pop_front();
		return state_tuple<int, istack>(val, s);
	};
	
	state <int, istack> SM(pop);
	std::cerr << SM << std::endl;
	state <char, istack> SMT = functor<state>::fmap(f, SM);
	std::cerr << SMT << std::endl;
	
	auto R = runState(SMT, L);
	std::cerr << R << std::endl;


	std::function<stack_comp(int)> push = [](int val) {
		return [val] (istack s) {
			s.push_front(val);
			return state_tuple<int, istack>(s);
		};
	};

	state <int, istack> SP(push(498));
	std::cerr << SP << std::endl;
	auto S = runState(SP, L);
	std::cerr << S << std::endl;

	state <char, istack> SPT = functor<state>::fmap(f, SP);
	auto st = runState(SPT, L);
	std::cerr << st << std::endl;
	
	return 0;
}

