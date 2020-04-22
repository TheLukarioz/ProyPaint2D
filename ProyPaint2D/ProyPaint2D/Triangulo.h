#pragma once
#include <vector>
#include <iostream>
#include <math.h>
#include <GL/glut.h>

using namespace std;

//----------------------------------------------------------------------------
double VEL_TIEMPO = 0.1;

double Tiempo = 0.0;

double TiempoRotacion, TiempoTraslacion, TiempoEscalado;

double Giro, SentidoX, SentidoY, TrasX, TrasY, EscaladoAD;

int R, E, T;
//----------------------------------------------------------------------------
int ALTO = 768;
int ANCHO = 1366;
//----------------------------------------------------------------------------
struct TPunto {
	GLdouble x;
	GLdouble y;
	GLfloat R;
	GLfloat G;
	GLfloat B;
}P1, P2, P3;

//----------------------------------------------------------------------------
struct TTriangulo {
	TPunto P1;
	TPunto P2;
	TPunto P3;
	TPunto BC;
	bool Sel;
}Triangulo;

//----------------------------------------------------------------------------
struct TColor {
	GLfloat R;
	GLfloat G;
	GLfloat B;
}ColorSel;


//----------------------------------------------------------------------------
int NroClick = 0;

int Cordx, Cordy;

int rx, ry;

bool band = false;

bool p1b = false, p2b = false;

vector<TTriangulo> Triangulos;

vector<TColor> Colores;

char Tecla = 'D';

char cEventoBoton = 'S'; //Evento Inicial Seleccionar

int unsigned Opcion = 0;

int TriSel = -1;

int avance = 5;

double escala = 0.05;

double FactorEscala = 1.0;

double angulo = 3.1416 / 180;

void CuadroColor() {

	glColor3f(ColorSel.R, ColorSel.G, ColorSel.B);
	glLineWidth(1.0);
	glBegin(GL_QUADS);
	glVertex2d(ANCHO - 200 - 10, 10 + 80);
	glVertex2d(ANCHO - 200 - 10, 30 + 80);
	glVertex2d(ANCHO - 200 - 30, 30 + 80);
	glVertex2d(ANCHO - 200 - 30, 10 + 80);
	glEnd();
}

void DibujaTriangulos() {

	// LIMPIA VENTANA Y DEFINE GROSOR DE LINEA
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(1.0);

	// DIBUJA TRIANGULOS
	for (int unsigned i = 0; i < Triangulos.size(); i++) {
		glBegin(GL_TRIANGLES);

		glColor3f(Triangulos[i].P1.R, Triangulos[i].P1.G, Triangulos[i].P1.B); // establece el color
		glVertex2d(Triangulos[i].P1.x, Triangulos[i].P1.y); // dibuja el vertice

		glColor3f(Triangulos[i].P2.R, Triangulos[i].P2.G, Triangulos[i].P2.B);
		glVertex2d(Triangulos[i].P2.x, Triangulos[i].P2.y);

		glColor3f(Triangulos[i].P3.R, Triangulos[i].P3.G, Triangulos[i].P3.B);
		glVertex2d(Triangulos[i].P3.x, Triangulos[i].P3.y);

		glEnd();

		// DIBUJA BORDE DE TRIANGULO SELECCIONADO
		if (Triangulos[i].Sel) {

			glLineWidth(2.0);
			glColor3f(1, 1, 1);

			glBegin(GL_LINES);
			glVertex2d(Triangulos[i].P1.x, Triangulos[i].P1.y);
			glVertex2d(Triangulos[i].P2.x, Triangulos[i].P2.y);
			glEnd();

			glBegin(GL_LINES);
			glVertex2d(Triangulos[i].P1.x, Triangulos[i].P1.y);
			glVertex2d(Triangulos[i].P3.x, Triangulos[i].P3.y);
			glEnd();

			glBegin(GL_LINES);
			glVertex2d(Triangulos[i].P3.x, Triangulos[i].P3.y);
			glVertex2d(Triangulos[i].P2.x, Triangulos[i].P2.y);
			glEnd();

		}
	}

	// DIBUJA PRIMERA LINEA DE TRIANGULO EN CASO DE HABER TRAZADO
	// Si se hizo clic en los 2 primeros vertices del triangulo
	// debe dibujarse el primer lado del triangulo fijo de P1 hasta P2
	if (p1b && p2b) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex2d(P1.x, P1.y);
		glVertex2d(P2.x, P2.y);
		glEnd();
	}

	CuadroColor();

	// Color de trazado para trazado de nuevos triángulos
	glColor3f(0.0f, 0.0f, 0.0f);

	glFlush();
}

void TrasladaTriangulo(int NroTri, double x, double y) {
	if (NroTri >= 0) {

		// Agrega (x, y) a cada vértice del triángulo

		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x + x;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y + y;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x + x;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y + y;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x + x;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y + y;

		// Recalcula centroide
		Triangulos[NroTri].BC.x = (Triangulos[NroTri].P1.x + Triangulos[NroTri].P2.x + Triangulos[NroTri].P3.x) / 3;
		Triangulos[NroTri].BC.y = (Triangulos[NroTri].P1.y + Triangulos[NroTri].P2.y + Triangulos[NroTri].P3.y) / 3;

		DibujaTriangulos();
	}
}

void ArrastrarMouse(int x, int y)
{
	// Calcula la diferencia entre la posición anterior y actual del mouse
	int difx = (x - rx);
	int dify = -(y - ry);
	// Traslada el triangulo seleccionado
	if (TriSel >= 0) {
		TrasladaTriangulo(TriSel, difx, dify);
		// Actualiza la coordenada del mouse para la siguiente iteracion
		rx = x;
		ry = y;
	}
}

void MoverMouse(int x, int y) {
	// Si band == true, entonces debe dibujarse
	if (band == true) {
		Cordx = x;
		Cordy = ALTO - y;

		// Limpia escena
		glClear(GL_COLOR_BUFFER_BIT);
		// dibuja los triangulos existentes 
		// y la primera linea del siguiente triangulo si existe
		DibujaTriangulos();

		// SE HIZO EL PRIMER CLIC (UN VERTICE FIJO)
		// Entoces dibujar el lado1 de P1 hasta la posicion del mouse
		if (p1b && !p2b) {
			glBegin(GL_LINES);
			glVertex2d(P1.x, P1.y);
			glVertex2d(Cordx, Cordy);
			glEnd();
		}

		// SE HIZO EL SEGUNDO CLIC (UN VERTICE FIJO)
		// Entonces trazar el segundo y tercer lado del triangulo
		// 2° lado: de P2 hasta el mouse, 3° lado: de P1 hasta el mouse 
		if (p1b && p2b) {
			// 2° lado: de P2 hasta la posicion del mouse
			glBegin(GL_LINES);
			glVertex2d(P2.x, P2.y);
			glVertex2d(Cordx, Cordy);
			glEnd();
			// 3° lado: de la posicion del mouse hasta P1 
			glBegin(GL_LINES);
			glVertex2d(Cordx, Cordy);
			glVertex2d(P1.x, P1.y);
			glEnd();
		}

		glFlush();
	}
}

void EscalaTriangulo(int NroTri, double escala) {
	if (NroTri >= 0) {

		// Almacena centroide
		double cx = Triangulos[NroTri].BC.x;
		double cy = Triangulos[NroTri].BC.y;

		// Trasladar el centroide a (0, 0)
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y - Triangulos[NroTri].BC.y;

		// Escala triángulo 
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x * escala;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y * escala;;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x * escala;;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y * escala;;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x * escala;;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y * escala;;

		// Trasladar centroide a la coordenada original
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x + cx;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y + cy;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x + cx;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y + cy;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x + cx;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y + cy;

		// Recalcular centroide
		Triangulos[NroTri].BC.x = (Triangulos[NroTri].P1.x + Triangulos[NroTri].P2.x + Triangulos[NroTri].P3.x) / 3;
		Triangulos[NroTri].BC.y = (Triangulos[NroTri].P1.y + Triangulos[NroTri].P2.y + Triangulos[NroTri].P3.y) / 3;

		DibujaTriangulos();
	}
}

void RotaTriangulo(double angulo, int NroTri, double sentido) {
	if (NroTri >= 0) {

		// Almacena centroide
		double cx = Triangulos[NroTri].BC.x;
		double cy = Triangulos[NroTri].BC.y;

		// Trasladar centroide a (0, 0)
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y - Triangulos[NroTri].BC.y;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x - Triangulos[NroTri].BC.x;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y - Triangulos[NroTri].BC.y;

		// Rotar el angulo solicitado aplicando senos y cosenos
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x * cos(angulo) - Triangulos[NroTri].P1.y * sin(angulo) * sentido;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.x * sin(angulo) * sentido + Triangulos[NroTri].P1.y * cos(angulo);

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x * cos(angulo) - Triangulos[NroTri].P2.y * sin(angulo) * sentido;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.x * sin(angulo) * sentido + Triangulos[NroTri].P2.y * cos(angulo);

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x * cos(angulo) - Triangulos[NroTri].P3.y * sin(angulo) * sentido;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.x * sin(angulo) * sentido + Triangulos[NroTri].P3.y * cos(angulo);

		// Trasladar centroide a su coordenada original
		Triangulos[NroTri].P1.x = Triangulos[NroTri].P1.x + cx;
		Triangulos[NroTri].P1.y = Triangulos[NroTri].P1.y + cy;

		Triangulos[NroTri].P2.x = Triangulos[NroTri].P2.x + cx;
		Triangulos[NroTri].P2.y = Triangulos[NroTri].P2.y + cy;

		Triangulos[NroTri].P3.x = Triangulos[NroTri].P3.x + cx;
		Triangulos[NroTri].P3.y = Triangulos[NroTri].P3.y + cy;

		// Recalcular centroide
		Triangulos[NroTri].BC.x = (Triangulos[NroTri].P1.x + Triangulos[NroTri].P2.x + Triangulos[NroTri].P3.x) / 3;
		Triangulos[NroTri].BC.y = (Triangulos[NroTri].P1.y + Triangulos[NroTri].P2.y + Triangulos[NroTri].P3.y) / 3;

		DibujaTriangulos();
	}
}

int SeleccionaTriangulo(int x, int y) {
	y = ALTO - y;
	TriSel = -1;

	// Analiza cada triangulo del vector de final al inicio
	for (int k = Triangulos.size() - 1; k >= 0; k--) {
		// Calcula productos vectoriales
		double oPAB = (Triangulos[k].P1.x - x) * (Triangulos[k].P2.y - y) - (Triangulos[k].P2.x - x) * (Triangulos[k].P1.y - y);
		double oPBC = (Triangulos[k].P2.x - x) * (Triangulos[k].P3.y - y) - (Triangulos[k].P3.x - x) * (Triangulos[k].P2.y - y);
		double oPCA = (Triangulos[k].P3.x - x) * (Triangulos[k].P1.y - y) - (Triangulos[k].P1.x - x) * (Triangulos[k].P3.y - y);
		// Determinar si (x, y) pertenece al triangulo, si es asi 
		// retorna el número de triangulo
		if ((oPAB > 0 && oPBC > 0 && oPCA > 0) || (oPAB < 0 && oPBC < 0 && oPCA < 0))
		{
			if (TriSel == -1) {
				TriSel = k;
				Triangulos[k].Sel = true;
			}
			else
				Triangulos[k].Sel = false;
		}
		else
			Triangulos[k].Sel = false;
	}
	return TriSel;
}

void RepintaVectice(int x, int y) {

	int tri_sel = SeleccionaTriangulo(x, y);
	if (tri_sel >= 0) {
		y = ALTO - y;
		double DP1_XY = sqrt(pow(x - Triangulos[tri_sel].P1.x, 2) + pow(y - Triangulos[tri_sel].P1.y, 2));
		double DP2_XY = sqrt(pow(x - Triangulos[tri_sel].P2.x, 2) + pow(y - Triangulos[tri_sel].P2.y, 2));
		double DP3_XY = sqrt(pow(x - Triangulos[tri_sel].P3.x, 2) + pow(y - Triangulos[tri_sel].P3.y, 2));
		if (DP1_XY <= DP2_XY && DP1_XY <= DP3_XY) {
			Triangulos[tri_sel].P1.R = ColorSel.R;
			Triangulos[tri_sel].P1.G = ColorSel.G;
			Triangulos[tri_sel].P1.B = ColorSel.B;
		}
		else if (DP2_XY <= DP1_XY && DP2_XY <= DP3_XY) {
			Triangulos[tri_sel].P2.R = ColorSel.R;
			Triangulos[tri_sel].P2.G = ColorSel.G;
			Triangulos[tri_sel].P2.B = ColorSel.B;
		}
		else if (DP3_XY <= DP1_XY && DP3_XY <= DP2_XY) {
			Triangulos[tri_sel].P3.R = ColorSel.R;
			Triangulos[tri_sel].P3.G = ColorSel.G;
			Triangulos[tri_sel].P3.B = ColorSel.B;
		}
		// Con esta operacion, un triángulo queda seleccionado, DESACTIVAR
		Triangulos[TriSel].Sel = false;
		TriSel = -1;
	}
}

void EliminaTriangulo(int k) {
	// Solo borrar si k >= 0
	if (k >= 0) {
		Triangulos.erase(Triangulos.begin() + k);
		DibujaTriangulos();
	}
}