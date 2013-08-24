#ifndef h__state_monad_h
#define h_state_monad__h
#include "state.hpp"

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
	// this doesn't work (yet)
	template<typename S, typename A, typename B>
	static std::function<state<B,S>(state<A,S>)>  fmap (std::function<B(A)> f) {
		return [f] (state<A,S> M) {
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


#endif
