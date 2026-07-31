CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread
INCLUDES := -Ithird_party -Iinternal -I$(shell brew --prefix hiredis 2>/dev/null)/include
LDLIBS := -L$(shell brew --prefix hiredis 2>/dev/null)/lib -lhiredis

SRC := $(shell find internal -name '*.cpp')
MAIN_SRC := cmd/main.cpp
TEST_SRC := internal/booking/service_test.cpp

LIB_SRC := $(filter-out $(TEST_SRC),$(SRC))

.PHONY: run test build clean

build: bin/seatlock

run: bin/seatlock
	./bin/seatlock

bin/seatlock: $(MAIN_SRC) $(LIB_SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDLIBS)

test: bin/service_test
	./bin/service_test

bin/service_test: $(TEST_SRC) $(LIB_SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDLIBS)

clean:
	rm -rf bin