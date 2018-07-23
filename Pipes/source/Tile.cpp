#include "../headers/Tile.hpp"


Tile::~Tile() {
}
void Tile::flip() {
	flipped = true;
}

bool Tile::isFlipped() {
    return flipped;
}

Opening Tile::getO1() {
    return o1;
}

Opening Tile::getO2() {
    return o2;
}

void Tile::setO1(Opening o) {
    o1 = o;
}

void Tile::setO2(Opening o) {
    o2 = o;
}
