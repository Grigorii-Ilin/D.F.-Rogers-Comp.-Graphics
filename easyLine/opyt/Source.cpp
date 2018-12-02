#include "glut.h"
#include <math.h>

int sign(float delta) {
	int res = delta < 0 ? -1 : 1;
	return res;
}

void easyLine() {
	const int N = 2; // 0=x , 1=y

	const float startP[N] = { 0.0, 0.0 };
	const float endP[N] = { -200.0, 70.0 }; // концы отрезка не совпадают с началом

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

	float currentP[N];
	for (int i = 0; i < N; i++) {
		currentP[i] = startP[i] + 0.5*sign(delta[i]); // округляем величины, а не отбрасываем дробную часть
	}

	for (int currentStep = 1; currentStep < len; currentStep++){
		glVertex2i((int)currentP[0], (int)(currentP[1]));

		for (int i = 0; i < N; i++)		{
			currentP[i] += delta[i];				
		}
	}
}


void Display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 0.0f); // yellow
	easyLine();

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