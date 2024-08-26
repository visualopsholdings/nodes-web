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

So on your Linux (or mac using Homebrew etc), get all you need: (These steps have been tested
on Ubuntu 24.04 on a Pi5 and in a TG4 in AWS):

```
sudo apt-get update
sudo apt-get -y install g++ gcc make cmake ruby-rubygems npm
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

[Instructions](https://github.com/visualopsholdings/nodes-devops/blob/main/dev/BOOST.md)

#### ZMQ

If you can get zeromq 4.3.5 or later, then use that otherwise you will need to build it
all from source.

[Instructions](https://github.com/visualopsholdings/nodes-devops/blob/main/dev/ZMQ.md)

#### Restinio

Put restinio alongside this project because you need to keep it around.

```
sudo gem install Mxx_ru
git clone https://github.com/stiffstream/restinio.git
cd restinio
```

Now if you have ruby 3.2.0 or later, you need to fix a script.

```
ruby -v
```

If you have this later version, then do something like:

```
sudo nano /var/lib/gems/3.2.0/gems/Mxx_ru-1.6.14.10/lib/mxx_ru/externals.rb
```

And then Ctrl+W and type "exists?" and then everywhere you see one change to "exist?" (no
plural).

Now the rest will work.

```
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

### This project

```
git clone https://github.com/visualopsholdings/nodes-web.git
```

## Web client

The web client is written in angular so you need to install that.

To install angular.

```
sudo npm install -g @angular/cli@16
```

To build the project (including the angular front end):

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

## Testing dependencies

Testing relies on the external project:

https://github.com/visualopsholdings/nodes-test

If this changes, you can fetch it down and rebuild again with:

```
cd build
rm -rf _deps/nodes-test-*
make
```

## Companion projects

### DevOps

https://github.com/visualopsholdings/nodes-devops

### The main nodes system

https://github.com/visualopsholdings/nodes

### An IRC front end

https://github.com/visualopsholdings/nodes-irc

## Current development focus

### Making users active/inactve and admin/admin

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

### 14 Aug 2024
- Use and update the "site" customised info

### 16 Aug 2024
- Implement querying for a user from an upstream and getting the query result back.
- Added new code to run ZMQ in a thread for the SUB. This is where queryResults come 
back from.

### 19 Aug 2024
- "ADD UPSTREAM USER" now returns the list of users using WebSockets.

### 23 Aug 2024
- New added users are now correctly discovered.
- implemented teams and adding a user to a team.

### 26 Aug 2024
- Users can be added and removed from a team



