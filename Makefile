CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread
INCLUDES := -Ithird_party -Iinternal

SRC := $(wildcard internal/*.cpp) $(wildcard internal/**/*.cpp)
MAIN_SRC := cmd/main.cpp
TEST_SRC := internal/service_test.cpp

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