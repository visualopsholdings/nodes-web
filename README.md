# nodes-web

Web based front end for Nodes.

## Visual Ops

This is a companion project to:

https://github.com/visualopsholdings/nodes

Visual Ops is a proprietary (closed source) system that runs on very low end hardware
like T4G's in AWS, or on a Raspberry Pi 4/5.

An Open Source version of Visual Ops called "Nodes" is being developed.

This "plugin" can run on a a server with nodes (open source) to provide a login, admin and
chat system.

If you would like access to the Visual Ops software it is available as an image download 
after we get to know you.

Visit https://info.visualops.com/ for more info.

All the places that mention "Visual Ops", could be interchanged with "Nodes".

### ZMQ API

This project communicate with Nodes using the ZMQ message queues.

A PUB/SUB socket on port tcp://127.0.0.1:3012.

This is used to receive new messages from other nodes and users locally connected to this node.

A REP/REQ socket on port tcp://127.0.0.1:3013.

This is used to send messages to other nodes and users connected locally to this node. 
This port is also used to "ask" for things of the Nodes system such as login etc.

[Here are the formats for the messages](https://github.com/visualopsholdings/nodes/blob/main/src/msg/README.md)

## Building

The development process for all of this code used a normal Linux environment with the BOOST
libraries and a C++ compiler.

So on your Linux (or mac using Homebrew etc), get all you need:

```
sudo apt-get update
sudo apt-get -y install g++ gcc make cmake ruby-rubygems
```

### Prerequisites

We rely on https://github.com/visualopsholdings/nodes being built first, so do that, and then
in your shell, define a variable to point to that:

```
export NODES_HOME=/where/nodes/went
```

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

If you can get zeromq 4.3.5 or later, then use that otherwise you will need to build it
all from source.

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
git clone https://github.com/visualopsholdings/nodes-web.git
nodes-web/scripts/build.sh
```

## Web client

The web client is written in angular and must be built before using the nodes web daemon

To install angular.

```
npm install -g @angular/cli@16
```

To build http stuff.

```
nodes-web/scripts/build.sh
```

Then your ready to host in nginx and the proxy is provided by Nodes web.

To start the http daemon.

```
nodes-web/scripts/start.sh
```

To stop.

```
nodes-web/scripts/stop.sh
```

## Testing

The end to end tests use cucumber (Ruby) with capybara and selenium to run end to end tests
from the browser, to get that going:

```
./test.sh
```

## Development and debugging

To develop and debug the various code bases, you can run them up after making changes. Run
them up in a different terminal window.

Firstly, you can populate the MongoDB with one of the tests, or for manual testing and general operation, 
run the "manual" test:

```
bundle exec cucumber features/manual.feature
```

Build and run nodes:

```
cd nodes/build
make && ./nodes --test --logLevel=trace
```

Build and run this http backend:

```
cd nodes-web/build
make && ./nodes-web --logLevel=trace
```

And then for the angular chat front end.

```
cd nodes-web/angular/chat-ui
./run.sh
```

Then visit here with your browser:

http://localhost:8081/login/?username=tracy

Then you can edit the TypeScript code for chat and it will deploy instantly with each change.

## Current development focus

### Implement chat web app

## License

Nodes is licensed under [version 3 of the GNU General Public License] contained in LICENSE.

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

### 29 Jul 2024
- rename to https://github.com/visualopsholdings/nodes-web


