#define GL_GLEXT_PROTOTYPES

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "render.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"


const int height = 5;

GLuint vbo[2*(height + 1)];		//identyfikatory buforow wierzcholkow
GLuint vao[height + 1];		//identyfikatory tablic wierzcholkow
GLuint ebo;		//identyfikator bufora elementow

GLuint shaderProgram;
GLint vertexShader;	//identyfikator shadera wierzcholkow
GLint fragmentShader;   //identyfikator shadera fragmentow
GLint posAttrib, colAttrib;	//

glm::mat4 viewMatrix = glm::mat4();
glm::mat4 projectionMatrix = glm::mat4(); //marzerz widoku i rzutowania
GLfloat fi = 0;


//-------------Atrybuty wierzcholkow------------------------------------------
const int vert_ct = 62;
float alfa = 2*M_PI/(vert_ct - 2);

	GLfloat ver_triangle[3* vert_ct];
	GLfloat col_triangle[3* vert_ct];
	
	void stozek(int dy, int scale) {
	for(int i=0; i<3; i++) {
		if (i != 1)
		{
			ver_triangle[i] = 0.0f;

		}
		else
		{
			ver_triangle[i] = (0.0f + dy) * scale;		
		}
		col_triangle[i] = 1.0f;
	}

	for(int i=1; i<vert_ct; i++) {
	
		ver_triangle[3*i] = cos (i*alfa);
		ver_triangle[3*i+1] = (-1.0f + dy) * scale;
		ver_triangle[3*i+2] = sin (i*alfa);
 
		col_triangle[3*i] = 1.0f * (i % 2);
		col_triangle[3*i+1] = 1.0f * (i % 2);
		col_triangle[3*i+2] = 1.0f * (i % 2);
 
	}
}

	GLfloat ver_rectangle[] = {	//wspolrzedne wierzcholkow prostokata
		  0.0f, 0.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,
		  1.0f, 0.0f, 0.0f,
		  1.0f, 1.0f, 0.0f,
	 
		  0.0f, 0.0f, 1.0f,
		  0.0f, 1.0f, 1.0f,
		  1.0f, 0.0f, 1.0f,
		  1.0f, 1.0f, 1.0f
	};

	GLfloat col_rectangle[] = {	//kolory wierzcholkow prostokata
		0.0f, 0.0f, 0.0f,	//0
		1.0f, 1.0f, 0.0f,	//1
		0.0f, 1.0f, 0.0f,	//2
		1.0f, 1.0f, 0.0f,	//3
		
		0.0f, 0.0f, 1.0f,	//4
		1.0f, 1.0f, 1.0f,	//5
		0.0f, 1.0f, 1.0f,	//6
		1.0f, 1.0f, 1.0f	//7
	};

	int n = 3;
	GLuint elements[] = { //prostokat skladamy z dwoch trojkatow
		0, 1, 2,		  //indeksy wierzcholkow dla pierwszego trojkata
		1, 2, 3,
		
		4, 5, 6,
		5, 6, 7,
		
		2, 3, 6,
		3, 6, 7,
		
		0, 1, 4,
		1, 4, 5,
		
		0, 2, 4,
		2, 4, 6,
		
		1, 3, 5,
		3, 5, 7
		
					  //indeksy wierzcholkow dla drugiego trojkata
	};
	

//----------------------------kod shadera wierzcholkow-----------------------------------------

const GLchar* vShader_string =
{
  "#version 130\n"\

  "in vec3 position;\n"\
  "in vec3 color;\n"\
  "out vec3 Color;\n"\
  "uniform mat4 transformMatrix;\n"\
  "void main(void)\n"\
  "{\n"\
  "  gl_Position = transformMatrix * vec4(position, 1.0);\n"\
  "  Color = color;\n"\
  "}\n"
};

//----------------------------kod shadera fragmentow-------------------------------------------
const GLchar* fShader_string =
{
  "#version 130\n"\
  "in  vec3 Color;\n"\
  "out vec4 outColor;\n"\

  "void main(void)\n"\
  "{\n"\
  "  outColor = vec4(Color, 1.0);\n"\
  "}\n"
};


//------------------------------------------------zmiana rozmiaru okna---------------------------

void resizeGLScene(unsigned int width, unsigned int height)
{
    if (height == 0) height = 1; // zabezpiecznie dla okna o zerowej wysokosci
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}


//----------------------------------tworzenie, wczytanie, kompilacja shaderow-------------------------

int initShaders(void)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShader_string, NULL);
    glCompileShader(vertexShader);
    
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
      std::cout << "Kompilacja shadera wierzcholkow powiodla sie!\n";
    else
    {
      std::cout << "Kompilacja shadera wierzcholkow NIE powiodla sie!\n";
      return 0;
     }
     
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader_string, NULL); 
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
      std::cout << "Kompilacja shadera fragmentow powiodla sie!\n";
    else
    {
      std::cout << "Kompilacja shadera fragmentow NIE powiodla sie!\n";
      return 0;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    //glBindFragDataLocation(shaderProgram, 0, "outColor"); 

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    return 1;
}



//--------------------------------------------funkcja inicjujaca-------------------------------------
int initGL(void)
{
   
    if(initShaders())
    {   
    	stozek(0, 1);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 
	glGenVertexArrays(height + 1, vao); //przypisanie do vao identyfikatorow tablic
	glGenBuffers(2*(height + 1), vbo);	   //przypisanie do vbo identyfikatorow buforow
	glGenBuffers(1, &ebo);

	posAttrib = glGetAttribLocation(shaderProgram, "position"); //pobranie indeksu tablicy atrybutow wierzcholkow okreslajacych polozenie
        glEnableVertexAttribArray(posAttrib);
	colAttrib = glGetAttribLocation(shaderProgram, "color");    //pobranie indeksu tablicy atrybutow wierzcholkow okreslajacych kolor
        glEnableVertexAttribArray(colAttrib);
	

    for (int i = 0; i < height; i++)
    {
        glBindVertexArray(vao[i]);					//wybor tablicy

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2 * i]); 							//powiazanie bufora z odpowiednim obiektem (wybor bufora) 
        glBufferData(GL_ARRAY_BUFFER, sizeof(ver_triangle), ver_triangle, GL_STATIC_DRAW); 	//skopiowanie danych do pamieci aktywnego bufora
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);				//okreslenie organizacji danych w tablicy wierzcholkow
	    glEnableVertexAttribArray(posAttrib);							//wlaczenie tablicy

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2*i+1]);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(col_triangle), col_triangle, GL_STATIC_DRAW);
	    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(colAttrib);
    }

	
	glBindVertexArray(vao[height]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2*height]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ver_rectangle), ver_rectangle, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2*height+1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col_rectangle), col_rectangle, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        
	//macierz widoku (okresla polozenie kamery i kierunek w ktorym jest skierowana) 
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
	//macierz rzutowania perspektywicznego
	projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 10.0f);		

 	return 1;
    }
    else
	return 0;
}

//------------------------------------------renderowanie sceny-------------------------------------

int drawGLScene(int counter)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    int dir[2] = { -1, 1 };

    for (int i = 0; i < height; i++)
    {
        glm::mat4 scaledMatrix = glm::scale(glm::mat4(), glm::vec3(0.66f/((0.25f*i)+0.5f),0.66f/((0.25f*i)+0.5f),0.66f/((0.25f*i)+0.5f)));
        glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 1+0.15f*i, 0.0f)); 
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::radians(fi * dir[i%2]), glm::vec3(0.0f, 1.0f, 0.0f));
            
        glm::mat4 transformMatrix = projectionMatrix * viewMatrix * translationMatrix * rotationMatrix * scaledMatrix;


        GLint transformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "transformMatrix");
        glUniformMatrix4fv(transformMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        

        glBindVertexArray(vao[i]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vert_ct);
    }



   
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3*vert_ct); //rysowanie trojkata

    glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-0.5f, -1.25f, 0.0f));  		//macierz przesuniecia o zadany wektor
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //macierz obrotu o dany kat wokol wektora

    glm::mat4 transformMatrix = projectionMatrix * viewMatrix * translationMatrix * rotationMatrix;

    GLint transformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "transformMatrix");  //pobranie polozenia macierzy bedacej zmienna jednorodna shadera
    glUniformMatrix4fv(transformMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix)); //zapisanie macierzy bedacej zmienna jednorodna shadera wierzcholkow

    glBindVertexArray(vao[height]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //rysowanie prostokata

    fi += 0.5;
 
    glFlush();

    return 1;    
}

//----------------------------------------------------porzadki--------------------------------------

void deleteAll()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(4, vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(2, vao);
}
