#!/bin/bash
echo "Just a temporary wrapper script while we get saru able to take over..."

#Old style named tests
echo "====================="
echo "  Old Style naming"
echo "====================="
for i in ??-??-* ; do
  echo -n $i 
  ./$i && echo " OK" || echo " FAILED" ; 
done

echo "====================="
echo "  New Style naming"
echo "====================="
for i in ??-*-??-* ; do
  echo -n $i
  ./$i && echo " OK" || echo " FAILED";
done


