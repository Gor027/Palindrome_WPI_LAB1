#!/bin/bash
for i in {1..10000}
do
./gen >"test$i.in"
./palindrom <"test$i.in" >"test$i.out"
done
