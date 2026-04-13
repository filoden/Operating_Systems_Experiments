make clean
make
echo "EX2"
echo
./myio 1 f7 131072
loopfat32=$(sudo losetup --find --show --nooverlap --direct-io=on f7)
mkfs.fat -a -S 512 -s 8 -F 32 -n FAT32VOL f7
mkdir mountPoint
sudo mount -t msdos -o rw,uid=frn,gid=frn $loopfat32 mountPoint
cd mountPoint
../myio 1 f8 200
rm f8
cd ..
rm f7
