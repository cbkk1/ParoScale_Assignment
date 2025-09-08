# Niova Systems' Coding Challenge

At Niova Systems, we have found little to no correlation between doing well in
brain-teasers and "whiteboard coding" and being a world-class software engineer.
On the other hand, we believe that having a candidate working on a scenario as
close to the real-world as possible is the best way to see if somebody is a good
fit for our team.

That's why we are sending you this coding challenge. Our goal is to have you
write code and supporting artifacts that reflect the way you think and act about
code in your professional life; Not put you under the gun writing code on a
whiteboard with somebody second-guessing each step while a wall clock is staring
you in the face telling you there are only a few minutes left.

## The problem we'd like you to solve

For this fictional problem, we ask you to write a package indexer.

*Packages* are executables or libraries that can be installed in a system, often
via a package manager such as apt, RPM, or Homebrew. Many packages use libraries
that are also made available as packages themselves, so usually a package will
require you to install its dependencies before you can install it on your
system.

The system you are going to write keeps track of package dependencies. Clients
will connect to your server and inform which packages should be indexed, and
which dependencies they might have on other packages. We want to keep our index
consistent, so your server must not index any package until all of its
dependencies have been indexed first. The server should also not remove a
package if any other packages depend on it.

The server will open a TCP socket on port 8080. It must accept connections from
multiple clients at the same time, all trying to add and remove items to the
index concurrently. Clients are independent of each other, and it is expected
that they will send repeated or contradicting messages. New clients can connect
and disconnect at any moment, and sometimes clients can behave badly and try to
send broken messages.

Messages from clients follow this pattern:

```
<command>|<package>|<dependencies>\n
```

Where:
* `<command>` is mandatory, and is either `INDEX`, `REMOVE`, or `QUERY`

* `<package>` is mandatory, the name of the package referred to by the command,
   e.g. `mysql`, `openssl`, `pkg-config`, `postgresql`, etc.

* `<dependencies>` is optional, and if present it will be a comma-delimited list
   of packages that need to be present before `<package>` is installed.
   e.g. `cmake,sphinx-doc,xz`

* The message always ends with the character `\n`

Here are some sample messages:
```
INDEX|cloog|gmp,isl,pkg-config\n
INDEX|ceylon|\n
REMOVE|cloog|\n
QUERY|cloog|\n
```

For each message sent, the client will wait for a response code from the server.
Possible response codes are `OK\n`, `FAIL\n`, or `ERROR\n`. After receiving the
response code, the client can send more messages.

The response code returned should be as follows:

* For `INDEX` commands, the server returns `OK\n` if the package can be indexed.
It returns `FAIL\n` if the package cannot be indexed because some of its
dependencies aren't indexed yet and need to be installed first. If a package
already exists, then its list of dependencies is updated to the one provided
with the latest command.

* For `REMOVE` commands, the server returns `OK\n` if the package could be
removed from the index. It returns `FAIL\n` if the package could not be removed
from the index because some other indexed package depends on it. It returns
`OK\n` if the package wasn't indexed.

* For `QUERY` commands, the server returns `OK\n` if the package is indexed.
It returns `FAIL\n` if the package isn't indexed.

* If the server doesn't recognize the command or if there's any problem with the
message sent by the client it should return `ERROR\n`.

### Technology choices and constraints
Code at Niova Systems is typically written in Go, C, and some python.
* C is used in our Raft engine, "Pumicedb", and our virtual block device driver.
* Go is used in our control plane and S3 interface layer.
* Python is used in our test harness, "Holon".

The position you are applying for should determine your choice of
language though this is not a firm requirement.  If you intend to work on driver
level software, then you are free to use another language which is suitable for
systems programming (C++, Zig, Rust, etc.) but it's discouraged to use a
language which has built in garbage collection.  Otherwise, if Go is not your
strong suit then feel free to select the language of your choice.

For this exercise we would like to see as much of your own code as possible so
we ask you **not to use any library apart from your chosen runtime's standard
library**. Testing code and build tools might use libraries, but not production
code.

We would also ask you to write code that you would consider production-ready,
something you think other people would be happy supporting. Please don't forget
to send us your automated tests and any other artifact needed to develop, build,
or run your solution.

At Niova Systems we encourage the use LLMs to supplement developer tasks when
possible.  For this task, you are permitted to use an LLM as long as a permalink
to the chat session is provided with the submitted result.

## The niova-candidate Repository

Together with this `README.md` file, the repository should contain:

* test-driver-bin/

Contains pre-compiled binaries for a variety of platforms.

* test-driver-src/

The Go source code for the test driver and a Makefile for compilation.

### The test driver

This executable runs an automated test suite. We would like you to use this to
verify your solution before sending it to us, and it can also be useful as
functional tests during your development.

To run the test driver, first make sure your server is up and listening on port
`8080`. Then execute the following command:

```
$ ./test-driver-bin/niova-candidate-test-driver_(PLATFORM)
```

The tool will first test for correctness, then try a robustness test. Both
should pass before you submit your solution to the challenge, and once they both
pass you will see a message like this:

```
================
All tests passed!
================
```

We have built several other features in the test suite you might find helpful.
To see them all, execute the following command:

```
$ ./test-driver-bin/niova-candidate-test-driver_(PLATFORM) --help
```

## Requirements

### Must Haves
These are the requirements your submission must fulfill to be considered
correct.

* Upload of all source code for test and production, including any any artifacts
(build scripts, etc.) to the provided git repository.

* Your code builds from a Makefile on a recent Ubuntu image and produces the
binary **./test-driver-bin/niova-candidate-test-server**.

* An accurate and representative git commit history.

* Permalink to the LLM session(s) if an LLM was used.  This may be added to the
repository in a file called "llm.txt".

* Your code is something you'd be comfortable putting in production and having
your team maintaining

* Your code must pass the supplied test harness using different random seeds and
concurrency factor up to 100

### Nice to Have
Stretch goals. If you fulfill these requirements, you get bonus points, but they
aren't required.

* Design rationale
* Usage of io_uring for the handling of tcp socket data transfer
