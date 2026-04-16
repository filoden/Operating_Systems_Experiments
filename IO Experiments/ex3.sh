make clean
make
echo "EX3"
echo
./myio 1 f9 250000
sudo losetup --find --show --nooverlap --direct-io=on f9
mkfs.ext4 -q -b 4096 -L EXT4VOL f9 # options q make it quiet -b specifiie block size -L specifies volume label
mkdir myext4fs
sudo mount -t ext4 /dev/loop8 myext4fs
sudo chown frn:frn myext4fs # chown necessary because ext4fs is under root access only by default
cd myext4fs
../myio 1 ext4.t 50000
ls -la


