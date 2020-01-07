/* Athabasca University

Comp390 - Introduction to Computer Graphics

Version 4

Cube Class

@author: Simon Ly

@date: Jul 8, 2018

*/

#include <iostream>
#include "GL/glut.h"
#include "cube.h"
#include "math.h"

// colors

vector3 grey(0.9, 0.9, 0.9);

vector3 lightBlue(0.1, 0.1, 0.75);

vector3 darkGrey(0.1, 0.1, 0.1);

vector3 white(1.0, 1.0, 1.0);

// lighting position

vector3 lightPosition(5, 5, 5.0);

plane floorDef(0.0, 4.0, 0.0, 0.0);

// lighting elements

float ambient_coef = 0.3;

float diffuse_coef = 0.7;

float reflect_coef = 0.4;

float local_coef = 0.6;



using namespace std;

// Cube unit
vector3 pt[8] = { { 0,0,0 },{ 0,1,0 },{ 1,0,0 },{ 1,1,0 },{ 0,0,1 },{ 0,1,1 },{ 1,0,1 },{ 1,1,1 } };

// Viewer position
vector3 viewer(5.0, 3.0, 5.0);


// Cubes
cube cube1;
cube cube2;
cube cube3;
// initialize

void initialize() {
	floorDef.setP1(vector3(-5, 0, -5));
	floorDef.setP2(vector3(5, 0, -5));
	floorDef.setP3(vector3(5, 0, 5));
	floorDef.setP4(vector3(-5, 0, 5));
	// set background color

	glClearColor(0.7, 0.9, 0.7, 0.0);
}

void showMatrix() {

	GLfloat m[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, m);


	// print matrix
	for (int i = 0; i<4; i++) {

		cout << m[i * 4] << " " << m[i * 4 + 1] << " " << m[i * 4 + 2] << " " << m[i * 4 + 3] << endl;

	}

	cout << endl;

}

// calculate local color

// local color is intensity * base color

vector3 localColor(vector3 intersect, vector3 baseColor, vector3 normal) {

	// calculate unit vector

	vector3 origin = lightPosition.subtract(intersect);

	vector3 unitVec = origin.normalize();



	// calculate dot product

	float dotProd = unitVec.dot(normal);



	// calculate intensity

	float ambientContr = ambient_coef;

	float diffuseContr = diffuse_coef * dotProd;

	float intensity = ambientContr + diffuseContr;



	if (intensity > 1.0) intensity = 1.0;



	float r = intensity * baseColor.x;

	float g = intensity * baseColor.y;

	float b = intensity * baseColor.z;



	return vector3(r, g, b);

}

vector3 lightFloor(vector3 intersect) {
	vector3 color(0.0, 0.0, 0.0);

	int floorWidth = abs((floorDef.getP1()).x - (floorDef.getP3()).x);
	int floorLength = abs((floorDef.getP1()).z - (floorDef.getP3()).z);
	vector3 floorNormal = vector3(floorDef.a, floorDef.b, floorDef.c);

	if ((intersect.x >= 0 && intersect.z <= 0) || (intersect.x <= 0 && intersect.z >= 0)) {

		if (int(intersect.x / (floorWidth / 8)) % 2) {
			color = int(intersect.z / (floorLength / 8)) % 2 ? localColor(intersect, grey, floorNormal) : localColor(intersect, darkGrey, floorNormal);
		}
		else {
			color = int(intersect.z / (floorLength / 8)) % 2 ? localColor(intersect, darkGrey, floorNormal) : localColor(intersect, grey, floorNormal);
		}
	}
	else {
		if (int(intersect.x / (floorWidth / 8)) % 2) {
			color = int(intersect.z / (floorLength / 8)) % 2 ? localColor(intersect, darkGrey, floorNormal) : localColor(intersect, grey, floorNormal);
		}
		else {
			color = int(intersect.z / (floorLength / 8)) % 2 ? localColor(intersect, grey, floorNormal) : localColor(intersect, darkGrey, floorNormal);
		}
	}
	return color;
}

vector3 RayTracer(vector3 incident, int tracerCalls) {
	vector3 mixedColor(0.0, 0.0, 0.0);
	// Check if the incident vector is too large or if there were too many reflections
	if (tracerCalls > 2) {
		return mixedColor;
	}

	else {
		// intersection point between the incident light and the floor plane
		vector3 fIntersect = floorDef.intersect(incident, lightPosition);

		cube *cubelist[3] = {&cube1, &cube2, &cube3};

		for (int i = 0; i < 3; i++) {
			// intersection point between the incident light and the wall plane
			vector3 wIntersect1 = (cubelist[i]->getPlane1()).intersect(incident, lightPosition);
			vector3 wIntersect2 = (cubelist[i]->getPlane2()).intersect(incident, lightPosition);
			vector3 wIntersect3 = (cubelist[i]->getPlane3()).intersect(incident, lightPosition);
			vector3 wIntersect4 = (cubelist[i]->getPlane4()).intersect(incident, lightPosition);
			vector3 wIntersect5 = (cubelist[i]->getPlane5()).intersect(incident, lightPosition);
			vector3 wIntersect6 = (cubelist[i]->getPlane6()).intersect(incident, lightPosition);

			vector3 color(0.0, 0.0, 0.0);

			if ((incident.z == -(cubelist[i]->getPlane4()).d) &&
				(wIntersect4.y >= ((cubelist[i]->getPlane4()).getP4()).y &&
				(wIntersect4.y < ((cubelist[i]->getPlane4()).getP2()).y)) &&
					(wIntersect4.x >= ((cubelist[i]->getPlane4()).getP4()).x &&
				(wIntersect4.x < ((cubelist[i]->getPlane4()).getP2()).x))) {

				color = localColor(incident, lightBlue, cubelist[i]->getNorm4());
				vector3 reflect = incident.subtract(viewer).reflect(cubelist[i]->getNorm4()); // unit reflection vector
				vector3 floorSect = floorDef.intersect(incident, incident.add(reflect));	// intersect vector againest wall

				mixedColor = RayTracer(floorSect, tracerCalls + 1);

				// If the reflection hit something then mix colors otherwise use original color
				if (mixedColor.x != 0.0 && mixedColor.y != 0.0 && mixedColor.z != 0.0) {
					mixedColor = mixedColor.scalar(reflect_coef).add(color.scalar(local_coef));
				}
				else {
					mixedColor = color;
				}
				break;
			}
			else if ((incident.z == -(cubelist[i]->getPlane2()).d) &&
				(wIntersect2.y >= ((cubelist[i]->getPlane2()).getP1()).y &&
				(wIntersect2.y < ((cubelist[i]->getPlane2()).getP3()).y)) &&
					(wIntersect2.x >= ((cubelist[i]->getPlane2()).getP1()).x &&
				(wIntersect2.x < ((cubelist[i]->getPlane2()).getP3()).x))) {

				color = localColor(incident, lightBlue, cubelist[i]->getNorm2());
				vector3 reflect = incident.subtract(viewer).reflect(cubelist[i]->getNorm2()); // unit reflection vector
				vector3 floorSect = floorDef.intersect(incident, incident.add(reflect));	// intersect vector againest wall

				mixedColor = RayTracer(floorSect, tracerCalls + 1);

				// If the reflection hit something then mix colors otherwise use original color
				if (mixedColor.x != 0.0 && mixedColor.y != 0.0 && mixedColor.z != 0.0) {
					mixedColor = mixedColor.scalar(reflect_coef).add(color.scalar(local_coef));
				}
				else {
					mixedColor = color;
				}
				break;
			}
			else if ((incident.x == -(cubelist[i]->getPlane3()).d) &&
				(wIntersect3.y >= ((cubelist[i]->getPlane3()).getP4()).y &&
				(wIntersect3.y < ((cubelist[i]->getPlane3()).getP2()).y)) &&
					(wIntersect3.z >= ((cubelist[i]->getPlane3()).getP4()).z &&
				(wIntersect3.z < ((cubelist[i]->getPlane3()).getP2()).z))) {

				color = localColor(incident, lightBlue, cubelist[i]->getNorm3());
				vector3 reflect = incident.subtract(viewer).reflect(cubelist[i]->getNorm3()); // unit reflection vector
				vector3 floorSect = floorDef.intersect(incident, incident.add(reflect));	// intersect vector againest wall

				mixedColor = RayTracer(floorSect, tracerCalls + 1);

				if (mixedColor.x != 0.0 && mixedColor.y != 0.0 && mixedColor.z != 0.0) {
					mixedColor = mixedColor.scalar(reflect_coef).add(color.scalar(local_coef));
				}
				else {
					mixedColor = color;
				}
				break;
			}
			else if ((incident.x == -(cubelist[i]->getPlane1()).d) &&
				(wIntersect1.y >= ((cubelist[i]->getPlane1()).getP1()).y &&
				(wIntersect1.y < ((cubelist[i]->getPlane1()).getP3()).y)) &&
					(wIntersect1.z >= ((cubelist[i]->getPlane1()).getP1()).z &&
				(wIntersect1.z < ((cubelist[i]->getPlane1()).getP3()).z))) {

				color = localColor(incident, lightBlue, cubelist[i]->getNorm1());
				vector3 reflect = incident.subtract(viewer).reflect(cubelist[i]->getNorm1()); // unit reflection vector
				vector3 floorSect = floorDef.intersect(incident, incident.add(reflect));	// intersect vector againest wall

				mixedColor = RayTracer(floorSect, tracerCalls + 1);

				if (mixedColor.x != 0.0 && mixedColor.y != 0.0 && mixedColor.z != 0.0) {
					mixedColor = mixedColor.scalar(reflect_coef).add(color.scalar(local_coef));
				}
				else {
					mixedColor = color;
				}
				break;
			}
			else if ((incident.y == -(cubelist[i]->getPlane6()).d) &&
				(wIntersect6.x >= ((cubelist[i]->getPlane6()).getP1()).x &&
				(wIntersect6.x < ((cubelist[i]->getPlane6()).getP3()).x)) &&
					(wIntersect6.z >= ((cubelist[i]->getPlane6()).getP1()).z &&
				(wIntersect6.z < ((cubelist[i]->getPlane6()).getP3()).z))) {

				color = localColor(incident, lightBlue, cubelist[i]->getNorm6());
				vector3 reflect = incident.subtract(viewer).reflect(cubelist[i]->getNorm6()); // unit reflection vector
				vector3 floorSect = floorDef.intersect(incident, incident.add(reflect));	// intersect vector againest wall

																							// this will not hit the floor because the reflection y is positive
				mixedColor = vector3(0.0, 0.0, 0.0);//RayTracer(floorSect, tracerCalls + 1);

				if (mixedColor.x != 0.0 && mixedColor.y != 0.0 && mixedColor.z != 0.0) {
					mixedColor = mixedColor.scalar(reflect_coef).add(color.scalar(local_coef));
				}
				else {
					mixedColor = color;
				}
				break;
			}

			else if ((fIntersect.x >= (floorDef.getP1()).x) && (fIntersect.x < (floorDef.getP3()).x) &&
				(fIntersect.y == floorDef.d) &&
				(fIntersect.z >= (floorDef.getP1()).z) && (fIntersect.z < (floorDef.getP3()).z)) {    // test for inclusion
				mixedColor = lightFloor(incident);
				break;
			}
		}

		return mixedColor;
	}
}

// Create a face of a cube
plane set_quad(GLint n1, GLint n2, GLint n3, GLint n4, GLint shiftx, GLint shiftz) {

	// This determines the normal
	vector3 dir = pt[n2].subtract(pt[n1]).cross(pt[n3].subtract(pt[n1]));
	GLfloat mag = sqrt(abs(dir.x + dir.y + dir.z));
	vector3 norm(dir.x / mag, dir.y / mag, dir.z / mag);
	GLfloat shiftD(0.0);

	if (norm.x != 0) {
		shiftD = pt[n1].x + shiftx;
	}
	else if (norm.y != 0) {
		shiftD = pt[n1].y;
	}
	else {
		shiftD = pt[n1].z + shiftz;
	}
	plane face(norm.x, norm.y, norm.z, -shiftD);
	face.setP1(pt[n1].add(vector3(shiftx, 0, shiftz)));
	face.setP2(pt[n2].add(vector3(shiftx, 0, shiftz)));
	face.setP3(pt[n3].add(vector3(shiftx, 0, shiftz)));
	face.setP4(pt[n4].add(vector3(shiftx, 0, shiftz)));

	return face;
}

void render_quad(plane face, vector3 norm, GLint shiftx, GLint shiftz) {

	vector3 point = face.getP1();
	GLfloat m, n, step;
	n = 0;
	m = 0;
	// This will determine the detail of the ray tracing based on the size of the floorSectall rectangle
	step = 0.01;
	vector3 color(0.0, 0.0, 0.0);

	// for a unit cube the range will always go from zero to 1 on two axis and constant on the last
	for (m = 0; m < 1; m = m + step) {
		for (n = 0; n < 1; n = n + step) {
			
			if (norm.x != 0) {
				color = RayTracer(vector3(point.x, n, m + shiftz), 0);
				//color = vector3(0.0, 0.0, 1.0);
				glColor3f(color.x,color.y,color.z);
				glBegin(GL_POLYGON);
				glVertex3f(point.x, n, m + shiftz);
				glVertex3f(point.x, n, m + step + shiftz);
				glVertex3f(point.x, n + step, m + step + shiftz);
				glVertex3f(point.x, n + step, m + shiftz);
				glEnd();
			}
			else if (norm.z != 0) {
				color = RayTracer(vector3(m + shiftx, n, point.z), 0);
				//color = vector3(0.0, 1.0, 0.0);
				glColor3f(color.x, color.y, color.z);
				glBegin(GL_POLYGON);
				glVertex3f(m + shiftx, n, point.z);
				glVertex3f(m + step + shiftx, n, point.z);
				glVertex3f(m + step + shiftx, n + step, point.z);
				glVertex3f(m + shiftx, n + step, point.z);
				glEnd();
			}
			else {
				// norm.y !=0
				color = RayTracer(vector3(m + shiftx, point.y, n + shiftz), 0);
				//color = vector3(0.0, 0.0, 0.0);
				glColor3f(color.x, color.y, color.z);
				glBegin(GL_POLYGON);
				glVertex3f(m + shiftx, point.y, n + shiftz);
				glVertex3f(m + step + shiftx, point.y, n + shiftz);
				glVertex3f(m + step + shiftx, point.y, n + step + shiftz);
				glVertex3f(m + shiftx, point.y, n + step + shiftz);
				glEnd();
			}

		}
		//n = 0;
	}
}

// create a cube
cube create_cube(GLint translateX, GLint translateZ) {

	// references each side of the cube
	// always give a counter-clockwise for the correct direction for a cross product
	// the first and third point are interchangable as well as the second and last point
	cube cubeRef;
	// create cube object
	cubeRef.setPlane1(set_quad(2, 3, 7, 6, translateX, translateZ)); // right cube
	cubeRef.setPlane2(set_quad(0, 1, 3, 2, translateX, translateZ)); // back cube
	cubeRef.setPlane3(set_quad(4, 5, 1, 0, translateX, translateZ)); // left cube
	cubeRef.setPlane4(set_quad(6, 7, 5, 4, translateX, translateZ)); // front cube
	cubeRef.setPlane5(set_quad(2, 6, 4, 0, translateX, translateZ)); // bottom cube
	cubeRef.setPlane6(set_quad(1, 5, 7, 3, translateX, translateZ)); // top cube 
	
	return cubeRef;
}

void render_cube(cube *cubeRef, GLint translateX, GLint translateZ) {
	// render the cube object
	render_quad(cubeRef->getPlane5(), cubeRef->getNorm5(), translateX, translateZ);
	render_quad(cubeRef->getPlane3(), cubeRef->getNorm3(), translateX, translateZ);
	render_quad(cubeRef->getPlane2(), cubeRef->getNorm2(), translateX, translateZ);

	render_quad(cubeRef->getPlane1(), cubeRef->getNorm1(), translateX, translateZ);
	render_quad(cubeRef->getPlane4(), cubeRef->getNorm4(), translateX, translateZ);
	render_quad(cubeRef->getPlane6(), cubeRef->getNorm6(), translateX, translateZ);
}

void render_scene() {

	// anti-aliasing - Smooth
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	// Generate cube
	cube1 = create_cube(0, 0);
	cube2 = create_cube(3, 0);
	cube3 = create_cube(0, 3);

	render_cube(&cube1, 0, 0);
	render_cube(&cube2, 3, 0);
	render_cube(&cube3, 0, 3);
}


GLuint rectCubes;
void setUp() {
	// create a display list with unique identifier
	rectCubes = glGenLists(1);

	// record display list for the house
	glNewList(rectCubes, GL_COMPILE);
	render_scene();
	glEndList();

}

void floor() {

	GLfloat m, n, step;
	n = 0;
	m = 0;
	// This will determine the detail of the ray tracing based on the size of the floorSectall rectangle
	step = 0.01;
	vector3 color(0.0, 0.0, 0.0);
	// for a unit cube the range will always go from zero to 1 on two axis and constant on the last
	for (m = (floorDef.getP1()).x; m < (floorDef.getP3()).x; m = m + step) {
		for (n = (floorDef.getP1()).z; n < (floorDef.getP3()).z; n = n + step) {

			color = lightFloor(vector3(m, 0, n));
			glColor3f(color.x, color.y, color.z);
			glBegin(GL_POLYGON);

			glVertex3f(m, 0, n);
			glVertex3f(m, 0, n + step);
			glVertex3f(m + step, 0, n + step);
			glVertex3f(m + step, 0, n);

			glEnd();
		}
	}
}

// render

void render() {
	// set the matrix mode to modelview
	glMatrixMode(GL_MODELVIEW);
	// call the cube display list
	
	floor();
	
	glPushMatrix();

	glCallList(rectCubes);
	//showMatrix();
}

// display registry

void display(void) {

	// clear buffers storing color and depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// load new identity matrix
	glLoadIdentity();

	// Depth_test

	glEnable(GL_DEPTH_TEST);

	// viewing

	gluLookAt(viewer.x, viewer.y, viewer.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	// Initialize the display list
	setUp();

	render();

	glutSwapBuffers();

}

// reshape registry

void reshape(int w, int h) {

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	// apply clipping planes
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 40.0);

	glMatrixMode(GL_MODELVIEW);

}

// main program

int main(int argc, char **argv)

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);

	glutInitWindowPosition(100, 100);



	int windowHandle = glutCreateWindow("Athabasca University - Comp390 TME 2-1");

	glutSetWindow(windowHandle);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	initialize();

	glutMainLoop();

}
