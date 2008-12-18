// SARU : TestName Test02
// SARU : MultiTest text
// SARU : CxxInclude ../include
// SARU : Garbage
// SARU : Moo some ting
// SARU : Moo more ting
// SARU : IEmpty
// SARU : IEmpty notsomuch
// SARU : GRR grr
// SARU : GRR


#include "saru_cxx.h"

void testA()
{

}


void testB()
{
  saru_assert(!"intentional fail");
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
