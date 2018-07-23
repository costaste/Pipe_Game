
#ifndef _Tile_HPP_
#define _Tile_HPP_

#include <utility>

using namespace std;

enum Opening {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class Tile {
public:
	void flip();
	bool isFlipped();
	virtual bool isJunk() = 0;
    virtual bool canSwap() = 0;
    virtual void flow() = 0;
    virtual bool hasFlow() = 0;
    virtual ~Tile();
    Opening getO1();
	Opening getO2();
	void setO1(Opening o);
	void setO2(Opening o);

protected:
    bool flipped;
    Opening o1;
	Opening o2;
};

#endif
