sudo apt-get install make bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo gnu-efi mtools qemu qemu-system-x86

cd ..

git clone https://git.code.sf.net/p/gnu-efi/code gnu-efi
cd gnu-efi
make

cd ..

export TARGET=i386-elf
export PREFIX="$HOME/opt/cross"
export PATH="$PREFIX/bin:$PATH"

rm -fr build-binutils
mkdir build-binutils
cd build-binutils
sudo ../binutils-*/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
sudo make install

cd ..

rm -fr build-gcc
mkdir build-gcc
cd build-gcc
sudo ../gcc-*/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc

echo "prefix is $PREFIX"