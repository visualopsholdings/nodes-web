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

### Prerequisites

The best way to get started is to take a look at:

https://github.com/visualopsholdings/nodes-devops

And then find your particular OS and run those steps to get a basic binary-only build running 
for your platform. Once you have that you can come back in here and setup all the development
tools you might need.

Ok your back!!!!

No this project can be found as the folder nodes-web in the thing you just setup. The next step
is to remove that folder and pull the SOURCE from git with:

```
git clone https://github.com/visualopsholdings/nodes-web.git
```

Then depending on your platform, add in necessary development tools:

On Ubuntu 24.04:

```
sudo apt-get update
sudo apt-get -y install g++ gcc make cmake ruby-rubygems npm
```

On the maxc OS:

```
brew install cmake npm
```

For convenience, do all these inside a folder you can remove, they take a lot of disk space
but aren't used once installed.

```
mkdir working
cd working
```

#### Boost 1.85.0

if you can get this or later with a simple tool then do that, otherwise you will
need to build it from source:

[Instructions](https://github.com/visualopsholdings/nodes-devops/blob/main/dev/BOOST.md)

#### ZMQ

If you can get zeromq 4.1.5 or later, then use that otherwise you will need to build it
all from source.

[Instructions](https://github.com/visualopsholdings/nodes-devops/blob/main/dev/ZMQ.md)

We use 4.1.5 which is an earlier version because that's what we have tested with. 4.3.5
also seems to work perfectly on the mac but 4.3.5 doesn't build with CURVE by default on
some platforms which needs to be sorted out.

#### Restinio

Put restinio alongside this project because you need to keep it around.

```
sudo gem install Mxx_ru
git clone https://github.com/stiffstream/restinio.git
cd restinio
```

To clarify, your directory tree would look this this:

- My dev place
-- nodes
-- nodes-devops
-- nodes-web
-- restinio

Now if you have ruby 3.2.0 or later, you need to fix a script, otherwise
just skip these steps.

```
ruby -v
```

If this says anything later than 3.2.0, then do something like:

```
sudo nano /var/lib/gems/3.2.0/gems/Mxx_ru-1.6.14.10/lib/mxx_ru/externals.rb
```

And then Ctrl+W and type "exists?" and then everywhere you see one change to "exist?" (no
plural).

The point is that you need to change the ruby method "exists?" to "exist?" in that ruby script.

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

If you know the name of a single test, you can just run that with:

```
./test-single.sh "A user can be set to admin"
```

## Development and debugging

To develop and debug the various code bases, you can run them up after making changes. Run
them up in different terminal windows.

Firstly, you can populate the MongoDB with one of the tests, or for manual testing and general operation, 
run the "manual" test:

```
./test.sh features/manual.feature
```

If you are doing development on nodes as well (it's easier to work it all out this way), 
build and run nodes (in one terminal window):

```
cd nodes/build
make && ./nodes --test --logLevel=trace
```

But you could just run nodes with:

```
nodes/scripts/start.sh
```

Build and run this project in another terminal window:

```
cd nodes-web/build
make && ./nodes-web --logLevel=trace
```

And then for the angular chat front end (in another terminal window):

```
cd nodes-web/angular/chat-ui
./run.sh
```

Then visit here with your browser:

http://localhost:8081/login/?username=tracy

Then you can edit the TypeScript code for chat and it will deploy instantly with each change.

If you need to change the C++ code, just make your changes, quit and rerun the nodes-web
command and it will rebuild and on success re-run ready to go again.

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

### Instructions for hardening and building a server on the internet.

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
- A user can be have it's active and admin flags set.

### 28 Aug 2024
- Restinio uses BOOST logging.

### 9 Sep 2024
- Use local.visualops.com as cert.



