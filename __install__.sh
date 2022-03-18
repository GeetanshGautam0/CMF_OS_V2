export PASS="0007"
echo $PASS | sudo -S apt update
sudo apt install build-essential -y
sudo apt install bison -y
sudo apt install flex -y
sudo apt install libgmp3-dev -y
sudo apt install libmpc-dev -y
sudo apt install libmpfr-dev -y
sudo apt install texinfo -y

export PREFIX="/usr/local/x86_64elfgcc"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

mkdir /tmp/src
cd /tmp/src
echo $PASS | sudo -S rm -r *
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.35.1.tar.gz
echo "tar"
tar xf binutils-2.35.1.tar.gz
mkdir binutils-build
cd binutils-build

../binutils-2.35.1/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
echo $PASS | sudo -S make all install 2>&1 | tee make.log

cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz
echo "tar"
tar xf gcc-10.2.0.tar.gz
mkdir gcc-build
cd gcc-build

../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-langauges=c++ --without-headers
echo $PASS | sudo -S make all-gcc
echo $PASS | sudo -S make all-target-libgcc
echo $PASS | sudo -S make install-gcc
echo $PASS | sudo -S make install-target-libgcc

ls /usr/local/x86_64elfgcc/bin
