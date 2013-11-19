aufs configuration & Compilation
----------------------------------

1) aufs3-standalone tree
	git clone git://git.code.sf.net/p/aufs/aufs3-standalone \aufs3-standalone.git
	cd aufs3-standalone.git
	git checkout origin/aufs3.0

2) aufs-util tree
	git clone git://git.code.sf.net/p/aufs/aufs-util \aufs-util.git
	cd aufs-util.git
	git checkout origin/aufs3.0

3) For aufs3-standalone tree,build process:
	- apply ./aufs3-kbuild.patch to your kernel source files.
	- apply ./aufs3-base.patch.
	- apply ./aufs3-proc_map.patch too, if you want to make /proc/PID/maps (and others including lsof(1)) show 	   	the file path on aufs instead of the path on the branch fs.
	- copy ./{Documentation,fs,include/uapi/linux/aufs_type.h} files to your
  	kernel source tree. Never copy $PWD/include/uapi/linux/Kbuild.
	- enable CONFIG_AUFS_FS
	- build the kernel as usual.

Note: Since linux-3.9, every filesystem module requires an alias
  "fs-". You should make sure that "fs-aufs" is listed in your
  modules.aliases file if you set CONFIG_AUFS_FS=m.
	- install the header files too by "make headers_install"

	- read README in aufs-util, build and install it.

4) Disable xfs in filesystems. Enable user namespace option in Namespaces(kernel)
5) compile and reboot the kernel.

6) Steps to execute test case in graphene:

cd src
make
cd wrapper
make
cd ../../test
make
make test
./test ./libpal.so
../src/wrapper/wrapper ./libpal.so ./HelloWorld
rm -rf /tmp/pal_range /tmp/graphene /mnt/mntpt

   


 