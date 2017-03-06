wget http://www.antlr3.org/download/C/libantlr3c-3.4.tar.gz
tar -xvf libantlr3c-3.4.tar.gz
cd libantlr3c-3.4
./configure --prefix=$HOME/system --enable-64bit --bindir=$HOME/system/bin
make install
cd ..
