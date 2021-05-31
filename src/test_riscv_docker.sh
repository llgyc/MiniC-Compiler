#!/bin/bash

TESTPATH=/root/test
QEMU=qemu-riscv32-static

for file in `ls $TESTPATH`
do
    suf=${file##*.}
    if [ "$suf" != "S" ]; then
        continue
    fi
    name=${file%%.*}
    IGNORE='^(92|93|94|95|96|97).*$'
    if [[ $name =~ $IGNORE ]]; then
        echo "[Skipped] Test $name"
        continue
    fi
    echo "[Info] Testing $name"
    riscv32-unknown-linux-gnu-gcc test/$name.S -o output -L/root -lsysy -static
    if [ -f "$TESTPATH/$name.in" ]; then
        $QEMU output < "$TESTPATH/$name.in" > "$name.out"
    else
        $QEMU output > "$name.out"
    fi
    ret=$?
    if [ -s "$name.out" ]; then
        echo >> "$name.out"
    fi
    echo $ret >> "$name.out"

done 
