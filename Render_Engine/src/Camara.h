#ifndef _CAMARA_H
#define _CAMARA_H

class Camara
{
	glm::vec3 cPos;
	glm::vec3 cView;
	glm::vec3 cUp;
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);	
	bool free = true;

public:
	Camara(){};
	~Camara(){};
	inline glm::vec3 getPos(){ return cPos; }
	inline glm::mat4 getProj(){ return proj; }
	glm::mat4 getView();
	inline glm::vec3 getPView(){ return cView; }
	inline void setFree(bool f){ free = f; }
	inline void setProj(glm::mat4 p){ proj = p; }
	void Strafe_Camera(float speed);
	void Mouse_Move(int wndWidth, int wndHeight);
	void Move_Camera(float speed);
	void Move_Up_Camera(float speed);
	void Rotate_View(float speedX,float speedY);	
	void Position_Camera(float pos_x, float pos_y, float pos_z,
		float view_x, float view_y, float view_z,
		float up_x, float up_y, float up_z);
};

//Inicializa la camara con la posicion (punto), el punto al que mira (punto) y el vector up (vector)
void Camara::Position_Camera(float pos_x, float pos_y, float pos_z,
	float view_x, float view_y, float view_z,
	float up_x, float up_y, float up_z)
{
	cPos = glm::vec3(pos_x, pos_y, pos_z); // posicion
	cView = glm::vec3(view_x, view_y, view_z); // view
	cUp = glm::vec3(up_x, up_y, up_z); // vector up	
}

//construye la matriz view con los datos actuales de la camara y la devuelve
glm::mat4 Camara::getView(){ view = glm::lookAt(cPos, cView, cUp); return view; }	


/////////////////////////////////////////////////////////////////////////////////////////////////
//								Movimiento de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Move_Camera(float speed)
{
	glm::vec3 vVector = cView - cPos;	//vector lookAt
	vVector = glm::normalize(vVector);

	// velocidad positiva para avanzar y negativa para retroceder
	cPos.x += vVector.x * speed;
	cPos.y += vVector.y * speed;
	cPos.z += vVector.z * speed;
	if (free){			
		cView.x += vVector.x * speed;
		cView.y += vVector.y * speed;
		cView.z += vVector.z * speed;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//										Desplazamiento lateral de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Strafe_Camera(float speed)
{
	glm::vec3 vVector = cView - cPos;	// vector view
	glm::vec3 vOrthoVector;             // vector ortogonal a view

	//vOrthoVector.x = -vVector.z;
	//vOrthoVector.z = vVector.x;
	vOrthoVector = glm::cross(vVector, cUp);
	vOrthoVector = glm::normalize(vOrthoVector);


	// velocidad positiva para derecha y negativa para izquierda
	if (free){
		cPos.x += vOrthoVector.x * speed;
		cPos.y += vOrthoVector.y * speed;
		cPos.z += vOrthoVector.z * speed;	
		cView.x += vOrthoVector.x * speed;
		cView.y += vOrthoVector.y * speed;
		cView.z += vOrthoVector.z * speed;
	}
	else{
		Rotate_View(speed*0.5, 0.0);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//										Desplazamiento vertical de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Move_Up_Camera(float speed)
{
	// velocidad positiva para arriba y negativa para abajo		
	if (free){
		cPos += cUp * speed;
		cView += cUp * speed;
	}
	else{
		Rotate_View(0.0, speed * 0.5);
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////
//								Rotacion de view
/////////////////////////////////////////////////////////////////////////////////////////////////
void Camara::Rotate_View(float speedX, float speedY)
{
	glm::vec3 vVector =  cView - cPos;	// vector view (lookAt)
	//vVector = glm::normalize(vVector);

	if (free){
		cView.y += speedY;
		cView.z = (float)(cPos.z + sin(speedX)*vVector.x + cos(speedX)*vVector.z);
		cView.x = (float)(cPos.x + cos(speedX)*vVector.x - sin(speedX)*vVector.z);
	}
	else{
		glm::vec3 vVector =  cPos - cView;
		//cPos.y += speedY;		
		cPos.y = (float)(cView.y + sin(speedY)*vVector.z + cos(speedY)*vVector.y);
		cPos.z = (float)(cView.z + sin(speedX)*vVector.x + cos(speedX)*vVector.z);
		cPos.x = (float)(cView.x + cos(speedX)*vVector.x - sin(speedX)*vVector.z);
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										Movimiento con raton de la camara
/////////////////////////////////////////////////////////////////////////////////////////////////
//se llama en la funcion idle pasandole las medidas de la pantalla
void Camara::Mouse_Move(int wndWidth, int wndHeight)
{
	POINT mousePos;
	int offsetX = glutGet((GLenum)GLUT_WINDOW_X);
	int offsetY = glutGet((GLenum)GLUT_WINDOW_Y);
	int mid_x = wndWidth >> 1;
	mid_x += offsetX;
	int mid_y = wndHeight >> 1;
	mid_y += offsetY;
	float angle_y = 0.0f;
	float angle_z = 0.0f;
	float factorX = -1.0;
	float factorY = 3.0;

	GetCursorPos(&mousePos);	//posicion 2D del raton (x,y)

	/*std::cout << "WindowX: " << offsetX << " windowY: " << offsetY << std::endl;
	std::cout << "midX: " << mid_x << " midY: " << mid_y << std::endl;
	std::cout << "mouseX: " << mousePos.x << " mouseY: " << mousePos.y << std::endl << std::endl;*/

	if ((mousePos.x == mid_x) && (mousePos.y == mid_y)) return;

	SetCursorPos(mid_x, mid_y);	// situa el raton en el centro de la ventana

	// obtenemos la direccion del cursor.
	angle_y = (float)((mid_x - mousePos.x)) / 1000;
	angle_z = (float)((mid_y - mousePos.y)) / 1000;

	// Aumentando el valor aumenta la velocidad de giro de la camara
	//cView.y += angle_z * 3;

	// limitamos el movimiento en el eje y
	if (free){
		if ((cView.y - cPos.y) > 8)  cView.y = cPos.y + 8;
		if ((cView.y - cPos.y) < -8)  cView.y = cPos.y - 8;
	}

	Rotate_View(angle_y * factorX, angle_z * factorY); // Rotacion
}

#endif