make clean
make
echo "EX4"
echo
./myio 1 f9 250000
sudo losetup --find --show --nooverlap --direct-io=on f9
mkfs.xfs -b size=4096 -q -L EXT4VOL f9 # options q make it quiet -b specifiie block size -L specifies volume label
mkdir myxfsfs
sudo mount -t xfs /dev/loop8 myxfsfs
sudo chown frn:frn myxfsfs # chown necessary because ext4fs is under root access only by default
sudo chmod u+w myxfsfs
cd myxfsfs
../myio 1 xfs.t 50000
ls -la