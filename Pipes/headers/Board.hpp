#ifndef SRC_BOARD_H_
#define SRC_BOARD_H_

#include "Tile.hpp"

#include "RegularTile.hpp"
#include "JunkTile.hpp"
#include <vector>
#include <memory>
const int BOARD_SIZE = 10;

class Board {
public:
    Board();
    Board(bool junk);
    Board(std::vector<Tile*> solution);
    ~Board();
    void increaseFlow();
    void autoComplete();
    bool swap(Tile & t);
    bool won();
    Tile * tiles[BOARD_SIZE][BOARD_SIZE];
    std::shared_ptr<RegularTile> pocket;
    bool loss();

private:
    pair<int, int> flow_location;
    pair<int, int> finish;
    bool done;
};

#endif
