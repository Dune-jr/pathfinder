#build routine to install cvc4 in ./CVC4
sudo apt-get install g++ git autoconf automake libtool libgmp-dev libantlr3c-dev libboost-thread-dev
sudo apt-get install antlr3 #will get java! (you may want to try without antlr3 global install)
git clone https://github.com/CVC4/CVC4
cd CVC4
git reset --hard 973cbd67611a2943714fd9544d098ec1472a40b8
./autogen.sh
contrib/get-antlr-3.4
./configure --enable-proof --enable-portfolio --enable-gpl
make
sudo make install
