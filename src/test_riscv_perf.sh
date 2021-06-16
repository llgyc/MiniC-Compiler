#!/bin/bash

TESTPATH=/home/llgyc/Desktop/Compiling/open-test-cases/sysy/section1/performance_test
TESTPATH22=/home/llgyc/Desktop/Compiling/open-test-cases/sysy/section2/performance_test
MINIVM=/home/llgyc/Desktop/Compiling/MiniVM/build/minivm 

for file in `ls $TESTPATH $TESTPATH2`
do
    suf=${file##*.}
    if [ "$suf" != "sy" ]; then
        continue
    fi
    name=${file%%.*}
    IGNORE='^(92|93|94|95|96|97).*$'
    if [[ $name =~ $IGNORE ]]; then
        echo "[Skipped] Test $name"
        continue
    fi
    echo "[Info] Testing $name"
    ./compiler -S "$TESTPATH/$name.sy" -o "../test/$name.S"
    if [ $? != 0 ]; then
        echo "[Failed] Test $name"
        break
    fi
done 
