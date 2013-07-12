#include "proto.hpp"
#include "monad.hpp"
#include "show.hpp"


template<typename A, typename F>
auto mapM (F f, std::forward_list<A> L) -> decltype(f(A()))
{
	typedef typename decltype(f(A()))::value_type ret_t;
	L.reverse();
	auto concat = [] (std::forward_list<ret_t> L, std::forward_list<ret_t> R) { 
		L.splice_after(L.before_begin(), R); 
		return L;
	};
	auto op     = std::bind(concat, std::placeholders::_1, std::bind(f, std::placeholders::_2));
	return std::accumulate(L.begin(), L.end(), std::forward_list<ret_t>(), op);;
}

int m_0()
{
	auto show   = [] (int v) { std::cout << v << ","; return v;};
	typedef std::forward_list<int> list_t;
	list_t L = {1,-6,23,78,45,13};
	auto l1  = [] (int y) {return list_t ({2*y+1, -y, -2*y + 1});};
	std::cerr << "before ...." << std::endl;
	map(show, L);
	std::cerr << std::endl << "after ...." << std::endl;
	map(show, mapM(l1, L));
	return 0;
}

int m_1()
{  
	auto show   = [] (std::tuple<int,char> v) { std::cout << v << ","; return v;};
	static char digits[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
	typedef std::forward_list<std::tuple<int, char>> list_t;
	auto op  = [] (int y) {return list_t({std::make_tuple(y, digits[abs(y)%10])});};
	map(show, mapM(op, std::forward_list<int>({1,-6,23,78,45,13})));
	
	auto res = map(op, std::forward_list<int>({1,-6,23,78,45,13}));
	std::cout << std::endl <<  "-----------------" << std::endl;
	for (auto& el : res) {
		std::cout << "[";
		map(show, el);
		std::cout << "], ";
	}
	return 0;
}


int m_2()
{
	auto show   = [] (int v) { std::cout << v << ","; return v;};
	std::forward_list<int> L = {1,3,45,78};
	auto op = [] (int x) {
		std::forward_list<int> R = {x , -x};
		return R;
	};

	auto R = monad<std::forward_list>::bind<int,int>(L)(op);
	map (show, R);
	return 0;
}

int m_3()
{
	
	std::function<std::function<int(int)>(int)> plus = [] (int x) {
		return [x] (int y) {
			return x + y;
		};
	};
	
	std::function<std::function<int(int)>(int)> mult = [] (int x) {
		return [=] (int y) {
			return x * y;
		};
	};

	auto f = mult(5);
	auto g = plus(3);
	auto res = functor<unary_op>::fmap(f, g);
	std::cout << "res(8) : " << res(8) << std::endl;
	
	std::function<int(int,int)> uncur = [plus] (int x, int y) { return plus(x)(y);};
	auto res2 = monad<unary_op>::bind(f, uncur);
	std::cout << " res (10) : " << res(10) << std::endl;
	return 0;
}

int m_4()
{
	
	std::forward_list<int> L = {1,2};
	auto op = [](int x) {
		std::forward_list<char> R = {'a' , 'b'};
		return R;
	};

	auto R = monad<std::forward_list>::bind<int,char>(L)(op);
	for (auto& v: R) {
		std::cout << v << ",";
	}
	std::cout << std::endl;
	
	return 0;
}

int m_5()
{
	std::forward_list<int> L = {1,2};

	auto v1 = monad<std::forward_list>::pure(-34);
	std::cout << v1 << std::endl;

	auto v2 = monad<std::forward_list>::mreturn(3400);
	std::cout << v2 << std::endl;
	
	return 0;
}

//
// std::forward_list
//monod law [m >>= return = m]
//
int m_6()
{
	std::forward_list<int> L = {1,2};
	auto op = [](int x) {
		return monad<std::forward_list>::pure(x);
	};
	auto R = monad<std::forward_list>::bind<int,int>(L)(op);
	std::cout << R << std::endl;
	return 0;
}

//
// std::forward_list
// monad law : [return a >>= k = k]
//
int m_7()
{
	auto op = [](int x) {
		return monad<std::forward_list>::pure(2 * x);
	};
	auto v = 7896;
	auto L = monad<std::forward_list>::bind<int,int>(monad<std::forward_list>::pure(v))(op);
	auto R = op(v);
	std::cerr << " L = R : " << L << " = " << R << std::endl;
	return 0;
}
//
// std::forward_list
// monad law : [m >>= (\x->k x >>= h) = (m >>= k) >>= h]
//
int m_8()
{
	std::forward_list<int> m = {1,2};
	auto k = [](int x) {
		return monad<std::forward_list>::pure(2 * x + 9);
	};
	auto h = [](int x) {
		return monad<std::forward_list>::pure((x - 9)/2);
	};
	auto R1 = monad<std::forward_list>::bind<int,int>(m)(k);	
	auto R2 = monad<std::forward_list>::bind<int,int>(R1)(h);	
	std::cerr << " m  " << m << std::endl;
	std::cerr << " R1 " << R1 << std::endl;
	std::cerr << " R2 " << R2 << std::endl;

	// [\x->k x >>= h]
	std::function < std::forward_list<int>(int) > lambda = [k,h](int x) {
		return  monad<std::forward_list>::bind<int,int>(k(x))(h);	
	};
	auto R3 = monad<std::forward_list>::bind<int,int>(m)(lambda);	
	std::cerr << "R3 : " << R3 << std::endl;
	return 0;
}


//
// std::list
//monod law [m >>= return = m]
//
int m_9()
{
	std::list<int> L = {1,2};
	auto op = [](int x) {
		return monad<std::list>::pure(x);
	};
	auto R = monad<std::list>::bind<int,int>(L)(op);
	std::cout << R << std::endl;
	return 0;
}

//
// std::list
// monad law : [return a >>= k = k]
//
int m_10()
{
	auto op = [](int x) {
		return monad<std::list>::pure(2 * x);
	};
	auto v = 7896;
	auto L = monad<std::list>::bind<int,int>(monad<std::list>::pure(v))(op);
	auto R = op(v);
	std::cerr << " L = R : " << L << " = " << R << std::endl;
	return 0;
}
//
// std::list
// monad law : [m >>= (\x->k x >>= h) = (m >>= k) >>= h]
//
int m_11()
{
	std::list<int> m = {1,2};
	auto k = [](int x) {
		return monad<std::list>::pure(2 * x + 9);
	};
	auto h = [](int x) {
		return monad<std::list>::pure((x - 9)/2);
	};
	auto R1 = monad<std::list>::bind<int,int>(m)(k);	
	auto R2 = monad<std::list>::bind<int,int>(R1)(h);	
	std::cerr << " m  " << m << std::endl;
	std::cerr << " R1 " << R1 << std::endl;
	std::cerr << " R2 " << R2 << std::endl;

	// [\x->k x >>= h]
	std::function < std::list<int>(int) > lambda = [k,h](int x) {
		return  monad<std::list>::bind<int,int>(k(x))(h);	
	};
	auto R3 = monad<std::list>::bind<int,int>(m)(lambda);	
	std::cerr << "R3 : " << R3 << std::endl;
	return 0;
}

//
// std::shared_ptr
//monod law [m >>= return = m]
//
int m_12()
{
	std::shared_ptr<int> L = monad<std::shared_ptr>::pure(10);
	auto op = [](int x) {
		return monad<std::shared_ptr>::pure(x);
	};
	auto R = monad<std::shared_ptr>::bind<int,int>(L)(op);
	std::cout << R << std::endl;
	std::cout << op(10) << std::endl;
	return 0;
}

//
// std::shared_ptr
// monad law : [return a >>= k = k]
//
int m_13()
{
	auto op = [](int x) {
		return monad<std::shared_ptr>::pure(2 * x);
	};
	auto v = 7896;
	auto L = monad<std::shared_ptr>::bind<int,int>(monad<std::shared_ptr>::pure(v))(op);
	auto R = op(v);
	std::cerr << " L = R : " << L << " = " << R << std::endl;
	return 0;
}
//
// std::shared_ptr
// monad law : [m >>= (\x->k x >>= h) = (m >>= k) >>= h]
//
int m_14()
{
	std::shared_ptr<int> m = monad<std::shared_ptr>::pure(10);
	auto k = [](int x) {
		return monad<std::shared_ptr>::pure(2 * x + 9);
	};
	auto h = [](int x) {
		return monad<std::shared_ptr>::pure((x - 9)/2);
	};
	auto R1 = monad<std::shared_ptr>::bind<int,int>(m)(k);	
	auto R2 = monad<std::shared_ptr>::bind<int,int>(R1)(h);	
	std::cerr << " m  " << m << std::endl;
	std::cerr << " R1 " << R1 << std::endl;
	std::cerr << " R2 " << R2 << std::endl;

	// [\x->k x >>= h]
	std::function < std::shared_ptr<int>(int) > lambda = [k,h](int x) {
		return  monad<std::shared_ptr>::bind<int,int>(k(x))(h);	
	};
	auto R3 = monad<std::shared_ptr>::bind<int,int>(m)(lambda);	
	std::cerr << "R3 : " << R3 << std::endl;
	return 0;
}


