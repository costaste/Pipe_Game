CXXTEST_HOME = cxxtest-4.4
CXXTEST_GEN = $(CXXTEST_HOME)/bin/cxxtestgen
CXXTEST_INCLUDE = $(CXXTEST_HOME)

.PHONY: all clean

all: runner

runner.cpp: TileTestSuite.hpp BoardTestSuite.hpp
	$(CXXTEST_GEN) -o $@ --error-printer $^

runner: runner.cpp ../source/Tile.cpp ../source/RegularTile.cpp ../source/JunkTile.cpp ../source/Board.cpp
	g++ -std=c++11 $^ -I$(CXXTEST_INCLUDE) -o $@

clean:
	rm -f runner runner.cpp
