#version 330 core
//#extension GL_NV_shader_buffer_load: enable			//usar punteros en los shaders
const int MAX_LUCES = 5;

out vec4 outColor;

in vec3 color;
in vec3 vPos;
in vec3 norm;
in vec2 texCoord;
in vec3 tangent;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D ambOcluTex;
uniform sampler2D specTex;
uniform sampler2D normalTex;

uniform mat4 view;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 50.0;
vec3 Ke;

//Propiedades de la luz
vec3 att = vec3(1.0, 0.1, 0.01);
vec3 Ia = vec3(0.2);	

//estructura de los parámetros de cada luz
struct lightParam{
	float spotN;				//exponente de luz focal
	float spotAngle;			//angulo de luz focal
	vec3 Id;					//Intensidad difusa
	vec3 Is;					//Intensidad especular	
	vec3 D;						//Direccion del foco	
	vec3 colD;					//color difuso	
	vec3 colS;					//color especular	
	vec4 pos;					//posicion de la luz, coordenada homogenea a 0 para luz direccional	
};

uniform lightParam lightSource[MAX_LUCES];				//variable uniform donde se cargan los datos de las luces

vec3 shade();

void main()
{
	Kd = texture(colorTex, texCoord).rgb;
	Ka = Kd * texture(ambOcluTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;
	Ks = texture(specTex,texCoord).rgb + vec3(0.1);
	//Ks = vec3(1.0);


	vec3 auxN = normalize(norm);									//Normales ya interpoladas y normalizadas
	vec3 auxT = normalize(tangent);									//Tangentes 
	vec3 bitangent = cross(auxN, auxT);								//Bitangentes, producto vectorial de N x T para crear una matriz normalizada
	mat3 TBNMatrix = mat3(auxT,bitangent,auxN);						//matriz para cambiar al sistema de coordenadas del TangentSpace

	N = TBNMatrix*(2.0*texture(normalTex,texCoord).rgb - vec3(1.0));		//normales dadas por un normal map, multiplicadas por la matriz anterior.	
	//N = normalize (norm);
	
	
	outColor = vec4(shade(), 1.0);   
	//outColor = vec4(auxT, 1.0);   
}


vec3 shade()    //sin lightsource
{
	vec3 c = vec3(0.0);
	c = Ia * Ka;

	for(int i = 0; i < MAX_LUCES; i++){
	
	vec4 pos = lightSource[i].pos;					//posicion de la luz, coordenada homogenea a 0 para luz direccional	

	vec3 L = normalize(vec3(view*lightSource[i].pos) - vPos * lightSource[i].pos.w);									//vector que va del punto al foco de luz, se multiplica por w de la pos del foco, si es direccional se anula el vertice		
		vec3 D2 = normalize(vec3(view*vec4(lightSource[i].D,0.0)));										//vector de dirección del foco
		float cosLD = dot(-L,D2);															//cos del angulo entre la dir. del foco y el vector luz-posicion
			
		vec3 V = normalize(-vPos);															//vector que va del punto a la camara
		vec3 R = normalize(reflect(-L, N));													//vector del rayo de luz reflejado			
		
		//factores de atenuacion(distancia y focal)
		float fatt = 1.0;
		fatt = min((1-lightSource[i].pos.w) +1/(att.x + att.y * distance(vec3(view*lightSource[i].pos),vPos) + att.z * pow(distance(vec3(view*lightSource[i].pos),vPos),2)),1);     //funcion de atenuacion con la distancia
		float spotAtt = 1.0;
		if(lightSource[i].spotAngle != 180)
			if(cosLD > cos(radians(lightSource[i].spotAngle)))
				spotAtt = pow((cosLD - cos(radians(lightSource[i].spotAngle)))/(1-cos(radians(lightSource[i].spotAngle))),lightSource[i].spotN);         //funcion de atenuacion focal
			else
				spotAtt = 0.0;		
				
		//luz difusa			
		vec3 Diff = lightSource[i].colD * Kd * max(dot(N,L),0.0);				

		//luz especular		
		float factor = clamp(dot(V,R),0.0001,1.0);				//se hace un clamp porque la funcion pow da problemas con el 0 exacto y valores negativos
		factor = pow (factor,alpha);
		vec3 Spec = lightSource[i].colS * Ks * factor;
		Spec = (dot(N,L)>0)? Spec * 1.0 : Spec * 0.0;

		c += fatt * spotAtt * (lightSource[i].Id * Diff + lightSource[i].Is * Spec);	
	}
	//color emisivo
	c += Ke;

	return clamp(c,0.0,1.0);
}
