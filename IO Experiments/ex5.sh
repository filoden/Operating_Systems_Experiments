make clean
make
echo "EX5"
echo
./myio 1 f9 250000
sudo losetup --find --show --nooverlap --direct-io=on f9
mkfs.f2fs -q -l F2FS4VOL f9 # options q make it quiet -b specifiie block size -L specifies volume label
mkdir myf2fsfs
sudo mount -t f2fs /dev/loop8 myf2fsfs
sudo chown frn:frn myf2fsfs # chown necessary because ext4fs is under root access only by default
sudo chmod u+w myf2fsfs
cd myf2fsfs
../myio 1 f2fs.t 50000
ls -la