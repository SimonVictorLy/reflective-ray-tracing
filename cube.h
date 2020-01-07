#include "plane.h"

class cube {

public:

	// constructors

	cube() {
		pl1 = plane(0, 0, 0, 0);
		pl2 = plane(0, 0, 0, 0);
		pl3 = plane(0, 0, 0, 0);
		pl4 = plane(0, 0, 0, 0);
		pl5 = plane(0, 0, 0, 0);
		pl6 = plane(0, 0, 0, 0);
	}

	// methods

	plane getPlane1();

	plane getPlane2();

	plane getPlane3();

	plane getPlane4();

	plane getPlane5();

	plane getPlane6();


	void setPlane1(plane p);

	void setPlane2(plane p);

	void setPlane3(plane p);

	void setPlane4(plane p);

	void setPlane5(plane p);

	void setPlane6(plane p);

	vector3 getNorm1();
	vector3 getNorm2();
	vector3 getNorm3();
	vector3 getNorm4();
	vector3 getNorm5();
	vector3 getNorm6();


	plane pl1, pl2, pl3, pl4, pl5, pl6;
};