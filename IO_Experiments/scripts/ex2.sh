make clean
make
echo "EX2"
echo
./myio 1 f7 131072
loopfat32=$(sudo losetup --find --show --nooverlap --direct-io=on f7)
mkfs.fat -a -S 512 -s 8 -F 32 -n FAT32VOL f7
mkdir myfat32fs
sudo mount -t msdos -o rw,uid=frn,gid=frn $loopfat32 myfat32fs
cd myfat32fs
../myio 1 f8 50000

