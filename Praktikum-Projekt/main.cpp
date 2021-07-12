#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Fürs Shader-Organisieren:
#include "shader.h"
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
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, 
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, 
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, 
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, 

		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, 
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, 
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, 

		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, 
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, 
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, 

		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, 
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, 
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, 

		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, 
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, 
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  
	};

	GLuint indices[] =
	{
		0, 1, 2, 
		0, 2, 3,
		4, 6, 5, 
		7, 9, 8, 
		10, 12, 11, 
		13, 15, 14
	};

	GLfloat lightVertices[] =
	{
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f,
	};

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};
	//Shader-Programm erstellen; siehe shaderClass.h/.cpp
	Shader shaderProgram("main.vert", "main.frag");
	//VAO erstellen und binden
	VAO VAO1;
	VAO1.Bind();
	//VBO und EBO erstellen
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	//VAO zum VBO linken(verbinden) und die Objekte wieder entlinken, damit man sich nicht wieder auf sie bezieht
	VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttribute(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttribute(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttribute(lightVBO, 0, 3, GL_FLOAT, 3* sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
	lightModelMatrix = glm::translate(lightModelMatrix, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModelMatrix = glm::mat4(1.0f);
	pyramidModelMatrix = glm::translate(pyramidModelMatrix, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(lightModelMatrix));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(pyramidModelMatrix));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	
	//Textur
	Texture tiles("Tiles084_1K_Color.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	tiles.texUnit(shaderProgram, "tex0", 0);
	Texture tilesSpecular("Tiles084_1K_AmbientOcclusion.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	tilesSpecular.texUnit(shaderProgram, "tex1", 1);

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

		camera.updateMatrix(50.0f, 0.01f, 100.0f);
		shaderProgram.Activate(); //Dieses Shader-Prgramm wird jetzt genutzt
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");

		tiles.Bind(); // Textur wird gelinkt
		tilesSpecular.Bind();
		VAO1.Bind(); //Binden, damit OpenGL weiß, welches es verwenden muss
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(float), GL_UNSIGNED_INT, 0);
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