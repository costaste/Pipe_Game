#include "../headers/RegularTile.hpp"

#include <ctime>
#include <cstdlib>

RegularTile::RegularTile() {
	flipped = false;
	flowed = false;
	isPocket = false;
	o1 = Opening(rand()%4);
	o2 = Opening(rand()%4);
	if (o2 == o1) {
        o2 = Opening((o1 + 1) % 4);
	}

}

RegularTile::RegularTile(Opening opening1, Opening opening2) {
	o1 = opening1;
	o2 = opening2;
	flipped = false;
	flowed = false;
	isPocket = false;
}

RegularTile& RegularTile::operator=(const RegularTile& t) {
    flipped = t.flipped;
    flowed  = t.flowed;
    o1 = t.o1;
    o2 = t.o2;
    return *this;
}

bool RegularTile::hasFlow() {
    return flowed;
}

bool RegularTile::isJunk() {
    return false;
}

bool RegularTile::canSwap() {
    return (flipped && !flowed && !isPocket);
}

void RegularTile::flow() {
    if (!isPocket) flowed = true;
}

void RegularTile::setPocket(bool v)  {
    isPocket = v;
}
