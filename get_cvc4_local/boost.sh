wget http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz
tar -xvf boost_1_54_0.tar.gz
./bootstrap.sh --prefix=$HOME/system
./b2 install
cd ..

