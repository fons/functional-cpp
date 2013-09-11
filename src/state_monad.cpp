#include "proto.hpp"
#include "show.hpp"
#include "functor.hpp"
#include "applicative_functor.hpp"
#include "state.hpp"
#include "monad.hpp"
#include "state_monad.hpp"

template <>
struct functor<state> {
	
	template<typename S, typename A, typename B>
	static state<B,S>  fmap (std::function<B(A)> f, state<A,S> M) {
		state_computation<B,S> comp =[f,&M](S s) {
			auto next       = runState(M, s);
			auto value      = next.value();
			auto new_state  = next.state().first;
			if (value.second) {
				return state_tuple<B, S>(f(value.first), new_state);
			}
			return state_tuple<B, S>(new_state);
		};
		state <B, S> ST(comp);
		return ST;	
	}

	template<typename S, typename A, typename lambda>
	static auto  fmap (lambda F, state<A,S> M) -> state<decltype(F(A())),S> {
		state_computation<decltype(F(A())),S> comp =[F,&M](S s) {
			auto next       = runState(M, s);
			auto value      = next.value();
			auto new_state  = next.state().first;
			if (value.second) {
				return state_tuple<decltype(F(A())), S>(F(value.first), new_state);
			}
			return state_tuple<decltype(F(A())), S>(new_state);
		};
		state <decltype(F(A())), S> ST(comp);
		return ST;	
	}


	// this doesn't work (yet)
	template<typename S, typename A, typename B>
	static std::function<state<B,S>(state<A,S>)>  fmap (std::function<B(A)> f) {
		return [&f] (state<A,S> M) {
			/*
			state_computation<B,S> comp =[f,&M](S s) {
				auto next       = runState(M, s);
				auto value      = next.value();
				auto new_state  = next.state().first;
				if (value.second) {
					return state_tuple<B, S>(f(value.first), new_state);
				}
				return state_tuple<B, S>(new_state);
			};
			state <B, S> ST(comp);
			return ST;	
			*/
			return fmap<S,A,B>(f, M);
		};
	}

};
template <>
struct applicative_functor<state> : public functor <state>
{

	template <typename S, typename A> static  state<A,S> pure(A val) {
		state_computation<A,S> comp =[val](S s) {
			return state_tuple<A, S>(val, s);
		};
		state <A, S> ST(comp);
		return ST;	
	}
	
	template<typename S, typename A, typename B>
	static state<B,S> apply ( state<std::function<B(A)>, S> F, state<A,S> M) {
		
		state_computation<B,S> comp =[F,&M](S s) {
			auto rs1  = runState(F, s);
			auto resv = rs1.value();
			if (resv.second) {
				std::function<B(A)> f = resv.first;
				auto MT         = functor<state>::fmap<S,A,B>(f, M);
				auto rs2        = runState(MT, s);
				auto value      = rs2.value();
				auto new_state  = rs2.state().first;
				if (value.second) {
					return state_tuple<B, S>(value.first, new_state);
				}
				return state_tuple<B, S>(new_state);
			}
			return state_tuple<B, S>(s); 
		};
		state <B, S> ST(comp);
		return ST;	
	}

};

template<> struct monad<state> : public applicative_functor<state> {

	template<typename S, typename A, typename B>
	static state<B,S> bind(state<A,S>& M, std::function< state<B,S> (A)>& f) {
		state_computation<B,S> comp =[&f,&M](S s) {
			auto res             = runState(M, s);
			state<B,S> state_g   = f (res.value().first);
			return runState(state_g, res.state().first);
		};
		return state<B,S> (comp);
	};

	template <typename S, typename A> static state<A,S> mreturn (A val) {
		return applicative_functor<state>::pure<S,A>(val);
	}

};


//////////////////////////////
typedef state_computation<int, std::list<int>> stack_comp;
typedef std::list<int>  istack;

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

int stm_2()
{
	istack L = {1,2,3,4};
	
	auto ST = applicative_functor<state>::pure<istack>(5);
	std::cerr << ST << std::endl;
	
	auto S = runState(ST, L);
	std::cerr << S << std::endl;;

	return 0;
}


int stm_3()
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

	auto F = applicative_functor<state>::pure<istack>(f);
	std::cerr << F << std::endl;

	auto r1 = runState(SM, L);
	std::cerr << r1 << std::endl;;
	auto SC = applicative_functor<state>::apply<istack, int, char>(F, SM);
	auto r2 = runState(SC, L);
	std::cerr << r2 << std::endl;;

	return 0;
}


int stm_4()
{
	istack L = {1,2,3,4};


	std::function< state<int, istack> (int)> f = [] (int v) {

		std::function<stack_comp(int)> push = [](int val) {
			return [val] (istack s) {
				s.push_front(val);
				return state_tuple<int, istack>(s);
			};
		};

		stack_comp pop = [] (istack s) {
			auto val = s.front();
			s.pop_front();
			return state_tuple<int, istack>(val, s);
		};

		std::cerr << "v : " << v << std::endl;
		if (v > 3) {
			state <int, istack> SP(push(v));		
			return SP;
		}

		state <int, istack> SP(pop);		
		return SP;
	};

	stack_comp pop = [] (istack s) {
		auto val = s.front();
		s.pop_front();
		return state_tuple<int, istack>(val, s);
	};
	
	state <int, istack> SM(pop);


	auto R = monad<state>::bind<istack,int,int>(SM, f);
	std::cerr << R << std::endl;

	auto r0 = runState(SM, istack{1,4,5,3,1,8});
	std::cerr << r0 << std::endl;

	auto r1 = runState(R, istack{1,4,5,3,1,8});
	std::cerr << r1 << std::endl;

	return 0;
}

int stm_5()
{
	istack L = {1,2,3,4};

	std::function<stack_comp()> pop = []() {
		return [] (istack s) {
			auto val = s.front();
			s.pop_front();
			return state_tuple<int, istack>(val, s);
		};
	};

	std::function<stack_comp(int)> push = [](int val) {
		return [val] (istack s) {
			s.push_front(val);
			return state_tuple<int, istack>(s);
		};
	};

	std::function< state<int, istack> (int)> f = [pop] (int v) {
		state <int, istack> SP(pop());		
		return SP;
	};

	std::function< state<int, istack> (int)> g = [push] (int v) {
		state <int, istack> SP(push(498));		
		return SP;
	};

	state <int, istack> SP(pop());
	auto r1 = runState(SP,L);
	std::cerr << r1 << std::endl;

	state<int,istack> R = monad<state>::bind<istack,int,int>(SP, f);
	auto res = runState(R,L);
	std::cerr << res << std::endl;

	state<int,istack> R2 = monad<state>::bind<istack,int,int>(R, g);
	auto res2 = runState(R2,L);
	std::cerr << res2 << std::endl;
	auto res21 = runState(R2,L);
	std::cerr << res21 << std::endl;

	state<int,istack> R3 = monad<state>::bind<istack,int,int>(R2, g);
	auto res3 = runState(R3,L);
	std::cerr << "res3 " << res3 << std::endl;
	auto res31 = runState(R3,L);
	std::cerr << "res31 " << res3 << std::endl;
	int n = 10;
	while (n--) {
		state<int,istack> R4 = monad<state>::bind<istack,int,int>(R3, g);
		auto res4 = runState(R4,L);
		std::cerr << "res4 :" << res4 << std::endl;
	}
	return 0;
}
//
// state monad
// monad law : [return a >>= k = k]
//
int stm_6()
{
	istack L = {1,2,3,4};

	std::function<stack_comp(int)> push = [](int val) {
		return [val] (istack s) {
			s.push_front(val);
			return state_tuple<int, istack>(s);
		};
	};

	std::function< state<int, istack> (int)> g = [push] (int v) {
		state <int, istack> SP(push(498));		
		return SP;
	};

	int a = 897;
	state<int,istack> R = monad<state>::mreturn<istack,int>(a);
	state<int,istack> R1 = monad<state>::bind<istack,int,int>(R, g);
	auto res = runState(R1,L);
	std::cerr << res << std::endl;

	state<int,istack> R2 = g(a);
	auto res2 = runState(R2,L);
	std::cerr << res2 << std::endl;


	std::function<stack_comp()> pop = []() {
		return [] (istack s) {
			auto val = s.front();
			s.pop_front();
			return state_tuple<int, istack>(val, s);
		};
	};
	std::function< state<int, istack> (int)> f = [pop] (int v) {
		state <int, istack> SP(pop());		
		return SP;
	};

	state<int,istack> RP = monad<state>::mreturn<istack,int>(a);
	state<int,istack> R12 = monad<state>::bind<istack,int,int>(RP, f);
	auto res12 = runState(R12,L);
	std::cerr << res12 << std::endl;

	state<int,istack> R22 = f(a);
	auto res22 = runState(R22,L);
	std::cerr << res22 << std::endl;

	return 0;
}

int stm_7()
{
	istack L = {1,2,3,4};

	std::function<stack_comp()> pop = []() {
		return [] (istack s) {
			auto val = s.front();
			s.pop_front();
			return state_tuple<int, istack>(val, s);
		};
	};

	std::function<stack_comp(int)> push = [](int val) {
		return [val] (istack s) {
			s.push_front(val);
			return state_tuple<int, istack>(s);
		};
	};

	std::function< state<int, istack> (int)> f = [pop] (int v) {
		state <int, istack> SP(pop());		
		return SP;
	};

	
	std::function< state<int, istack> (int)> g1 = [push] (int v) {
		state <int, istack> SP(push(498));		
		return SP;
	};

	std::function<std::function< state<int, istack> (int)>(int)> g = [push] (int val) {
		std::function< state<int, istack> (int)> gf = [val, push] (int v) {
			state <int, istack> SP(push(val));		
			return SP;
		};
		return gf;
	};

	state <int, istack> SP(pop());
	auto r1 = runState(SP,L);
	int a = r1.value().first;

	state<int,istack> R = monad<state>::bind<istack,int,int>(SP, f);
	auto r2 = runState(R,L);
	int b = r2.value().first;

	std::function< state<int, istack> (int)> lam1 = g(a);
	state<int,istack> R2 = monad<state>::bind<istack,int,int>(R, lam1);
	std::function< state<int, istack> (int)> lam2 = g(b);
	state<int,istack> R3 = monad<state>::bind<istack,int,int>(R2, lam2);
	auto res3 = runState(R3,L);
	std::cerr << res3 << std::endl;

	return 0;
}

//
//
// monad law : [m >>= (\x->k x >>= h) = (m >>= k) >>= h]
//
//

int stm_8()
{
	istack L = {1,2,3,4};

	std::function<stack_comp()> pop = []() {
		return [] (istack s) {
			auto val = s.front();
			s.pop_front();
			return state_tuple<int, istack>(val, s);
		};
	};

	std::function<stack_comp(int)> push = [](int val) {
		return [val] (istack s) {
			s.push_front(val);
			return state_tuple<int, istack>(s);
		};
	};

	std::function< state<int, istack> (int)> k = [pop] (int v) {
		state <int, istack> SP(pop());		
		return SP;
	};
	
	
	std::function< state<int, istack> (int)> h= [push] (int v) {
		state <int, istack> SP(push(498));		
		return SP;
	};
	
	std::function< state<int, istack> (int)> lambda = [&h,&k] (int v) {
		auto sp = new state<int, istack>(k(v));
	    state<int, istack>& KX = *sp;
		return monad<state>::bind<istack,int,int>(KX, h);
	};	

	state <int, istack> M(pop());
	state<int,istack> LE = monad<state>::bind<istack,int,int>(M, lambda);
	auto lhs = runState(LE,L);
	std::cerr << lhs << std::endl;
	
	state<int,istack> R = monad<state>::bind<istack,int,int>(M, k);
	state<int,istack> R2 = monad<state>::bind<istack,int,int>(R, h);
	auto rhs = runState(R2,L);
	std::cerr << rhs << std::endl;

	return 0;
}


int stm_9()
{
	typedef std::uniform_int_distribution<int> idist;
	std::default_random_engine de;


	state_computation<int,std::uniform_int_distribution<int>>  getrand = [&de] (std::uniform_int_distribution<int> s) {
		auto val = s(de);
		return state_tuple<int, std::uniform_int_distribution<int> >(val, s);
	}; 

	state <int, idist> ST(getrand);
	
	std::function<state<int,idist>(int)> f = [&de, &ST](int val) {
		std::cerr << val <<std::endl; 
		return ST;
	};

	auto S1 = monad<state>::bind<idist,int,int>(ST,f);
	auto S2 = monad<state>::bind<idist,int,int>(S1,f);
	auto S3 = monad<state>::bind<idist,int,int>(S2,f);
	auto S4 = monad<state>::bind<idist,int,int>(S3,f);
	auto S5 = monad<state>::bind<idist,int,int>(S4,f);
	auto S6 = monad<state>::bind<idist,int,int>(S5,f);
	auto S7 = monad<state>::bind<idist,int,int>(S6,f);
	auto S8 = monad<state>::bind<idist,int,int>(S7,f);
	auto Sf = monad<state>::bind<idist,int,int>(S8,f);

	auto S = runState(Sf, idist (10, 20));
	std::cerr << S << std::endl;;
	S = runState(Sf, idist (100, 200));
	std::cerr << S << std::endl;;

	return 0;
}

int stm_10()
{
	typedef std::list<int>                     icont_t;
	typedef std::uniform_int_distribution<int> idist_t;
	typedef std::pair<icont_t, idist_t>        state_t;
	typedef state_tuple<int, state_t>          state_tuple_t;

	std::default_random_engine de((unsigned int)time(0));

	state_computation<int,state_t>  getrand = [&de] (state_t s) {
		auto val = s.second(de);
		s.first.push_back(val);
		return state_tuple_t(val, s);
 	}; 
	
	std::function<state_computation<int,state_t> (int, int)> getrand2 = [&de](int f, int t) {
		return [&de,f,t] (state_t s) {
			auto val = s.second(de);
			return state_tuple_t(val, std::make_pair(s.first, idist_t(f,t)));
		}; 
	};


	state <int, state_t> ST(getrand);

	std::function<state<int,state_t>(int)> f = [&ST,&getrand2](int val) {
		std::cerr << val <<std::endl; 
		if (val % 7 == 0) {
			return state<int,state_t> (getrand2(10000,11456));
		}
		return ST;
	};

	auto S1 = monad<state>::bind<state_t,int,int>(ST,f);
	auto S2 = monad<state>::bind<state_t,int,int>(S1,f);
	auto S3 = monad<state>::bind<state_t,int,int>(S2,f);
	auto S4 = monad<state>::bind<state_t,int,int>(S3,f);
	auto S5 = monad<state>::bind<state_t,int,int>(S4,f);
	auto S6 = monad<state>::bind<state_t,int,int>(S5,f);
	auto S7 = monad<state>::bind<state_t,int,int>(S6,f);
	auto S8 = monad<state>::bind<state_t,int,int>(S7,f);
	auto S9 = monad<state>::bind<state_t,int,int>(S8,f);
	auto S10 = monad<state>::bind<state_t,int,int>(S9,f);
	auto S11 = monad<state>::bind<state_t,int,int>(S10,f);
	auto S12 = monad<state>::bind<state_t,int,int>(S11,f);
	auto Sf = monad<state>::bind<state_t,int,int>(S12,f);

	auto S = runState(Sf, std::make_pair(icont_t(), idist_t(10,20)));
	std::cerr << S << std::endl;;
	S = runState(Sf, std::make_pair(icont_t(), idist_t(100,200)));
	std::cerr << S << std::endl;;

	return 0;
}



