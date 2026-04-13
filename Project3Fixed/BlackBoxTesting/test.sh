#!/bin/bash

#set -x
#trap read debug

make clean
make

printf "\nThis BlackBox testing script requires around 8 GB of free disk space and may take around 10 seconds (for SSDs) or longer (for HDDs) depending on your storage device performance!\n\n"

./myio 1 fat32disk.img 100000
dd if=/dev/zero of=ddfat32disk.img bs=4096 count=100000 status=none

if diff -w fat32disk.img ddfat32disk.img; then
    echo Test 1 - Success--------------------fat32disk.img------------------------------Success
    rm ddfat32disk.img
else
    echo Test 1 - Fail-----------------------fat32disk.img------------------------------Fail
    mv ddfat32disk.img fat32disk.img
fi

loopfat32=$(sudo losetup --find --show --nooverlap --direct-io=on fat32disk.img)
out=$(mkfs.fat -a -S 512 -s 8 -F 32 -n FAT32VOL fat32disk.img)
mkdir myfat32fs
sudo mount -t msdos -o rw,uid=$USER,gid=$USER $loopfat32 myfat32fs
./myio 1 myfat32fs/fat32.t 50000
# at this point we've created the fat32 file system on the mountpoint myfat32fs and file fat32.t within
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
# determine latency for 1000 samples on io grabs of 4096 bytes
fat32=$(./myio 2 myfat32fs/fat32.t 4096 1000)
echo "myio-fat32-4K:" $fat32
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
fio --filename=myfat32fs/fat32.t --direct=1 --rw=randread --bs=4k --ioengine=sync --iodepth=1 --numjobs=1 --number_ios=1000 --name=readlatency-test-fat32 --readonly --clat_percentiles=1 --percentile_list=50 > fiofat32out.txt
fiofat32=$(cat fiofat32out.txt | grep -o '50th=.*'  | cut -d "[" -f2 | cut -d "]" -f1)
fiofat32unit=$(cat fiofat32out.txt | grep 'clat percentiles' | awk '{print $3}')
if [ "$fiofat32unit" == "(usec):" ]; then
  fiofat32=$(($fiofat32*1000))
fi
printf "fio-fat32-4K: %.2f\n" $(($fiofat32))e-3
doublefiofat32=$(($fiofat32/500))
halffiofat32=$(($fiofat32/2000))
fat32int=${fat32%.*}
echo "double of fio (msec):" $doublefiofat32, "fio (nsec):" $fiofat32, "half of fio (msec):" $halffiofat32, "fat32 as int (msec):" $fat32int
if (($fat32int > $halffiofat32 && $fat32int < $doublefiofat32)); then
    echo Test 2 - Success--------------------fat32-latency------------------------------Success
else
    echo Test 2 - Fail-----------------------fat32-latency------------------------------Fail
fi

sudo umount myfat32fs 
sudo losetup -d $loopfat32
rm -rf myfat32fs
rm fat32disk.img
rm fiofat32out.txt

./myio 1 ext4disk.img 1000000
dd if=/dev/zero of=ddext4disk.img bs=4096 count=1000000 status=none

if diff -w ext4disk.img ddext4disk.img; then
    echo Test 3 - Success--------------------ext4disk.img------------------------------Success
    rm ddext4disk.img
else
    echo Test 3 - Fail-----------------------ext4disk.img------------------------------Fail
    mv ddext4disk.img ext4disk.img
fi

loopext4=$(sudo losetup --find --show --nooverlap --direct-io=on ext4disk.img)
out=$(mkfs.ext4 -q -b 4096 -L EXT4VOL ext4disk.img)
mkdir myext4fs
sudo mount -t ext4 $loopext4 myext4fs
sudo chown $USER:$USER myext4fs
./myio 1 myext4fs/ext4.t 500000
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
ext4=$(./myio 2 myext4fs/ext4.t 8192 10000)
echo "myio-ext4-8K:" $ext4
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
fio --filename=myext4fs/ext4.t --direct=1 --rw=randread --bs=8k --ioengine=sync --iodepth=1 --numjobs=1 --number_ios=10000 --name=readlatency-test-ext4 --readonly --clat_percentiles=1 --percentile_list=50 > fioext4out.txt 
fioext4=$(cat fioext4out.txt | grep -o '50th=.*'  | cut -d "[" -f2 | cut -d "]" -f1)
fioext4unit=$(cat fioext4out.txt | grep 'clat percentiles' | awk '{print $3}')
if [ "$fioext4unit" == "(usec):" ]; then
  fioext4=$(($fioext4*1000))
fi
printf "fio-ext4-8K: %.2f\n" $(($fioext4))e-3
doublefioext4=$(($fioext4/500))
halffioext4=$(($fioext4/2000))
ext4int=${ext4%.*}
#echo "double of fio (msec):" $doublefioext4, "fio (nsec):" $fioext4, "half of fio (msec):" $halffioext4, "ext4 as int (msec):" $ext4int
if (($ext4int > $halffioext4 && $ext4int < $doublefioext4)); then
    echo Test 4 - Success--------------------ext4-latency------------------------------Success
else
    echo Test 4 - Fail-----------------------ext4-latency------------------------------Fail
fi

sudo umount myext4fs 
sudo losetup -d $loopext4
rm -rf myext4fs
rm ext4disk.img
rm fioext4out.txt
