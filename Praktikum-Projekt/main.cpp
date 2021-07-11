#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Fürs Shader-Organisieren:
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "camera.h"

//Texturen
#include "texture.h"

//Variablen
const unsigned int width = 1024;
const unsigned int height = 1024;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Hauptversion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //Nebenversion
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Praktikum-Projekt", NULL, NULL); //Fenster wird erstellt

	if (window == NULL)
	{
		std::cout << "Es gab einen Fehler bei der Erschaffung des Fensters" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //Damit GLFW weiß, welches Fenster es benutzen soll

	gladLoadGL(); //Lädt OpenGL Funktionen

	glViewport(0, 0, width, height); //Größe des Erstellten Bildes von OpenGL, am besten nimmt man die Auflösung des Fensters(muss noch flexibel gemacht werden)
	
	

	GLfloat vertices[] = //Koordinaten von einem Dreieck
	{							//---Farben---//	   //-Mapping-//
	    -0.5f, 0.0f,  0.5f,	 0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,	 0.83f, 0.70f, 0.44f,   5.0f, 0.0f, //Wenn höher als 1, dann wird die Textur wiederholt//
		 0.5f, 0.0f, -0.5f,	 0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,	 0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,	 0.92f, 0.86f, 0.76f,   2.5f, 5.0f,
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};
	//Shader-Programm erstellen; siehe shaderClass.h/.cpp
	Shader shaderProgram("vertexShader.vert", "fragmentShader.frag");
	//VAO erstellen und binden
	VAO VAO1;
	VAO1.Bind();
	//VBO und EBO erstellen
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	//VAO zum VBO linken(verbinden) und die Objekte wieder entlinken, damit man sich nicht wieder auf sie bezieht
	VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttribute(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	
	//Textur
	Texture tiles("Tiles084_1K_Color.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	tiles.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	glfwSwapInterval(0);

	float prevTime = 0.0;
	float crntTime = 0.0;
	float deltaTime;
	unsigned int counter = 0;

	//Haupt-Loop
	while (!glfwWindowShouldClose(window)) //Fenster muss offen bleiben, solange es nicht manuell geschlossen wird
	{
		crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;
		counter++;
		if (deltaTime >= 1.0 / 60.0)
		{
			std::string fps = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) * 1000);
			std::string newTitle = "Praktikum-Projekt --- " + fps + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
			camera.Inputs(window, deltaTime);
		}
		
		glClearColor(0.1f, 0.16f, 0.16f, 1.0f); //Färbt den Bereich in dieser Farbe
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Erstellen vom Buffer
		shaderProgram.Activate(); //Dieses Shader-Prgramm wird jetzt genutzt

		
		camera.Matrix(50.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		tiles.Bind(); // Textur wird gelinkt
		VAO1.Bind(); //Binden, damit OpenGL weiß, welches es verwenden muss
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window); //Letzten Frame(schwarz, auch Buffer) durch neuen Frame(Buffer) ersetzen
		glfwPollEvents(); //Damit das Fenster auf Eingaben reagiert
	}

	//Löschung der erstellten Elemente nach dem Schließen
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	tiles.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}