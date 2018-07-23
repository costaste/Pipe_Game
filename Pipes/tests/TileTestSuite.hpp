#ifndef TEST_TILETESTSUITE_HPP
#define TEST_TILETESTSUITE_HPP

#include <cxxtest/TestSuite.h>
#include <iostream>
#include "../headers/Tile.hpp"
#include "../headers/RegularTile.hpp"
#include "../headers/JunkTile.hpp"

using namespace std;
class TileTestSuite : public CxxTest::TestSuite
{

public:
	void setUp() {

	}

	void tearDown() {

	}
	void testJunkTile() {
		Tile * t = new JunkTile();
		Tile * r = new RegularTile();

		TS_ASSERT(t->isJunk());
		TS_ASSERT(!r->isJunk());
		
		delete t;
		delete r;
	}

	void testFlip() {
		Tile * t = new JunkTile();

		TS_ASSERT(!t->isFlipped());
		t->flip();
		TS_ASSERT(t->isFlipped());
		delete t;
	}

	void testFlow() {
		Tile * t = new JunkTile();
		Tile * r = new RegularTile();

		t->flow();
		r->flow();
		TS_ASSERT(!t->hasFlow());
		TS_ASSERT(r->hasFlow());
		delete t;
		delete r;
	}

	void testCanSwap() {
		Tile * t = new JunkTile();
		Tile * r = new RegularTile();

		TS_ASSERT(!t->canSwap());
		t->flip();
		TS_ASSERT(!t->canSwap());

		TS_ASSERT(!r->canSwap());
		r->flip();
		TS_ASSERT(r->canSwap());

		r->flow();
		TS_ASSERT(!r->canSwap());

		delete r;
		delete t;
	}

	void testJunkConstructor() {
		Tile * t = new JunkTile();
		TS_ASSERT(!t->isFlipped());
		delete t;
	}

	void testRegularConstructor() {
		Tile * r = new RegularTile(NORTH, EAST);
		TS_ASSERT_EQUALS(r->getO1(), NORTH);
		TS_ASSERT_EQUALS(r->getO2(), EAST);
		delete r;	
	}

	void testRegularAssignment() {
		RegularTile t;
		RegularTile s = t;

		TS_ASSERT_EQUALS(t.isFlipped(), s.isFlipped());
		TS_ASSERT_EQUALS(t.hasFlow(), s.hasFlow());
		TS_ASSERT_EQUALS(t.getO1(), s.getO1());
		TS_ASSERT_EQUALS(t.getO2(), s.getO2());
	}


};
#endif

