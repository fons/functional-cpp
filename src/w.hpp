#ifndef h__w__h
#define h__w__h

struct W {

  explicit W(int v, const std::string& s) : v(v), s(s){}
  W(const W& o) : v(o.v), s(o.s){}
  void operator=(const W& o) = delete;

  int ssn() const { return v;}
  std::string name() const {return s;}
  std::ostream& pp(std::ostream& os) const {
    return os << "name : " << s << " ssn :" << v << " ";
  }
private :
  int v;
  std::string s;
};

inline std::ostream& operator<<(std::ostream& os, const W& w)
{
  return w.pp(os);
}

inline W operator+(const W& lhs, const W& rhs)
{
  std::string s(lhs.name());
  s.append(rhs.name());
  return W(lhs.ssn() + rhs.ssn(), s);
}

inline bool operator==(const W& lhs, const W& rhs)
{
  return (lhs.ssn() == rhs.ssn()) and (lhs.name() == rhs.name());
}

inline bool operator!=(const W& lhs, const W& rhs)
{
  return ! (lhs == rhs);
}

#endif
