# zchttp

Web based front end for zmqchat

## Visual Ops

This is a companion project to:

https://github.com/visualopsholdings/zmqchat

Visual Ops is a proprietary (closed source) system that runs on very low end hardware
like T4G's in AWS, or on a Raspberry Pi 4/5.

An Open Source version of Visual Ops called "ZMQChat" is being developed.

This "plugin" can run on a a server with zmqchat (open source) to provide a login, admin and
chat system.

If you would like access to the Visual Ops software it is available as an image download 
after we get to know you.

Visit https://info.visualops.com/ for more info.

All the places that mention "Visual Ops", could be interchanged with "ZMQChat".

## Building

The development process for all of this code used a normal Linux environment with the BOOST
libraries and a C++ compiler.

So on your Linux (or mac using Homebrew etc), get all you need:

```
sudo apt-get update
sudo apt-get -y install g++ gcc make cmake ruby-rubygems
```

### Prerequisites

For convenience, do all these inside a folder you can remove, they take a lot of disk space
but aren't used once instsalled.

```
mkdir working
cd working
```

#### Boost 1.85.0

if you can get this or later with a simple tool then do that, otherwise you will
need to build it from source:

```
wget https://github.com/boostorg/boost/releases/download/boost-1.85.0/boost-1.85.0-b2-nodocs.tar.gz
tar xzf boost-1.85.0-b2-nodocs.tar.gz 
cd boost-1.85.0
./bootstrap.sh --prefix=/usr --with-python=python3
./b2 stage threading=multi link=shared boost.stacktrace.from_exception=off

as root user:
./b2 install threading=multi link=shared
```

#### ZMQ

```
wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.19-stable.tar.gz  
tar xzf libsodium-1.0.19-stable.tar.gz
cd libsodium-stable
./configure
make 
sudo make install
sudo ldconfig
cd ..

wget https://github.com/zeromq/libzmq/releases/download/v4.3.5/zeromq-4.3.5.tar.gz
tar xzf zeromq-4.3.5.tar.gz
cd zeromq-4.3.5
./configure
make
sudo make install
sudo ldconfig
cd ..

git clone https://github.com/zeromq/libzmq.git
cd libzmq
mkdir build
cd build
cmake ..
make -j4
make test
sudo make install
cd ../..

git clone https://github.com/zeromq/cppzmq
cd cppzmq
mkdir build
cd build
cmake ..
make -j4
sudo make install
cd ../..
cd ..
```

#### Restinio

```
cd working
sudo gem install Mxx_ru
git clone https://github.com/stiffstream/restinio.git
cd restinio
mxxruexternals
cd dev
cmake -Bcmake_build \
   -DCMAKE_BUILD_TYPE=Debug \
   -DRESTINIO_ASIO_SOURCE=boost
cmake --build cmake_build --config Debug -j 4
```

You will need this in your .bashrc or equivalent

```
export RESTINIO_HOME=/where/restinio/went
```

Now this project:

```
git clone https://github.com/visualopsholdings/zchttp.git
zchttp/scripts/build.sh
```

## Web client

The web client is written in angular and must be built before using the zchttp daemon

To install angular.

```
npm install -g @angular/cli@16
```

To build http stuff.

```
zchttp/scripts/http.sh
```

Then your ready to host in nginx and the proxy is provided by zchttp.

To start the http daemon.

```
zchttp/scripts/http.sh
```

To stop.

```
zchttp/scripts/http.sh
```

## Testing

The end to end tests use cucumber (Ruby), to get that going:

```
$ bundle install
$ bundle exec cucumber
```

## Current development focus

### Implement chat web app

## License

ZMQChat is licensed under [version 3 of the GNU General Public License] contained in LICENSE.

## Acknowlegements

### ZeroMQ messaging

- https://zeromq.org/

### Boost C++ library

- https://www.boost.org/

### End to end testing

- https://cucumber.io/

#### Restinio

- https://stiffstream.com/en/docs/restinio/0.7/

## Change Log

### 26 Jul 2024
- Move web app to https://github.com/visualopsholdings/zchttp



