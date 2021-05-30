#!/bin/bash

TESTPATH=/home/llgyc/Desktop/Compiling/open-test-cases/sysy/section1/functional_test
MINIVM=/home/llgyc/Desktop/Compiling/MiniVM/build/minivm 

for file in `ls $TESTPATH`
do
    suf=${file##*.}
    if [ "$suf" != "sy" ]; then
        continue
    fi
    name=${file%%.*}
    #IGNORE='^(92|93|94|95|96|97).*$'
    #if [[ $name =~ $IGNORE ]]; then
    #    echo "[Skipped] Test $name"
    #    continue
    #fi
    echo "[Info] Testing $name"
    ./compiler -S -e "$TESTPATH/$name.sy" -o "../test/$name.S"
    if [ -f "$TESTPATH/$name.in" ]; then
        $MINIVM "../test/$name.S" < "$TESTPATH/$name.in" > "../test/$name.out"
    else
        $MINIVM "../test/$name.S" > "../test/$name.out"
    fi
    ret=$?
    if [ -s "../test/$name.out" ]; then
        echo >> "../test/$name.out"
    fi
    echo $ret >> "../test/$name.out"
    if diff -wB "$TESTPATH/$name.out" "../test/$name.out" &> /dev/null ; then
        echo "[Success] $name OK"
    else
        echo "[Failed] $name Failed"
        break
    fi
done 
