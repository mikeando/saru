#ifndef SARU_CXX_H
#define SARU_CXX_H

#include <sstream>
#include <iostream>

namespace saru
{


class TestFailed 
{
public:
  TestFailed( int lineNumber, const char * const filename ) : lineNumber_(lineNumber), filename_(filename) {}

  void setMessage( const std::string & message ) { message_ = message; }
  std::string mesg() const { return message_; }

  int lineNumber_;
  const char * const filename_;
  std::string message_;
};

class TestAssertFailed : public TestFailed
{
public:
  TestAssertFailed( const char * test, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename)
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_assert failed. : "<<test;
    setMessage(ss.str());
  }
};

template<typename X, typename Y>
class TestEqualityFailed : public TestFailed
{
public:
  TestEqualityFailed( const X & x, const Y & y, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename), x_(x), y_(y) 
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_assert_equal failed. Expected \""<<x<<"\" got \""<<y<<"\"";
    setMessage(ss.str());
  }

  X x_;
  Y y_;
};


template<typename X, typename Y>
void assert_equal_template( const X & x, const Y & y, int lineNumber, const char * filename )
{
  if(x==y) return;
  throw TestEqualityFailed<X,Y>(x,y,lineNumber,filename);
}

void assert_template( bool v, const char * testmesg, int lineNumber, const char * filename )
{
  if( v ) return;
  throw TestAssertFailed(testmesg,lineNumber,filename);
}

class TestLogger
{
public:
  TestLogger() : runCount(0), passedCount(0) {}

  virtual void registerTestPassed( const std::string & testname )
  {
    ++runCount;
    ++passedCount;
    std::cout<<testname<< " : OK\n";
  }
  virtual void registerTestFailed( const std::string & testname, TestFailed & exception )
  {
    ++runCount;
    std::cout<<testname << " : FAILED\n";
    std::cerr<<"\n    In function " << testname <<std::endl;
    std::cerr<<exception.mesg()<<std::endl;
  }

  void printSummary() const
  {
    std::cout<<passedCount<<" / "<<runCount<<std::endl;
  }

  bool allOK() const { return runCount==passedCount; }

  unsigned int runCount;
  unsigned int passedCount;
};

void do_test( void(*fn)(), const std::string & testname, TestLogger & logger )
{
  try
  {
    (*fn)();
    logger.registerTestPassed( testname );
  }
  catch( TestFailed & err )
  {
     logger.registerTestFailed( testname, err );
  }
}

}

#define saru_assert(x) \
do { \
saru::assert_template(x, #x, __LINE__,__FILE__); \
} while(false)

#define saru_assert_equal(x,y) \
do { \
saru::assert_equal_template(x,y,__LINE__,__FILE__); \
} while(false)

#define SARU_TEST( test, logger ) saru::do_test(test,#test,logger) 

#endif
