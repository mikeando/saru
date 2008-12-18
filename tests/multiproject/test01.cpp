// SARU : TestName Test01
// SARU : MultiTest text
// SARU : CxxInclude ../include


#include "saru_cxx.h"

void testA()
{

}


void testB()
{

}

void testC()
{

}

int main()
{
  saru::TestLogger logger;
  SARU_TEST( testA, logger );
  SARU_TEST( testB, logger );
  SARU_TEST( testC, logger );

  logger.printSummary();

  return logger.allOK()?0:1;
}
