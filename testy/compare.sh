#!/bin/bash
FILES=./tests/*.in

for f in $FILES
do

out=${f::-2}
out="${out}out"
./palindrom <$f >pal.out
diff -bq $out pal.out
done






