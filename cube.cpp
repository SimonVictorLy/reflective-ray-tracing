#include <iostream>
#include <math.h>
#include "cube.h"

using namespace std;

void cube::setPlane1(plane p) {pl1 = p;}

void cube::setPlane2(plane p) {pl2 = p;}

void cube::setPlane3(plane p) {pl3 = p;}

void cube::setPlane4(plane p) {pl4 = p;}

void cube::setPlane5(plane p) {pl5 = p;}

void cube::setPlane6(plane p) {pl6 = p;}

plane cube::getPlane1() {return pl1;}

plane cube::getPlane2() {return pl2;}

plane cube::getPlane3() {return pl3;}

plane cube::getPlane4() {return pl4;}

plane cube::getPlane5() {return pl5;}

plane cube::getPlane6() {return pl6;}

vector3 cube::getNorm1() {
	// under the assumption that the points were inputted in a counter clockwise direction
	return (pl1.getP2()).subtract(pl1.getP1()).cross((pl1.getP3()).subtract(pl1.getP1()));
}
vector3 cube::getNorm2() {
	// under the assumption that the points were inputted in a counter clockwise direction
	return (pl2.getP2()).subtract(pl2.getP1()).cross((pl2.getP3()).subtract(pl2.getP1()));
}
vector3 cube::getNorm3() {
	// under the assumption that the points were inputted in a counter clockwise direction
	return (pl3.getP2()).subtract(pl3.getP1()).cross((pl3.getP3()).subtract(pl3.getP1()));
}
vector3 cube::getNorm4() {
	// under the assumption that the points were inputted in a counter clockwise direction
	return (pl4.getP2()).subtract(pl4.getP1()).cross((pl4.getP3()).subtract(pl4.getP1()));
}
vector3 cube::getNorm5() {
	// under the assumption that the points were inputted in a counter clockwise direction
	return (pl5.getP2()).subtract(pl5.getP1()).cross((pl5.getP3()).subtract(pl5.getP1()));
}
vector3 cube::getNorm6() {
	// under the assumption that the points were inputted in a counter clockwise direction
	return (pl6.getP2()).subtract(pl6.getP1()).cross((pl6.getP3()).subtract(pl6.getP1()));
}