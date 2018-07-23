#ifndef _Regular_Tile_HPP_
#define _Regular_Tile_HPP_

#include "Tile.hpp"

class RegularTile : public Tile {

private:
	bool flowed;
	bool isPocket;

public:
    RegularTile();
    ~RegularTile() {};
	RegularTile(Opening opening1, Opening opening2);
	RegularTile& operator=(const RegularTile& t);
    void flow();
    void setPocket(bool v);
    bool isJunk();
    bool canSwap();
    bool hasFlow();
};
#endif
