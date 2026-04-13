make clean
make
echo "myio f1 creation:"
time ./myio 1 f1 1048576
echo "dd f1 copying (f4)"
time dd if=f1 of=f4
echo "Diff results:"
diff -u -s f1 f4
rm f1 f4

echo "myio f2 creation:"
time ./myio 1 f2 524288
echo "dd f2 copying (f5)"
time dd if=f2 of=f5
echo "Diff results:"
diff -u -s f2 f5
rm f2 f5

echo "myio f3 creation:"
time ./myio 1 f3 262144
echo "dd f3 copying (f6)"
time dd if=f3 of=f6
echo "Diff results:"
diff -u -s f3 f6
rm f3 f6