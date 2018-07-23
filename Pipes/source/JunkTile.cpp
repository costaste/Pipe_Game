#include "../headers/JunkTile.hpp"

JunkTile::JunkTile() {
	flipped = false;
}

bool JunkTile::isJunk() {
    return true;
}

bool JunkTile::hasFlow() {
    return false;
}

bool JunkTile::canSwap() {
    return false;
}

void JunkTile::flow() {
    return;
}
