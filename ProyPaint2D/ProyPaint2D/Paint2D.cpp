#include <iostream>
#include <vector>
#include <math.h>

#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <GL/glui.h>

#include "Triangulo.h"

using namespace std;

int	render_num = 50;
int initial_num = 50;

float xy_aspect;
int last_x, last_y;
float rotationX = 0.0f, rotationY = 0.0f;

int   main_window;
int   current_off;
int   wireframe = 1;
int   light0_enabled = 1;
int   light1_enabled = 1;
float light0_intensity = 1.0;
float light1_intensity = .4;
float scale = 1.0;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

GLUI* glui, * glui2;
GLUI_Spinner* light0_spinner, * light1_spinner;
GLUI_RadioGroup* radio;
GLUI_Panel* obj_panel;

#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260
#define ENABLE_ID            300
#define DISABLE_ID           301
#define SHOW_ID              302
#define HIDE_ID              303

GLfloat light0_ambient[] = { 0.1f, 0.1f, 0.3f, 1.0f };
GLfloat light0_diffuse[] = { .6f, .6f, 1.0f, 1.0f };
GLfloat light0_position[] = { .5f, .5f, 1.0f, 0.0f };

GLfloat light1_ambient[] = { 0.1f, 0.1f, 0.3f, 1.0f };
GLfloat light1_diffuse[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat light1_position[] = { -1.0f, -1.0f, 1.0f, 0.0f };

GLfloat lights_rotation[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

//--------------------------------------------------//
// Inicializa Colores
//--------------------------------------------------//
void inline InicializaColores()
{
	// Color inicial seleccionado
	TColor CInicial;
	CInicial.R = light1_diffuse[0];
	CInicial.G = light1_diffuse[1];
	CInicial.B = light1_diffuse[2];
	ColorSel = CInicial;
}

//--------------------------------------------------//
// Evento Clic Mouse
//--------------------------------------------------//
void ClicMouse(int boton, int estado, int x, int y) {	

	if (cEventoBoton == 'S')
	{
		if (boton == GLUT_LEFT_BUTTON) {
			if (estado == GLUT_DOWN) {
				band = false;
				SeleccionaTriangulo(x, y);
			}
		}
	}

	if (cEventoBoton == 'T')
	{
		if (boton == GLUT_LEFT_BUTTON) {
			if (estado == GLUT_UP) {
				// Activar trazado de lineas, si se suelta el boton 
				// izquierdo del mouse: band <-- true
				// variable utilizada mientras se mueve el mouse sin preionar
				band = false;

				// Si se solto el boton izquierdo del mouse y NroClick == 0 entonces 
				// almacenar el primer vertice del triangulo, e incrementar NroClicks
				if (NroClick == 0) {
					NroClick++;
					P1.x = x; P1.y = ALTO - y; p1b = true;
					band = true;
				}
				// Si se solto el boton izquierdo del mouse y NroClick = 1 entonces 
				// almacemar el segundo vertice del triangulo, e incrementar NroClicks
				else if (NroClick == 1) {
					NroClick++;
					P2.x = x; P2.y = ALTO - y; p2b = true;
					band = true;
				}
				// Si se solto el boton izquierdo del mouse y NroClick = 2 entonces 
				// reincializar NroClicks a cero y las banderas a falso, ya que se 
				// termino de dibujar el triángulo
				// LUEGO GRABAR UN NUEVO TIRNAGULO EN EL VECTOR
				else if (NroClick == 2) {
					// Reiniciarlizar Clicks de mouse y
					// Banderas de trazado de lineas en falso
					NroClick = 0;
					band = false; p1b = false; p2b = false;

					// Crear triangulo con P1, P2 y posición actual del mouse
					Triangulo.P1.x = P1.x; Triangulo.P1.y = P1.y;
					Triangulo.P2.x = P2.x; Triangulo.P2.y = P2.y;
					Triangulo.P3.x = x; Triangulo.P3.y = ALTO - y;

					// Color del triangulo, color seleccionado
					Triangulo.P1.B = ColorSel.B; Triangulo.P1.G = ColorSel.G; Triangulo.P1.R = ColorSel.R;
					Triangulo.P2.B = ColorSel.B; Triangulo.P2.G = ColorSel.G; Triangulo.P2.R = ColorSel.R;
					Triangulo.P3.B = ColorSel.B; Triangulo.P3.G = ColorSel.G; Triangulo.P3.R = ColorSel.R;

					// Calcula centroide
					Triangulo.BC.x = (Triangulo.P1.x + Triangulo.P2.x + Triangulo.P3.x) / 3;
					Triangulo.BC.y = (Triangulo.P1.y + Triangulo.P2.y + Triangulo.P3.y) / 3;

					// Establecer como triangulo no seleccionado
					Triangulo.Sel = false;

					// Agregar al vector
					Triangulos.push_back(Triangulo);
				}
			}
		}
	}

	// MOVER TRIÁNGULO
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN && TriSel >= 0) {
		// Almacena posición actual del mouse, para mover un triangulo
		rx = x;
		ry = y;
	}

	glutPostRedisplay();
}

void control_cb(int control)
{

	if (control == LIGHT0_ENABLED_ID) {
		if (light0_enabled) {
			glEnable(GL_LIGHT0);
			light0_spinner->enable();
		}
		else {
			glDisable(GL_LIGHT0);
			light0_spinner->disable();
		}
	}

	else if (control == LIGHT1_ENABLED_ID) {
		if (light1_enabled) {
			glEnable(GL_LIGHT1);
			light1_spinner->enable();
		}
		else {
			glDisable(GL_LIGHT1);
			light1_spinner->disable();
		}
	}

	else if (control == LIGHT1_INTENSITY_ID) {
		float v[] = {
		  light1_diffuse[0],  light1_diffuse[1],
		  light1_diffuse[2],  light1_diffuse[3] };

		v[0] *= light1_intensity;
		v[1] *= light1_intensity;
		v[2] *= light1_intensity;

		TColor temp;
		temp.R = v[0];
		temp.G = v[1];
		temp.B = v[2];

		ColorSel = temp;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
	}
	else if (control == ENABLE_ID)
	{
		glui2->enable();
	}
	else if (control == DISABLE_ID)
	{
		glui2->disable();
	}
	else if (control == SHOW_ID)
	{
		glui2->show();
	}
	else if (control == HIDE_ID)
	{
		glui2->hide();
	}
}

//--------------------------------------------------//
// Eventos
//--------------------------------------------------//
void EventoSeleccionar()
{
	cEventoBoton = 'S';
}

void EventoBorrar()
{
	EliminaTriangulo(TriSel);
	cEventoBoton = 'S';
	Opcion = 1;
	TriSel = -1;
}

void EventoDibujarTriangulo()
{
	cEventoBoton = 'T';
}

void EventoDibujarCuadrado()
{
	cEventoBoton = 'C';
}

void EventoSetColor()
{

}

void EventoEscalarTriangulo()
{
	EscalaTriangulo(TriSel, 1.0 + (scale/100));	
}

void EventoRotarTrianguloHorario()
{
	RotaTriangulo(angulo, TriSel, -1);	
}

void EventoRotarTrianguloAntiHorario()
{
	RotaTriangulo(angulo, TriSel, 1);	
}

void EventoTranslacionTrianguloEjePosX()
{
	TrasladaTriangulo(TriSel, (avance/2), 0);	
}

void EventoTranslacionTrianguloEjePosY()
{
	TrasladaTriangulo(TriSel, 0, +(avance/2));
}

void EventoTranslacionTrianguloEjeNegX()
{
	TrasladaTriangulo(TriSel, -(avance / 2), 0);
}

void EventoTranslacionTrianguloEjeNegY()
{
	TrasladaTriangulo(TriSel, 0, -(avance / 2));
}

void EventoTranslacionTrianguloEjeXY()
{	
	TrasladaTriangulo(TriSel, (avance / 2), (avance / 2));	
}

//--------------------------------------------------//
// Inicializa Glut 
//--------------------------------------------------//
void InicializaGlut() {

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ANCHO, 0, ALTO);
}

//--------------------------------------------------//
// Main => Funcion principal
//--------------------------------------------------//
int main(int argc, char** argv) {	
	InicializaColores();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(ANCHO, ALTO);
	glutInitWindowPosition(0, 0);

	main_window = glutCreateWindow(" ==================================== PAINT EDITOR 2D BY CRISTHIAN ALVARO ==================================== ");
		
	GLUI_Master.set_glutSpecialFunc(NULL);
	
	//--------------------------------------------------//
	// GLUI para controles de formularios
	//--------------------------------------------------//

	printf("GLUI version: %3.2f\n", GLUI_Master.get_version());

	/*** Create the side subwindow ***/
	glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

	obj_panel = new GLUI_Rollout(glui, "Propiedades", true);
		

	GLUI_Spinner* scale_spinner =
	scale_spinner = new GLUI_Spinner(obj_panel, "Escala:", &scale, 1, (GLUI_Update_CB) EventoEscalarTriangulo);
	
	scale_spinner->set_float_limits(0.0f, 1.0f);	
	scale_spinner->set_alignment(GLUI_ALIGN_RIGHT);	
	

	GLUI_Rollout* roll_lights = new GLUI_Rollout(glui, "Luces", true);

	GLUI_Panel* light1 = new GLUI_Panel(roll_lights, "Color RGB");
		
	GLUI_Scrollbar* sb;	

	new GLUI_Checkbox(light1, "Enabled", &light1_enabled,
		LIGHT1_ENABLED_ID, control_cb);
	light1_spinner =
		new GLUI_Spinner(light1, "Intencidad:",
			&light1_intensity, LIGHT1_INTENSITY_ID,
			control_cb);

	light1_spinner->set_float_limits(0.0, 1.0);
	sb = new GLUI_Scrollbar(light1, "Red", GLUI_SCROLL_HORIZONTAL,
		&light1_diffuse[0], LIGHT1_INTENSITY_ID, control_cb);

	sb->set_float_limits(0, 1);
	sb = new GLUI_Scrollbar(light1, "Green", GLUI_SCROLL_HORIZONTAL,
		&light1_diffuse[1], LIGHT1_INTENSITY_ID, control_cb);

	sb->set_float_limits(0, 1);
	sb = new GLUI_Scrollbar(light1, "Blue", GLUI_SCROLL_HORIZONTAL,
		&light1_diffuse[2], LIGHT1_INTENSITY_ID, control_cb);

	sb->set_float_limits(0, 1);

	new GLUI_StaticText(glui, "");
	new GLUI_StaticText(glui, "");
	
	new GLUI_Button(glui, "            Salir            ", 0, (GLUI_Update_CB)exit);
		
	glui->set_main_gfx_window(main_window);

	glui2 = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_BOTTOM);
	glui2->set_main_gfx_window(main_window);

	new GLUI_Button(glui2, "Seleccionar", 0, (GLUI_Update_CB) EventoSeleccionar);	

	new GLUI_Button(glui2, "Borrar", 0, (GLUI_Update_CB) EventoBorrar);

	new GLUI_Column(glui2, false);

	new GLUI_Button(glui2, " Triangulo ", 0, (GLUI_Update_CB) EventoDibujarTriangulo);	

	new GLUI_Button(glui2, " Cuadrado ", 0, (GLUI_Update_CB) EventoDibujarCuadrado);

	new GLUI_Column(glui2, false);	
	
	GLUI_Rotation* view_rot_horario = new GLUI_Rotation(glui2, "Horario", view_rotate);
	view_rot_horario->callback = (GLUI_Update_CB)EventoRotarTrianguloHorario;
	view_rot_horario->set_spin(1.0);

	new GLUI_Column(glui2, false);

	GLUI_Rotation* view_rot_anti_horario = new GLUI_Rotation(glui2, "Anti Horario", view_rotate);
	view_rot_anti_horario->callback = (GLUI_Update_CB)EventoRotarTrianguloAntiHorario;
	view_rot_anti_horario->set_spin(1.0);

	new GLUI_Column(glui2, false);

	GLUI_Translation* trans_xy = new GLUI_Translation(glui2, "Transl XY", GLUI_TRANSLATION_XY, obj_pos);
	trans_xy->callback = (GLUI_Update_CB)EventoTranslacionTrianguloEjeXY;
	trans_xy->set_speed(.005);	

	new GLUI_Column(glui2, false);

	GLUI_Translation* trans_xPos =	new GLUI_Translation(glui2, "Transl +X", GLUI_TRANSLATION_X, &obj_pos[0]);
	trans_xPos->callback = (GLUI_Update_CB)EventoTranslacionTrianguloEjePosX;
	trans_xPos->set_speed(.005);

	new GLUI_Column(glui2, false);

	GLUI_Translation* trans_yPos =	new GLUI_Translation(glui2, "Transl +Y", GLUI_TRANSLATION_Y, &obj_pos[1]);
	trans_yPos->callback = (GLUI_Update_CB)EventoTranslacionTrianguloEjePosY;
	trans_yPos->set_speed(.005);

	new GLUI_Column(glui2, false);

	GLUI_Translation* trans_xNeg = new GLUI_Translation(glui2, "Transl -X", GLUI_TRANSLATION_X, &obj_pos[0]);
	trans_xNeg->callback = (GLUI_Update_CB)EventoTranslacionTrianguloEjeNegX;
	trans_xNeg->set_speed(.005);

	new GLUI_Column(glui2, false);

	GLUI_Translation* trans_yNeg = new GLUI_Translation(glui2, "Transl -Y", GLUI_TRANSLATION_Y, &obj_pos[1]);
	trans_yNeg->callback = (GLUI_Update_CB)EventoTranslacionTrianguloEjeNegY;
	trans_yNeg->set_speed(.005);

#if 0
	/**** We register the idle callback with GLUI, *not* with GLUT ****/
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
#endif	
	
	InicializaGlut();
	glutMouseFunc(ClicMouse);
	glutPassiveMotionFunc(MoverMouse);
	glutMotionFunc(ArrastrarMouse);	
	glutDisplayFunc(DibujaTriangulos);

	glutMainLoop();

	return 0;
}