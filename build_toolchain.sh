sudo apt-get install make bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo gnu-efi mtools qemu qemu-system-x86

cd ..

git clone https://git.code.sf.net/p/gnu-efi/code gnu-efi
cd gnu-efi
make

cd ..

export TARGET=x86_64-elf
export TARGETS=$TARGET,x86_64-pe

export PREFIX=/opt/cross/$TARGET
export PATH="$PREFIX/bin:$PATH"

mkdir build-binutils
cd build-binutils
../binutils-*/configure --target=$TARGET --enable-targets=$TARGETS --prefix="$PREFIX" --with-sysroot --disable-nls
make
sudo make install

cd ..

mkdir build-gcc
cd build-gcc
../gcc-*/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc


echo "prefix is $PREFIX"