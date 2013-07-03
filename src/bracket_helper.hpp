#ifndef h__bracket_helper__h
#define h__bracket_helper__h


template <int n, template<typename Tx, typename... D> class Cont, typename F, typename T1, typename T2=T1, typename T3=T2, typename T4=T3>
struct bracket_helper {

};


template <template<typename Tx, typename... D> class Cont, typename F, typename T1> 
struct bracket_helper<1, Cont, F, T1> {

	static auto bracket(F cf, Cont<T1> L1) {
		return bracket(cf)(L1);
	}

	static auto bracket (F f) {
		auto cffl = applicative_functor<Cont>::pure(f);
		return [cffl](Cont<T1> L) { 
			typedef decltype(f(T1())) ret_t;
			applicative_functor<Cont> APF;
			auto C = APF.template apply<T1, ret_t>(cffl)(L);
			return C;
		};
	};
  
};


template<template<typename Tx, typename... D> class Cont, typename F, typename T1, typename T2>
struct bracket_helper<2, Cont, F, T1, T2> {
	static auto bracket(F cf, Cont<T1> L1, Cont<T2> L2) {
		return bracket(cf)(L1)(L2);
	}

	static auto bracket(F cf) {
		typedef decltype(cf(T1())(T2())) ret_t;
		return [cf] (Cont<T1> L1) {
			auto C  = bracket_helper<1, Cont, F, T1>::bracket(cf)(L1);
			return [C](Cont<T2> L2) { 
				applicative_functor<Cont> APF;
				auto J  = APF.template apply<T2, ret_t>(C)(L2);
				return J;
			};
		};
	};
};


template<template<typename Tx, typename... D> class Cont, typename F, typename T1, typename T2, typename T3>
struct bracket_helper<3, Cont, F, T1, T2, T3> {

	static auto bracket(F cf, Cont<T1> L1, Cont<T2> L2, Cont<T3> L3) {
		return bracket(cf)(L1)(L2)(L3);
	}

	static auto bracket(F cf) {
		typedef decltype(cf(T1())(T2())(T3())) ret_t;
		return [cf] (Cont<T1> L1) {
			auto H    = bracket_helper<2, Cont, decltype(cf), T1, T2>::bracket(cf)(L1);
			return [H] (Cont<T2> L2) {
				auto J = H(L2);
				return [J](Cont<T3> L3) { 
					applicative_functor<Cont> APF;
					auto K    = APF.template apply<T3, ret_t>(J)(L3);
					return K;
				};
			};
		};
	};
};

template<template<typename Tx, typename... D> class Cont, typename F, typename T1, typename T2, typename T3, typename T4>
struct bracket_helper<4, Cont, F, T1, T2, T3, T4> {

	static auto bracket(F cf, Cont<T1> L1, Cont<T2> L2, Cont<T3> L3, Cont<T4> L4) {
		return bracket(cf)(L1)(L2)(L3)(L4);
	}

	static auto bracket(F cf) {
		typedef decltype(cf(T1())(T2())(T3())(T4())) ret_t;
		return [cf] (Cont<T1> L1) {
			auto H    = bracket_helper<3, Cont, decltype(cf), T1, T2,T3>::bracket(cf)(L1);
			return [H] (Cont<T2> L2) {
				auto J1 = H(L2);
				return [J1](Cont<T3> L3) { 
					auto J = J1(L3);
					return [J] (Cont<T4> L4) {
						applicative_functor<Cont> APF;
						auto K    = APF.template apply<T4, ret_t>(J)(L4);
						return K;
					};
				};
			};
		};
	};
};



#endif
