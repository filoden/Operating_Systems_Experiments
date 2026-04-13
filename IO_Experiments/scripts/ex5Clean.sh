rm myf2fsfs/f2fs.t
#cd ..
sudo umount /dev/loop8
sudo losetup -d /dev/loop8
rmdir myf2fsfs
rm f9