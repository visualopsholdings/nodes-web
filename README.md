# nodes-web

A Web component that does RESTful requests, sockets, etags and all web things communicating
with the Nodes subsystem.

The best way to get started is to take a look at:

https://github.com/visualopsholdings/nodes-devops

## ZMQ API

This project communicate with Nodes using the ZMQ message queues.

### Nodes

https://github.com/visualopsholdings/nodes

### Request/Reply

A REP/REQ socket on port tcp://127.0.0.1:3013.

This is used to send messages to other nodes and users connected locally to this node. 
This port is also used to "ask" for things of the Nodes system such as login etc.

### Publish/Subscribe

A PUB/SUB socket on port tcp://127.0.0.1:3012.

This is used to receive new messages from other nodes and users locally connected to this node.

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

Some parts of the web client is written in angular so you need to install that.

To install angular.

```
sudo npm install -g @angular/cli@16
```

To build the project (including the angular and react front end):

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

You will need a machine with a display for this, so don't try to run it headless (for now). It
also probably won't run in Windows with the WSL since you will need to run all of the tests
from the Windows side and install Ruby etc there.

```
bundle install
```

You will only need to do that once to install the ruby stuff.

```
./test.sh
```

If you know the name of a single test, you can just run that with:

```
./test-single.sh "A user can be set to admin"
```

## More complicated tests

You can run tests that build multiple servers and run tests between then with.

```
./test.sh features/add-from-upstream.feature
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
make -j4 && ./nodes --test --logLevel=trace
```

The "test" allows the login to take a username "GET" argument (you'll see that below). This is just part
of the fullname or the username of the user to allow testing to work.

Build and run this project in another terminal window:

```
cd nodes-web/build
make -j4 && ./nodes-web --logLevel=trace
```

And then for the angular chat front end (in another terminal window):

```
cd nodes-web/angular/chat-ui
./run.sh
```

Then visit here with your browser:

```
http://localhost:8081/login/?username=tracy
```

Then you can edit the TypeScript code for chat and it will deploy instantly with each change.

If you need to change the C++ code, just make your changes, quit and rerun the nodes-web
command and it will rebuild and on success re-run ready to go again.

These commands will work for all of the angular projects and also the react ones, since there
is a "run.sh" script inside each folder.

## Running production built projects locally

When you want to test a fully build project locally, nodes-web can operate as a simple web server
with a "test" argument. Make sure you specify a reasonable port:

```
build/nodes-web --logLevel=trace --test --httpPort=8081
```

This URL will work just fine:

```
http://localhost:8081/login/?username=tracy
```

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

### 10 Sep 2024
- Add support for adding, deleting and editing team and streams.
- Speed up cucumber an order of magnitude by directly serving the angular built 
stuff rather than using ng serve.

### 11 Sep 2024
- Allow security policies to be edited for streams.

### 12 Sep 2024
- Allow streams to be shared with other users.

### 16 Sep 2024
- Start creating new invited users.

### 17 Sep 2024
- Invitations working except for testing expiry.

### 21 Sep 2024
- Add people panel to chat.

### 22 Sep 2024
- Get websockets working in chat.

### 23 Sep 2024
- Add delete user.

### 24 Sep 2024
- Nodes uses "corr" (correlation id) instead of "socketid".

### 25 Sep 2024
- Implement framework for Etags.
- We have a "none", "simpleTime", and "modifyDate" etags for now.

### 27 Sep 2024
- Add "collectionChanged" etag.

### 6 Oct 2024
- Now that Nodes handles upstream, switch to testing with it instead of an existing
Visual Ops node.

### 7 Oct 2024
- Nodes can be displayed, added and deleted.

### 9 Oct 2024
- Simplest possible react application. Just shows who is logged in.

### 20 Oct 2024
- Upstream for streams and groups.





