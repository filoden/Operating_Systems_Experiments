#!/bin/sh
# This is a comment

echo
echo This testing script should take up to 15 seconds total...

rm *.txt
make clean
make

./common files/THEGODFATHER.txt files/THEGODFATHER2.txt 1 student_out_1.txt
if diff -w student_out_1.txt files/correct_out_1.txt; then
    echo Test 1: Success-----------------------------------------------Success
else
    echo Test 1: Fail--------------------------------------------------Fail
fi

./common files/THEGODFATHER.txt files/THEGODFATHER2.txt 50 student_out_50.txt
if diff -w student_out_50.txt files/correct_out_50.txt; then
    echo Test 2: Success-----------------------------------------------Success
else
    echo Test 2: Fail--------------------------------------------------Fail
fi

./common files/THEGODFATHER.txt files/THEGODFATHER2.txt 100 student_out_100.txt
if diff -w student_out_100.txt files/correct_out_100.txt; then
    echo Test 3: Success-----------------------------------------------Success
else
    echo Test 3: Fail--------------------------------------------------Fail
fi

./common files/THEGODFATHER.txt files/THEGODFATHER2.txt 500 student_out_500.txt
if diff -w student_out_500.txt files/correct_out_500.txt; then
    echo Test 4: Success-----------------------------------------------Success
else
    echo Test 4: Fail--------------------------------------------------Fail
fi

./common files/THEGODFATHER.txt files/THEGODFATHER2.txt 1000 student_out_1000.txt
if diff -w student_out_1000.txt files/correct_out_1000.txt; then
    echo Test 5: Success-----------------------------------------------Success
else
    echo Test 5: Fail--------------------------------------------------Fail
fi
