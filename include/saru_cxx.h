/**
 * Copyright (c) 2008, Michael Anderson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the saru library nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MICHAEL ANDERSON ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MICHAEL ANDERSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef SARU_CXX_H
#define SARU_CXX_H

#include <sstream>
#include <iostream>

void saru_break_point() {}

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

class TestSkipped
{
public:
  TestSkipped( int lineNumber, const char * const filename ) : lineNumber_(lineNumber), filename_(filename) {}
  TestSkipped( const std::string &message, int lineNumber, const char * const filename ) : lineNumber_(lineNumber), filename_(filename), message_(message) {}

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
  TestAssertFailed( const char * test, const std::string & mesg, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename)
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_assert failed. : "<<test<<" ["<<mesg<<"]";
    setMessage(ss.str());
  }
};

class TestError : public TestFailed
{
public:
  TestError( const std::string & mesg, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename)
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_error : "<<mesg;
    setMessage(ss.str());
  }
};

template<typename X, typename Y>
class TestEqualityFailed : public TestFailed
{
public:
  TestEqualityFailed( const X & x, const Y & y, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename)
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_assert_equal failed. Expected \""<<x<<"\" got \""<<y<<"\"";
    setMessage(ss.str());
  }

  TestEqualityFailed( const X & x, const Y & y, const std::string & mesg, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename)
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_assert_equal failed. Expected \""<<x<<"\" got \""<<y<<"\" ["<<mesg<<"]";
    setMessage(ss.str());
  }
};

template<typename X, typename Y>
class TestNonEqualityFailed : public TestFailed
{
public:
  TestNonEqualityFailed( const X & x, const Y & y, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename)
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_assert_not_equal failed. Values are \""<<x<<"\" and \""<<y<<"\"";
    setMessage(ss.str());
  }
};


template<typename X, typename Y>
void assert_equal_template( const X & x, const Y & y, int lineNumber, const char * filename )
{
  if(x==y) return;
  saru_break_point();
  throw TestEqualityFailed<X,Y>(x,y,lineNumber,filename);
}

template<typename X, typename Y>
void assert_equal_mesg_template( const X & x, const Y & y, const std::string & mesg, int lineNumber, const char * filename )
{
  if(x==y) return;
  saru_break_point();
  throw TestEqualityFailed<X,Y>(x,y,mesg,lineNumber,filename);
}

template<typename X, typename Y>
void assert_not_equal_template( const X & x, const Y & y, int lineNumber, const char * filename )
{
  if(x!=y) return;
  saru_break_point();
  throw TestNonEqualityFailed<X,Y>(x,y,lineNumber,filename);
}

void assert_template( bool v, const char * testmesg, int lineNumber, const char * filename )
{
  if( v ) return;
  saru_break_point();
  throw TestAssertFailed(testmesg,lineNumber,filename);
}

void assert_mesg_template( bool v, const char * testmesg, const std::string & mesg, int lineNumber, const char * filename )
{
  if( v ) return;
  saru_break_point();
  throw TestAssertFailed(testmesg,mesg,lineNumber,filename);
}

void error( const std::string & mesg, int lineNumber, const char * filename )
{
  saru_break_point();
  throw TestError(mesg, lineNumber, filename );
}

void skip( const std::string & mesg, int lineNumber, const char * filename )
{
  saru_break_point();
  throw TestSkipped(mesg, lineNumber, filename );
}

class TestLogger
{
public:
  TestLogger() : runCount(0), passedCount(0), failedCount(0), skippedCount(0) {}
  virtual ~TestLogger() {}

  virtual void registerTestPassed( const std::string & testname )
  {
    ++runCount;
    ++passedCount;
    std::cout<<testname<< " : OK\n";
  }

  virtual void registerTestFailed( const std::string & testname, TestFailed & exception )
  {
    ++runCount;
    ++failedCount;
    std::cout<<testname << " : FAILED\n";
    std::cerr<<"\n    In function " << testname <<std::endl;
    std::cerr<<exception.mesg()<<std::endl;
  }

  virtual void registerTestSkipped( const std::string & testname, TestSkipped & exception )
  {
    ++runCount;
    ++skippedCount;
    std::cout<<testname << " : SKIPPED\n";
    std::cerr<<"\n    In function " << testname <<std::endl;
    std::cerr<<exception.mesg()<<std::endl;
  }

  void printSummary() const
  {
    std::cout<<passedCount<<" / "<<(runCount-skippedCount)<<" [ "<<skippedCount<<" skipped ]"<<std::endl;
  }

  bool allOK() const { return failedCount==0; }

  unsigned int runCount;
  unsigned int passedCount;
  unsigned int failedCount;
  unsigned int skippedCount;
};

void do_test( void(*fn)(), const std::string & testname, TestLogger & logger )
{
  std::cerr<<"<@saru start "+testname+" @>"<<std::endl;
  try
  {
    (*fn)();
    logger.registerTestPassed( testname );
  }
  catch( TestFailed & err )
  {
     logger.registerTestFailed( testname, err );
  }
  catch( TestSkipped & err )
  {
     logger.registerTestSkipped( testname, err );
  }
  std::cerr<<"<@saru end "+testname+" @>"<<std::endl;
}

template< typename T>
void do_test( void(T::*fn)(), const std::string & testname, TestLogger & logger )
{
  std::cerr<<"<@saru start "+testname+" @>"<<std::endl;
  try
  {
    T t;
    (t.*fn)();
    logger.registerTestPassed( testname );
  }
  catch( TestFailed & err )
  {
     logger.registerTestFailed( testname, err );
  }
  catch( TestSkipped & err )
  {
     logger.registerTestSkipped( testname, err );
  }
  std::cerr<<"<@saru end "+testname+" @>"<<std::endl;
}

}

#define saru_assert(x) \
do { \
saru::assert_template(x, #x, __LINE__,__FILE__); \
} while(false)

#define saru_assert_m(x,mesg) \
do { \
saru::assert_mesg_template(x, #x, mesg, __LINE__,__FILE__); \
} while(false)

#define saru_assert_equal(x,y) \
do { \
saru::assert_equal_template(x,y,__LINE__,__FILE__); \
} while(false)

#define saru_assert_equal_m(x,y,mesg) \
do { \
saru::assert_equal_mesg_template(x,y,mesg,__LINE__,__FILE__); \
} while(false)

#define saru_assert_not_equal(x,y) \
do { \
saru::assert_not_equal_template(x,y,__LINE__,__FILE__); \
} while(false)

#define saru_error(m) \
do { \
saru::error(m, __LINE__, __FILE__); \
} while(false)

#define saru_skip(m) \
do { \
saru::skip(m, __LINE__, __FILE__); \
} while(false)

#define SARU_TEST( test, logger ) saru::do_test(&test,#test,logger)

#endif
