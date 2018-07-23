#include "../headers/Board.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

Board::Board() {
    srand((unsigned)time(0));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            // std::shared_ptr<Tile> t(new RegularTile());
            tiles[i][j] = new RegularTile();
        }
    }
    pocket.reset(new RegularTile());
    pocket->setPocket(true);
    pocket->flip();
    done = false;
    flow_location = std::make_pair(BOARD_SIZE - 1, BOARD_SIZE - 1);
    tiles[BOARD_SIZE - 1][BOARD_SIZE - 1] = new RegularTile(Opening(NORTH), Opening(SOUTH));
}

Board::Board(bool junk) {
    srand((unsigned)time(0));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Tile * t;
            if (rand() % 13 == 0 && junk) {
                t = new JunkTile();
                if (i > 0 && tiles[i-1][j]->isJunk()) {
                    t = new RegularTile();
                }
                if (i > 0 && j > 0 && tiles[i-1][j-1]->isJunk()) {
                    t = new RegularTile();
                }
                if (i > 0 && j + 1 < BOARD_SIZE && tiles[i-1][j+1]->isJunk()) {
                    t = new RegularTile();
                }
                if (j > 0 && tiles[i][j-1]->isJunk()) {
                    t = new RegularTile();
                }
            }
            else {
                t = new RegularTile();
            }
            tiles[i][j] = t;
        }
    }
    pocket.reset(new RegularTile());
    pocket->setPocket(true);
    pocket->flip();
    done = false;
    tiles[0][0] = new RegularTile();
    tiles[BOARD_SIZE - 1][BOARD_SIZE - 2] = new RegularTile();
    tiles[BOARD_SIZE - 1][BOARD_SIZE - 1] = new RegularTile(Opening(NORTH), Opening(SOUTH));
    flow_location = std::make_pair(BOARD_SIZE - 1, BOARD_SIZE - 1);
}

Board::Board(std::vector<Tile*> solution) {
    srand((unsigned)time(0));
    int factor = (BOARD_SIZE*BOARD_SIZE) / solution.size();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if ((i + j) % factor == 0 && !solution.empty()) {
                tiles[i][j] = solution.back();
                solution.pop_back();
            }
            else {
                tiles[i][j] = new RegularTile();
            }
        }
    }
    pocket.reset(new RegularTile());
    pocket->setPocket(true);
    pocket->flip();
    done = false;
    flow_location = std::make_pair(BOARD_SIZE - 1, BOARD_SIZE - 1);
    tiles[BOARD_SIZE - 1][BOARD_SIZE - 1] = new RegularTile(Opening(NORTH), Opening(SOUTH));
}

Board::~Board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            delete tiles[i][j];
        }
    }
}


void Board::increaseFlow() {
    std::cout << "increase flow called" << std::endl;

    int x = flow_location.first;
    int y = flow_location.second;
    Tile* curr_flow = tiles[x][y];
    int next_x = -1;
    int next_y = -1;
    bool found_tile = false;
    switch (curr_flow->getO1()) {
        case NORTH:
            if (y - 1 >= 0 && !tiles[x][y - 1]->hasFlow()
                && (tiles[x][y - 1]->getO1() == SOUTH || tiles[x][y - 1]->getO2() == SOUTH)) {
                next_x = x;
                next_y = y - 1;
                found_tile = true;
            }
            break;
        case EAST:
            if (x + 1 < BOARD_SIZE && !tiles[x + 1][y]->hasFlow()
                && (tiles[x + 1][y]->getO1() == WEST || tiles[x + 1][y]->getO2() == WEST)) {
                next_x = x + 1;
                next_y = y;
                found_tile = true;
            }
            break;
        case SOUTH:
            if (y + 1 < BOARD_SIZE && !tiles[x][y + 1]->hasFlow()
                && (tiles[x][y + 1]->getO1() == NORTH || tiles[x][y + 1]->getO2() == NORTH)) {
                next_x = x;
                next_y = y + 1;
                found_tile = true;
            }
            break;
        case WEST:
            if (x - 1 >= 0 && !tiles[x - 1][y]->hasFlow()
                && (tiles[x - 1][y]->getO1() == EAST || tiles[x - 1][y]->getO2() == EAST)) {
                next_x = x - 1;
                next_y = y;
                found_tile = true;
            }
            break;
        default:
            std::cerr << "invalid enum" << std::endl;
            return;
    }
    if (!found_tile) {
        switch (curr_flow->getO2()) {
            case NORTH:
                if (y - 1 >= 0 && !tiles[x][y - 1]->hasFlow()
                && (tiles[x][y - 1]->getO1() == SOUTH || tiles[x][y - 1]->getO2() == SOUTH)) {
                    next_x = x;
                    next_y = y - 1;
                    found_tile = true;
                }
                break;
            case EAST:
                if (x + 1 < BOARD_SIZE && !tiles[x + 1][y]->hasFlow()
                && (tiles[x + 1][y]->getO1() == WEST || tiles[x + 1][y]->getO2() == WEST)) {
                    next_x = x + 1;
                    next_y = y;
                    found_tile = true;
                }
                break;
            case SOUTH:
                if (y + 1 < BOARD_SIZE && !tiles[x][y + 1]->hasFlow()
                && (tiles[x][y + 1]->getO1() == NORTH || tiles[x][y + 1]->getO2() == NORTH)) {
                    next_x = x;
                    next_y = y + 1;
                    found_tile = true;
                }
                break;
            case WEST:
                if (x - 1 >= 0 && !tiles[x - 1][y]->hasFlow()
                && (tiles[x - 1][y]->getO1() == EAST || tiles[x - 1][y]->getO2() == EAST)) {
                    next_x = x - 1;
                    next_y = y;
                    found_tile = true;
                }
                break;
            default:
                std::cerr << "invalid enum" << std::endl;
                return;
            }
    }
    if (!found_tile) {
        std::cout << "cannot flow to tile[" << next_x << "," << next_y << "]";
        std::cout << " from tile[" << x << "," << y << "]" << std::endl;
        done = true;
        return;
    }
    std::cout << "found tile[" << next_x << "," << next_y << "]" << std::endl;
    // only flow if tile is flipped
    if (tiles[next_x][next_y]->isFlipped()) {
        tiles[next_x][next_y]->flow();
        flow_location = std::make_pair(next_x, next_y);
        std::cout << "flowed" << std::endl;
    }
    // otherwise, the game has been lost
    else {
        done = true;
        std::cout << "flow failed: tile not flipped" << std::endl;
    }
}

void Board::autoComplete() {
    while (!done) {
        increaseFlow();
    }
}

bool Board::swap(Tile & t) {
	// can only swap a tile that is face up and empty
	if (!t.canSwap()) {
		return false;
	}
	Opening temp1 = t.getO1();
	Opening temp2 = t.getO2();
	t.setO1(pocket->getO1());
	t.setO2(pocket->getO2());
	pocket->setO1(temp1);
	pocket->setO2(temp2);
	return true;
}

bool Board::won() {
    return (done && flow_location == finish &&
            (tiles[finish.first][finish.second]->getO1() == WEST
             || tiles[finish.first][finish.second]->getO2() == WEST));
}
bool Board::loss() {
    return (done && (flow_location != finish ||
    (tiles[finish.first][finish.second]->getO1() != WEST
     || tiles[finish.first][finish.second]->getO2() != WEST)));
}
