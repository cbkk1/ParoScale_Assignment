# Builds the package indexer server to the path required by the challenge:
#   ./test-driver-bin/niova-candidate-test-server
#
# Run from the repository root:
#   make         build the server
#   make test    build and run the unit tests
#   make clean   remove the built server binary

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pthread

SRCDIR := server
OUTDIR := test-driver-bin
SERVER := $(OUTDIR)/niova-candidate-test-server

# Production sources (standard library + POSIX sockets only).
SOURCES := $(SRCDIR)/index.cpp $(SRCDIR)/protocol.cpp $(SRCDIR)/server.cpp

# Test sources reuse the index and protocol units with their own main().
TEST_SOURCES := $(SRCDIR)/index.cpp $(SRCDIR)/protocol.cpp $(SRCDIR)/test.cpp
TEST_BIN     := niova_tests

all: $(SERVER)

$(SERVER): $(SOURCES) $(SRCDIR)/index.hpp $(SRCDIR)/protocol.hpp
	@mkdir -p $(OUTDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

test: $(TEST_SOURCES) $(SRCDIR)/index.hpp $(SRCDIR)/protocol.hpp
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(TEST_SOURCES)
	./$(TEST_BIN)

clean:
	rm -f $(SERVER) $(TEST_BIN)

.PHONY: all test clean
