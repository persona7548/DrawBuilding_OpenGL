#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include<math.h>
#include<stdlib.h>
//q = 쉐이딩 적용, w 텍스쳐 on/off, a = Y축회전, 스포트라이트 회전 

static int SpinAngle =0;
bool TextureOn = false;
bool WireframeOn = true;
GLenum ShadeModel = GL_FLAT;
int random[36];
GLUquadricObj * quadric;
GLUquadricObj* cyl;
float Distance = 15.0;
float RotateX = 0.0;
float RotateY = 0.0;
float viewTransY = 0.0;
float viewTransX = 0.0;
int pX = 0;
int pY = 0;
unsigned char mbuttons[3] = { 0 };

void MyLightInit() {//실습코드 이용
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };	//전역 주변반사

	GLfloat light0_ambient[] = { 0.4, 0.4, 0.4, 1.0 }; 	//0번 광원 특성(전체적 조명)
	GLfloat light0_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light1_ambient[] = { 0.25, 0.25, 0.0, 1.0 };	//1번 광원 특성(햇빛)
	GLfloat light1_diffuse[] = { 0.9, 0.9, 0.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat material_ambient[] = { 0.4, 0.4, 0.4, 1.0 };	//물체 특성
	GLfloat material_diffuse[] = { 0.9, 0.9, 0.9, 1.0};
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 15.0 };

	glShadeModel(GL_SMOOTH);	//구로 셰이딩
	glEnable(GL_DEPTH_TEST);	//깊이 버퍼 활성화
	glEnable(GL_LIGHTING);		//조명 활성화

	glEnable(GL_LIGHT0);		//0번 광원 활성화
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);	//0번 광원 특성할당
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT1);		//1번 광원 활성화
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);	//1번 광원 특성할당
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);//물체 특성할당
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);//전역주변반사 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //근접시점
}

void Draw_Road() {
	glPushMatrix();
	glBegin(GL_QUADS);//도로
	glTranslatef(0.0, -5.10, 0);
	glColor3f(0.3, 0.3, 0.35);
	glVertex3f(50.0, 0.0, 1.0);
	glVertex3f(50.0, 0.0, -1.0);
	glVertex3f(-50.0, 0.0, -1.0);
	glVertex3f(-50.0, 0.0, 1.0);
	glEnd();
	glTranslatef(-50.0, 0.01, 0);
	for (int i = 0; i < 50; i++) {// 중앙표시선
		glTranslatef(2.0, 0, 0); 
		glBegin(GL_QUADS);
		glColor3f(0.9, 0.9, 0.9);
		glVertex3f(0.5, 0.0, 0.15);
		glVertex3f(0.5, 0.0, -0.15);
		glVertex3f(-0.5, 0.0, -0.15);
		glVertex3f(-0.5, 0.0, 0.15);
		glEnd();
	}
	glPopMatrix();

}

void DrawGround()
{
	glColor3f(0.7, 0.7, 0.7);
	if (WireframeOn)
		glBegin(GL_LINES);
	else
		glBegin(GL_QUADS);
	for (int i = -10; i <= 10; ++i) {
		glVertex3f(i * 5, -0.2, -50);
		glVertex3f(i * 5, -0.2, 50);

		glVertex3f(50, -0.2, i * 5);
		glVertex3f(-50, -0.2, i * 5);
	}
	glEnd();
}

void Draw_Building1() {
	glPushMatrix();
	glColor3f(0.7f, 0.2f, 0.2f);
	glTranslatef(0, 2.3, 0);
	glScalef(1.0f, 5.0f, 2.0f);
	glutSolidCube(1);
	glPopMatrix();
}

void Draw_Building2() {
	glPushMatrix();
	glTranslatef(0, -0.15, 0);
	glColor3f(0.3f, 0.2f, 0.2f);
	glScalef(4.5, 1.0, 2.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.01, 0.4, 0.2);
	glColor3f(0.968f, 0.921f, 0.45f);
	glScalef(4.7, 0.2, 3.2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.85, 0);
	glColor3f(0.3f, 0.2f, 0.2f);
	glScalef(4.5, 0.8, 2.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1, 1.5, 0);
	glColor3f(0.0f, 0.2f, 0.2f);
	glScalef(0.7, 0.6, 1.2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, 1.5, 0);
	glColor3f(0.0f, 0.2f, 0.2f);
	glScalef(0.7, 0.6, 1.2);
	glutSolidCube(1);
	glPopMatrix();

}

void Draw_Building3() {

	glColor3f(0.3f, 0.2f, 0.7f);
	glTranslatef(0, 1.8, 0.0);

	glPushMatrix();
	glScalef(1.5f, 4.0f, 3.2f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.1, 1);
	glScalef(1.0f, 4.2f, 1.8f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -0.8, 0);
	glScalef(1.0f, 2.8f, 1.0f);
	glutSolidCube(1);
	glPopMatrix();
}
void Draw_Building4() {

	glPushMatrix();
	glTranslatef(0, 3.2, 0);
	glColor3f(0.3f, 0.5f, 0.6f);
	glScalef(1.5f, 8.0f, 1.5f);
	glutSolidCube(1);
	glPopMatrix();

}
void Draw_Building5() {
	glPushMatrix();
	glColor3f(0.3f, 0.5f, 0.7f);
	glScalef(2.0f, 5.0f, 2.0f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 0);
	glColor3f(0.3f, 0.5f, 0.7f);
	glScalef(1.0f, 6.5f, 1.0f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.5, 0);
	glColor3f(0.3f, 0.5f, 0.7f);
	glScalef(0.5f, 8.0f, 0.5f);
	glutSolidCube(1);
	glPopMatrix();
}

void Draw_Tree()
{
	cyl = gluNewQuadric();
	glPushMatrix();
	glColor3f(0.3f, 0.9f, 0.3f);
	glTranslatef(0, 1.2, 0);
	glScalef(0.5, 1.75, 0.5);
	glutSolidCube(1);
	
	glColor3f(0.439f, 0.333f, 0.239f);

	glTranslatef(0, -0.4, 0);
	glRotatef(90, 180, 1, 0);
	gluCylinder(cyl, 0.3, 0.3, 0.3, 20, 20);
	glPopMatrix();
}

void Draw_part() //프리셋 0
{
	glPushMatrix();
	glTranslatef(0, 0.0, 4);
	Draw_Building1();
	glTranslatef(-1.5, 0.0, 0);
	Draw_Building1();
	glTranslatef(-1.5, 0.0, 0);
	Draw_Building1();
	glTranslatef(-1.5, 0.0, 0);
	Draw_Building1();

	glTranslatef(4.5, 0.0, -3);
	Draw_Building1();
	glTranslatef(-1.5, 0.0, 0);
	Draw_Building1();
	glTranslatef(-1.5, 0.0, 0);
	Draw_Building1();
	glTranslatef(-1.5, 0.0, 0);
	Draw_Building1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5, 0.5, -5.0);
	Draw_Building2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0, -0.19, -5.0);
	glColor3f(0.7, 1.0, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(2.0, 0.0, 1.0);
	glVertex3f(2.0, 0.0, -1.0);
	glVertex3f(-1.5, 0.0, -1.0);
	glVertex3f(-1.5, 0.0, 1.0);
	glEnd();

	glTranslatef(-0.8, 0.0, 0.0);
	Draw_Tree();

	glTranslatef(1, 0.0, 0);
	Draw_Tree();

	glTranslatef(1, 0.0, 0);
	Draw_Tree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0, -0.19, 2.0);
	glColor3f(0.7, 1.0, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(2.0, 0.0, 2.0);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(-2.5, 0.0, -2.0);
	glVertex3f(-2.5, 0.0, 2.0);
	glEnd();

	glTranslatef(1.6, 0.0, 2.4);
	for (int i = 0; i < 4; i++) {
		glTranslatef(-4, 0.0, -1);
		for (int j = 0; j < 4; j++) {
			glTranslatef(1, 0.0, 0);
			Draw_Tree();
		}
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.1, -0.19, -1.5);
	glColor3f(0.3, 0.3, 0.35);
	glBegin(GL_QUADS);
	glVertex3f(6.9, 0.0, 0.5);
	glVertex3f(6.9, 0.0, -0.5);
	glVertex3f(-6.9, 0.0, -0.5);
	glVertex3f(-6.9, 0.0, 0.5);
	glEnd();
	glPopMatrix();
}
void Draw_part1()  //프리셋 1
{
	glPushMatrix();
	glTranslatef(-8,0,2);
	for (int i = 0; i < 2; i++) {
		glTranslatef(3, 0.0, -9);
		for (int j = 0; j < 3; j++) {
			glTranslatef(0, 0.0, 3);
			Draw_Building4();
		}
	}

	glPushMatrix();
	glTranslatef(2.5, -0.1, -2.3);
	glColor3f(0.3, 0.3, 0.35);
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(6.9, 0.0, 0.5);
	glVertex3f(6.9, 0.0, -0.5);
	glVertex3f(-6.9, 0.0, -0.5);
	glVertex3f(-6.9, 0.0, 0.5);
	glEnd();
	glPopMatrix();

	glTranslatef(4.5, 0.0, -4);
	Draw_Building3();
	glTranslatef(0, -1.8, 4);
	Draw_Building3();
	glPopMatrix();


}
void Draw_part2()  //프리셋 2
{
	glPushMatrix();
	glTranslatef(-3.0, 0.0, -5);
	Draw_Building2();
	glTranslatef(0.0, 0.0, 4);
	Draw_Building2();
	glTranslatef(0.0, 0.0, 4);
	Draw_Building2();

	glTranslatef(7.5, 0.0, -6);
	Draw_Building3();
	glTranslatef(0, -1.8, 6);
	Draw_Building3();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, -0.18, -1.0);
	glColor3f(0.3, 0.3, 0.35);
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(6.9, 0.0, 0.5);
	glVertex3f(6.9, 0.0, -0.5);
	glVertex3f(-6.9, 0.0, -0.5);
	glVertex3f(-6.9, 0.0, 0.5);
	glEnd();
	glPopMatrix();
}
void Draw_part3()  //프리셋 3
{
	glPushMatrix();
	glTranslatef(-7.0, 2.0, 1);
	for (int i = 0; i < 2; i++) {
		glTranslatef(3, 0.0, -9);
		for (int j = 0; j < 3; j++) {
			glTranslatef(0, 0.0, 3);
			Draw_Building5();

		}
	}
	glTranslatef(1, -1.3, 2);
	for (int i = 0; i < 2; i++) {
		glTranslatef(3, 0.0, -9);
		for (int j = 0; j < 3; j++) {
			glTranslatef(0, 0.0, 3);
			Draw_Building4();
		}
	}
	glPopMatrix();

}

void DrawScene() //전체적인 도시 그려내는 부분
{
	DrawGround(); //지면 배치

	glPushMatrix(); // 도로 배치부분
	glTranslatef(0.0, 0.0, -60);
	glTranslatef(0.0, -0.19, 0);
	for (int i = 0; i < 7;i++) {
		glColor3f(0.3, 0.3, 0.7);
		glTranslatef(0, 0, 15);
		Draw_Road();
	}
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0.0, -0.10, -60);
	for (int i = 0; i < 7; i++) {
		glColor3f(0.3, 0.3, 0.7);
		glTranslatef(0, 0, 15);
		Draw_Road();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-38.0, 0.0, -37);
	for (int i=0;i<6;i++) {
		for (int j=0;j<6;j++) {
			switch (random[j + (j*i)])//프리셋들을 랜덤하게 배치
			{
			case 0:
				Draw_part();
				break;
			case 1:
				Draw_part1();
				break;
			case 2:
				Draw_part2();
				break;
			case 3:
				Draw_part3();
				break;
			}
			glTranslatef(0, 0.0, 15);
		}
		glTranslatef(15, 0.0,-90);
	}
	glPopMatrix();

}


void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q') // ShadeModel  부분
	{
		if (ShadeModel == GL_FLAT)
			ShadeModel = GL_SMOOTH;
		else 
			ShadeModel = GL_FLAT;
	}
	else if (key == 'w') //wire <-> texture 변환부분
		WireframeOn = !WireframeOn;
	else if (key == 'a') //도시 시점 회전 부분
		RotateY += 5;
	else if (key == 's') {  //스포트라이트 이동 부분
		SpinAngle = (SpinAngle + 15) % 360;
		glutPostRedisplay();
		if (SpinAngle > 170) {//밤낮 설정
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
		}
		else if (SpinAngle <= 160) {
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
		}
	}

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) //마우스로 시점 변환하는 부분
{
	pX = x;
	pY = y;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mbuttons[0] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_MIDDLE_BUTTON:
		mbuttons[1] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_RIGHT_BUTTON:
		mbuttons[2] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Motion(int x, int y)
{
	int dx = x - pX;
	int dy = y - pY;
	pX = x;
	pY = y;

	if (mbuttons[0])
	{
		RotateX += (float) 0.05f * dy;
		RotateY += (float) 0.05f * dx;
	}
	else if (mbuttons[1])
	{
		viewTransX += (float) 0.05f * dx;
		viewTransY -= (float) 0.05f * dy;
	}
	else if (mbuttons[2])
	{
		viewTransX += (float) 0.05f * dx;
		Distance += (float) 0.05f * dy;
	}
	glutPostRedisplay();
}



void MyDisplay()
{
	GLfloat LightPosition0[] = { 30.0, 20.0, 30.0, 1.0 };	//0번 광원위치
	GLfloat LightPosition1[] = { 30, 15.0, 30.0, 1.0 };	//1번 광원위치(스포트라이트 방식의 태양)
	GLfloat LightDirection1[] = { -0.5, -1.0, -1.0 };	//1번 광원 방향
	GLfloat SpotAngle1[] = {25.0 };			//1번 광원 각도
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);							//모델뷰 행렬
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(50.0, 30.0, 50.0, 0.0, -20.0, -30.0, 0.0, 1.0, 0.0);	//초기 시점 설정
	glTranslatef(0.3, 0.3, 0.0);					//모델변환
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);	//위치성 광원

	glPushMatrix(); //스포트라이트 이동 부분
	glRotatef(SpinAngle, 0,0,1);
	glTranslatef(0.0, 0.0, 1.5);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);	//스포트라이트
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LightDirection1);	//방향
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, SpotAngle1);	         //각도
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);			//코사인 승수
	glPopMatrix();

	glTranslatef(0, 0, -Distance);
	glTranslatef(viewTransX, viewTransY, 0);
	glRotatef(RotateY, 0.0, 1.0, 0.0);
	glRotatef(RotateX, 1.0, 0.0, 0.0);
	

	if (ShadeModel == GL_FLAT)  //ShadeModel 부분
		glShadeModel(GL_FLAT);
	else if (ShadeModel == GL_SMOOTH)
		glShadeModel(GL_SMOOTH);

	if (TextureOn)//texture 모드시 설정
	{
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(quadric, GL_TRUE);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		gluQuadricTexture(quadric, GL_FALSE);
	}

	if (WireframeOn) //wireframe모드시 설정
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_COLOR_MATERIAL);
		gluQuadricNormals(quadric, GLU_NONE);
		gluQuadricDrawStyle(quadric, GLU_LINE);
		gluQuadricTexture(quadric, GL_FALSE);
	}
	else
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluQuadricDrawStyle(quadric, GLU_FILL);
	}
	DrawScene();
	glutSwapBuffers();
	glPopMatrix();
}

int main(int argc, char** argv)
{
	srand((unsigned)time(NULL)); //도시를 랜덤하게 구현하기 위한 함수
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
			random[j + (j*i)] = rand() % 4;;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glutInitWindowSize(1200, 1200);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2013270348_김태현");

	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glEnable(GL_DEPTH_TEST);
	MyLightInit();
	glutDisplayFunc(MyDisplay);
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);
	glLineWidth(3);

	glutMainLoop();
}