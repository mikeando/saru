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
  TestEqualityFailed( const X & x, const Y & y, int lineNumber, const char * const filename ) : TestFailed(lineNumber, filename), x_(x), y_(y) 
  {
    std::stringstream ss;
    ss<<filename<<":"<<lineNumber<<": saru_assert_equal failed. Expected \""<<x<<"\" got \""<<y<<"\"";
    setMessage(ss.str());
  }

  const X & x_;
  const Y & y_;
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

void error( const std::string & mesg, int lineNumber, const char * filename )
{
  throw TestError(mesg, lineNumber, filename );
}

class TestLogger
{
public:
  TestLogger() : runCount(0), passedCount(0) {}
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
  std::cerr<<"<@saru end "+testname+" @>"<<std::endl;
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

#define saru_error(m) \
do { \
saru::error(m, __LINE__, __FILE__); \
} while(false)

#define SARU_TEST( test, logger ) saru::do_test(test,#test,logger) 


#endif
