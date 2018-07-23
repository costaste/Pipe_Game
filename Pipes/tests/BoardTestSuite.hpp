#ifndef TEST_BOARDTESTSUITE_HPP
#define TEST_BOARDTESTSUITE_HPP

#include <cxxtest/TestSuite.h>
#include <iostream>
#include "../headers/Board.hpp"
using namespace std;
class BoardTestSuite : public CxxTest::TestSuite
{

public:
	void testIncreaseFlowFlipped() {
		Board * b = new Board();
		b->tiles[9][9] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][8] = new RegularTile(NORTH, SOUTH);

		b->increaseFlow();

		TS_ASSERT(b->loss());

		b->tiles[9][8]->flip();

		b->increaseFlow();

		TS_ASSERT(b->tiles[9][8]->hasFlow());

		delete b;
	}

	void testIncreaseFlowOffBoard() {
		Board * b = new Board();

		b->tiles[9][8] = new RegularTile(SOUTH, EAST);
		b->tiles[9][9] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][9]->flip();
		b->tiles[9][8]->flip();

		b->autoComplete();

		TS_ASSERT(b->tiles[9][8]->hasFlow());
		TS_ASSERT(b->loss());
		
		delete b;
	}

	void testIncreaseFlowTwice() {
		Board * b = new Board();

		b->tiles[9][8] = new RegularTile(WEST, SOUTH);
		b->tiles[8][8] = new RegularTile(NORTH, EAST);
		b->tiles[9][8]->flip();
		b->tiles[8][8]->flip();
		
		// NS to WS
		b->increaseFlow();

		TS_ASSERT(b->tiles[9][8]->hasFlow());

		// WS to NE
		b->increaseFlow();


		TS_ASSERT(b->tiles[8][8]->hasFlow());
		delete b;
	}

	void testSwap() {
		Board * b = new Board();

		TS_ASSERT(!b->swap(*b->tiles[7][7]));

		b->tiles[7][7]->flip();

		TS_ASSERT(b->swap(*b->tiles[7][7]));
		delete b;
	}

	void testJunkTiles() {
		Board * b = new Board(true);
		Board * a = new Board(false);
		int junkb = 0, junka = 0;
		for (int i = 0 ; i < BOARD_SIZE ; i++) {
			for (int j = 0 ; j < BOARD_SIZE ; j++) {
				if (b->tiles[i][j]->isJunk()) {
					junkb++;
				}
				if (a->tiles[i][j]->isJunk()) {
					junka++;
				}
			}
		}

		TS_ASSERT(junkb > 0 && junka == 0);
		delete b;
		delete a;
	}

	void testWon() {
		Board * b = new Board();

		b->tiles[9][8] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][8]->flip();


		b->tiles[9][7] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][7]->flip();


		b->tiles[9][6] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][6]->flip();


		b->tiles[9][5] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][5]->flip();

		
		b->tiles[9][4] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][4]->flip();

		
		b->tiles[9][3] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][3]->flip();

		
		b->tiles[9][2] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][2]->flip();

		
		b->tiles[9][1] = new RegularTile(NORTH, SOUTH);
		b->tiles[9][1]->flip();

		
		b->tiles[9][0] = new RegularTile(WEST, SOUTH);
		b->tiles[9][0]->flip();

		
		b->tiles[8][0] = new RegularTile(EAST, WEST);
		b->tiles[8][0]->flip();

		
		b->tiles[7][0] = new RegularTile(EAST, WEST);
		b->tiles[7][0]->flip();

		
		b->tiles[6][0] = new RegularTile(EAST, WEST);
		b->tiles[6][0]->flip();

		
		b->tiles[5][0] = new RegularTile(EAST, WEST);
		b->tiles[5][0]->flip();

		
		b->tiles[4][0] = new RegularTile(EAST, WEST);
		b->tiles[4][0]->flip();


		b->tiles[3][0] = new RegularTile(EAST, WEST);
		b->tiles[3][0]->flip();


		b->tiles[2][0] = new RegularTile(EAST, WEST);
		b->tiles[2][0]->flip();

		
		b->tiles[1][0] = new RegularTile(EAST, WEST);
		b->tiles[1][0]->flip();

		
		b->tiles[0][0] = new RegularTile(EAST, WEST);
		b->tiles[0][0]->flip();

		b->autoComplete();
		TS_ASSERT(b->won());
		
		delete b;
	}

};
#endif
