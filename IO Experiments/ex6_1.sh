
FN="myf2fsfs/f2fs.t"

IO1=4096
IO2=8192
IO3=16384
IO4=32768
IO5=65536

S1=2000
S2=1000
S3=500
S4=250
S5=125

echo "Running $FN randIO" 
echo "-- -- -- -- -- -- -- --"
echo 
echo "IO_Size = $IO1, samples = $S1" 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO1 $S1 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO1 $S1 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO1 $S1 

echo "IO_Size = $IO2, samples = $S2" 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO2 $S2 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO2 $S2 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO2 $S2 

echo "IO_Size = $IO3, samples = $S3" 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO3 $S3 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO3 $S3 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO3 $S3 

echo "IO_Size = $IO4, samples = $S4" 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO4 $S4 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO4 $S4 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO4 $S4 

echo "IO_Size = $IO5, samples = $S5" 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO5 $S5 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO5 $S5 
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
./myio 2 $FN $IO5 $S5 