#ifndef _Junk_Tile_HPP_
#define _Junk_Tile_HPP_

#include "Tile.hpp"

class JunkTile : public Tile {


public:
    JunkTile();
    ~JunkTile() {};
    bool isJunk();
    bool canSwap();
    void flow();
    bool hasFlow();
};
#endif
