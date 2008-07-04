#!/bin/bash
echo "Just a temporary wrapper script while we get saru able to take over..."
for i in ??-??-* ; do
  echo -n $i 
  ./$i && echo " OK" || echo " FAILED" ; 
done
