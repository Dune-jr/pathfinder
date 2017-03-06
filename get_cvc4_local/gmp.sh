wget https://gmplib.org/download/gmp/gmp-6.1.0.tar.bz2
tar xvf gmp-6.1.0.tar.bz2
cd gmp-6.1.0
./configure --prefix=$HOME/system --enable-cxx
make install -j 8
cd ..
