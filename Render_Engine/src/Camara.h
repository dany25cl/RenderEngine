#ifndef _CAMARA_H
#define _CAMARA_H

class Camara
{
public:

	glm::vec3 cPos;
	glm::vec3 cView;
	glm::vec3 cUp;	

	void Strafe_Camera(float speed);
	void Mouse_Move(int wndWidth, int wndHeight);
	void Move_Camera(float speed);
	void Move_Up_Camera(float speed);
	void Rotate_View(float speed);
	void Position_Camera(float pos_x, float pos_y, float pos_z,
		float view_x, float view_y, float view_z,
		float up_x, float up_y, float up_z);
};

void Camara::Position_Camera(float pos_x, float pos_y, float pos_z,
	float view_x, float view_y, float view_z,
	float up_x, float up_y, float up_z)
{
	cPos = glm::vec3(pos_x, pos_y, pos_z); // posicion
	cView = glm::vec3(view_x, view_y, view_z); // view
	cUp = glm::vec3(up_x, up_y, up_z); // vector up	
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//								Movimiento de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Move_Camera(float speed)
{
	glm::vec3 vVector = cView - cPos;	//vector view

	// velocidad positiva para avanzar y negativa para retroceder
	cPos.x += vVector.x * speed;
	cPos.y += vVector.y * speed;
	cPos.z +=  vVector.z * speed;
	cView.x +=  vVector.x * speed;
	cView.y += vVector.y * speed;
	cView.z +=  vVector.z * speed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//										Desplazamiento vertical de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Move_Up_Camera(float speed)
{
	// velocidad positiva para arriba y negativa para abajo	
	cPos.y += 5 * speed;	
	cView.y += 5 * speed;
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//								Rotacion de view
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Rotate_View(float speed)
{
	glm::vec3 vVector = cView - cPos;	// Get the view vector

	cView.z = (float)(cPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	cView.x = (float)(cPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										Desplazamiento lateral de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Strafe_Camera(float speed)
{
	glm::vec3 vVector = cView - cPos;	// vector view
	glm::vec3 vOrthoVector;              // vector ortogonal a view

	vOrthoVector.x = -vVector.z;
	vOrthoVector.z = vVector.x;

	// velocidad positiva para derecha y negativa para izquierda
	cPos.x += vOrthoVector.x * speed;
	cPos.y += vOrthoVector.y * speed;
	cPos.z += vOrthoVector.z * speed;
	cView.x += vOrthoVector.x * speed;
	cView.y += vOrthoVector.y * speed;
	cView.z += vOrthoVector.z * speed;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										Movimiento con raton de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Mouse_Move(int wndWidth, int wndHeight)
{
	POINT mousePos;
	int mid_x = wndWidth >> 1;
	int mid_y = wndHeight >> 1;
	float angle_y = 0.0f;
	float angle_z = 0.0f;

	GetCursorPos(&mousePos);	//posicion 2D del raton (x,y)					

	if ((mousePos.x == mid_x) && (mousePos.y == mid_y)) return;

	SetCursorPos(mid_x, mid_y);	// situa el raton en el centro de la ventana

	// obtenemos la direccion del cursor.
	angle_y = (float)((mid_x - mousePos.x)) / 1000;
	angle_z = (float)((mid_y - mousePos.y)) / 1000;

	// Aumentando el valor aumenta la velocidad de giro de la camara
	cView.y += angle_z * 3;

	// limit the rotation around the x-axis
	if ((cView.y - cPos.y) > 8)  cView.y = cPos.y + 8;
	if ((cView.y - cPos.y) <-8)  cView.y = cPos.y - 8;

	Rotate_View(-angle_y); // Rotacion
}

#endif