#ifndef h__curry_helper__h
#define h__curry_helper__h

template <int n, typename F, typename T1, typename T2=T1, typename T3=T2, typename T4=T3, typename T5=T4>
struct curry_helper {
  auto operator()(F op);
};

template<typename F, typename T1>
struct curry_helper<1,F,T1> {
  //typedef std::function<R (T1)> f1_t;
  auto operator() (F op) {
    return [=] (T1 x) {return op(x);};
  }
};

template<typename F, typename T1, typename T2>
struct curry_helper<2,F, T1,T2> {
  //typedef std::function<R(T2)>     f2_t;
  //typedef std::function<f2_t (T1)> f1_t;
  auto operator() (F op) {
    return [=] (T1 x) {
      return [=] (T2 y) {
	return op(x,y);
      };
    };
  };
};

template<typename F, typename T1, typename T2, typename T3>
struct curry_helper<3,F,T1,T2,T3> {
  //typedef std::function<R(T3)>        f3_t;
  //typedef std::function<f3_t(T2)>     f2_t;
  //typedef std::function<f2_t (T1)>    f1_t;
  auto operator() (F op) {
    return [=] (T1 x) {
      return [=] (T2 y) {
	return [=] (T3 z) {
	  return op(x,y,z);
	};
      };
    };
  };
};

template<typename F, typename T1, typename T2, typename T3, typename T4>
struct curry_helper<4,F,T1,T2,T3, T4> {
  //typedef std::function<R(T4)>        f4_t;
  //typedef std::function<f4_t(T3)>     f3_t;
  //typedef std::function<f3_t(T2)>     f2_t;
  //typedef std::function<f2_t (T1)>    f1_t;
  auto operator() (F op) {
    return [=] (T1 x) {
      return [=] (T2 y) {
	return [=] (T3 z) {
	  return [=] (T4 w) {
	    return op(x,y,z,w);
	  };
	};
      };
    };
  };
};

template<typename F, typename T1, typename T2, typename T3, typename T4, typename T5>
struct curry_helper<5,F,T1,T2,T3, T4, T5> {
  //typedef std::function<R(T5)>        f5_t;
  //typedef std::function<f5_t(T4)>     f4_t;
  //typedef std::function<f4_t(T3)>     f3_t;
  //typedef std::function<f3_t(T2)>     f2_t;
  //typedef std::function<f2_t (T1)>    f1_t;
  auto operator() (F op) {
    return [=] (T1 x) {
      return [=] (T2 y) {
	return [=] (T3 z) {
	  return [=] (T4 w) {
	    return [=] (T5 p) {
	      return op(x,y,z,w,p);
	    };
	  };
	};
      };
    };
  };
};


#endif
