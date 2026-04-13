make clean
make
echo "EX2"
echo
./myio 1 f7 1000000
loopfat32=$(sudo losetup --find --show --nooverlap --direct-io=on f7)


loopext4=$(sudo losetup --find --show --nooverlap --direct-io=on f9)
out=$(mkfs.ext4 -q -b 4096 -L EXT4VOL f9)
mkdir myext4fs
sudo mount -t ext4 $loopext4 myext4fs
sudo chown $USER:$USER myext4fs
./myio 1 myext4fs/ext4.t 500000
sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
ext4=$(./myio 2 myext4fs/ext4.t 8192 10000)




mkfs.fat -a -S 512 -s 8 -F 32 -n FAT32VOL f7
mkdir mountPoint
sudo mount -t msdos -o rw,uid=frn,gid=frn $loopfat32 mountPoint
cd mountPoint
../myio 1 f8 200
rm f8
cd ..
rm f7