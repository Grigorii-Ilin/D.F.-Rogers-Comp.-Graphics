#include "glut.h"
#include <math.h>
#include <iostream>

using namespace std;

const int X_ID = 0;
const int Y_ID = 1;

int sign(float delta) {
	int res = delta < 0 ? -1 : 1;
	return res;
}

int sign(int delta) {
	int res = delta < 0 ? -1 : 1;
	return res;
}

void easyLine() {
	const int N = 2; // 0=x , 1=y

	const float startP[N] = { 0.0, 0.0 };
	const float endP[N] = { -200.0, 100.0 }; // концы отрезка не совпадают с началом

	float distance[N];
	for (int i = 0; i < N; i++) {
		distance[i] = endP[i] - startP[i];
	}

	float len = fabs(distance[0]) >= fabs(distance[1]) ? distance[0] : distance[1];
	len = fabs(len);

	float delta[N];
	for (int i = 0; i < N; i++) {
		delta[i] = distance[i] / len;// предполагаем, что большее из приращений = 1-це растра
	}

	float p[N];
	for (int i = 0; i < N; i++) {
		p[i] = startP[i] + 0.5*sign(delta[i]); // округл€ем величины, а не отбрасываем дробную часть
	}

	for (int currentStep = 1; currentStep < len; currentStep++) {
		glVertex2i((int)p[0], (int)p[1]);

		for (int i = 0; i < N; i++) {
			p[i] += delta[i];
		}
	}
}

void BresenhamLineAlgInt() {
	const int N = 2; // 0=x , 1=y

	const int startP[N] = { 0, 3 };
	const int endP[N] = { -200, 103 }; // концы отрезка не совпадают с началом

	int p[N];
	for (int i = 0; i < N; i++) {
		p[i] = startP[i];
	}

	int delta[N]; //модуль рассто€ни€
	int signum[N];
	for (int i = 0; i < N; i++) {
		int dist = endP[i] - startP[i];
		delta[i] = abs(dist);
		signum[i] = sign(dist);
	}

	int longerDeltaCoordIndex = 0;
	// обмен значений x и y в зависимости от углового коэффициента
	if (delta[1] > delta[0]) {
		int temp = delta[0];
		delta[0] = delta[1];
		delta[1] = temp;
		longerDeltaCoordIndex = 1;
	}
	int shorterDeltaCoordIndex = abs(longerDeltaCoordIndex - 1);

	int errPixel = 2 * delta[1] - delta[0]; // иниц. с поправкой на 0,5 пиксела
	for (int currentStep = 0; currentStep < delta[0]; currentStep++) {
		cout << p[0] << ' ' << p[1] << ' ' << errPixel << endl;
		glVertex2i(p[0], p[1]);

		if (errPixel >= 0) {
			p[shorterDeltaCoordIndex] += signum[shorterDeltaCoordIndex];
			errPixel -= 2 * delta[0];
		}

		p[longerDeltaCoordIndex] += signum[longerDeltaCoordIndex];
		errPixel += 2 * delta[1];
	}
}

enum SigmaCase{
	MOVE_HORIZONTAL,
	MOVE_VERTICAL,
	MOVE_DIAGONAL
};

void circleQuad1(const int x, const int r) {
	const int N = 2; // 0=x , 1=y
	int p[N]; //{ x, R };
	p[X_ID] = x;
	p[Y_ID] = r;

	int delta = 2 * (1 - r);
	const int LIM = 0;
	int sigma = 0;
	SigmaCase sigmaCase;

	while (p[Y_ID] > LIM) {
		cout << "x= " << p[X_ID] << "y= " << p[Y_ID] << "delta= " << delta << "sigma=" << sigma << endl;
		glVertex2i(p[X_ID], p[Y_ID]);

		if (delta < 0) {
			sigma = 2 * delta + 2 * p[Y_ID] - 1;
			sigma <= 0 ? sigmaCase = MOVE_HORIZONTAL : sigmaCase = MOVE_DIAGONAL;			
		}
		else if (delta > 0) {
			sigma = 2 * delta + 2 * p[X_ID] - 1;
			sigma <= 0 ? sigmaCase = MOVE_DIAGONAL : sigmaCase = MOVE_VERTICAL;
		}
		else {
			sigmaCase = MOVE_DIAGONAL;
		}

		switch (sigmaCase)		{
		case MOVE_HORIZONTAL:
			p[X_ID]++;
			delta += 2 * p[X_ID] + 1;
			break;
		case MOVE_VERTICAL:
			p[Y_ID]--;
			delta += -2 * p[Y_ID] + 1;
			break;
		case MOVE_DIAGONAL:
			p[X_ID]++;
			p[Y_ID]--;
			delta += 2 * p[X_ID] - 2 * p[Y_ID] + 2;
			break;
		default:
			break;
		}
	}
}

void Display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

	glBegin(GL_POINTS);

	glColor3f(1.0f, 1.0f, 0.0f); // yellow
	easyLine();

	glColor3f(1.0f, 0.0f, 0.0f);
	BresenhamLineAlgInt();

	glColor3f(0.5f, 0.0f, 0.5f);
	circleQuad1(0, 100);

	glEnd();

	glFlush();  // Render now
}


void Initialize() {
	glClearColor(0.8, 1.0, 0.6, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-200.0, 200.0, -200.0, 200.0, -5.0, 5.0);
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 200);
	glutCreateWindow("Easy line algorithm");
	glutDisplayFunc(Display);
	Initialize();
	glutMainLoop();
	return 0;
}