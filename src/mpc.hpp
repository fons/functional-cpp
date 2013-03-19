#ifndef h__mpc__h
#define h__mpc__h

template <typename a>
using ppair_t = std::tuple<a, std::string>;

template<typename a>
std::ostream& operator<<(std::ostream& strm, const ppair_t<a>& p)
{
  return strm << "(" << std::get<0>(p) << "," << std::get<1>(p) << ")";
}

template <typename a>
using plist_t = std::list<ppair_t<a>>;

template<typename a>
std::ostream& operator<<(std::ostream& strm, const plist_t<a>& L)
{
  strm<<"[";
  size_t l = 0;
  for (auto& v: L) {
    strm << v; 
    if (++l % 10) {
      strm << ",";
    }
    else {
      strm << std::endl;
    }
  }
  return strm<<"]";
}


template <typename a>
using parser_t = std::function< plist_t<a> (std::string)>;

template<typename a>
std::ostream& operator<<(std::ostream& strm, const parser_t<a>& F)
{
  return strm << "(std::string) -> [(" << typeid(a).name() << ", std::string)]";
}

template <typename a>
struct Parser 
{
  
  Parser(const parser_t<a>& p) : p(p){}
  Parser(const Parser& o) : p(o.p){}
  const Parser& operator=(const Parser& o) {
    if (this != &o) {
      p = o.p;
    }
    return *this;
  }
  ~Parser() {}

  inline parser_t<a> operator ()() const {
    return p;
  }

  std::ostream& operator()(std::ostream& o) const {
    return o << "Parser (" << p << ")";
  }
private:

  parser_t<a> p;

};

template<typename a>
std::ostream& operator<<(std::ostream& strm, const Parser<a>& p)
{
  return p(strm);
}

template<typename a>
parser_t<a> parse (Parser<a> P) {
  return P();
};

extern plist_t<char> item_f(std::string s);

#endif
