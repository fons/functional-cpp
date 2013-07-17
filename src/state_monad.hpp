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

#endif
