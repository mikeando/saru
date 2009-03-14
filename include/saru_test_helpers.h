#ifndef SARU_TEST_HELPERS_H
#define SARU_TEST_HELPERS_H

#include <ostream>
#include <vector>
#include <iterator>

template<typename T>
class Makelist
{
  public:
    Makelist() : list_() {}
    std::vector<T> operator()() const { return list_; }
    Makelist<T> operator()( const T & t )
    {
      Makelist retval;
      retval.list_ = list_;
      retval.list_.push_back(t);
      return retval;
    }

  protected:
    std::vector<T> list_;
};


template<typename T>
Makelist<T> makelist(const T & t)
{
  return Makelist<T>()( t );
}

template<typename T>
std::ostream& operator<<(std::ostream & os, const std::vector<T> & v)
{
  os<<"[ ";
  std::copy( v.begin(), v.end(), std::ostream_iterator<T>(os,", ") );
  os<<" ]";
  return os; 
}

#endif // SARU_TEST_HELPERS_H

