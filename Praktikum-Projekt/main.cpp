#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Fürs Shader-Organisieren:
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Hauptversion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //Nebenversion
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Praktikum-Projekt", NULL, NULL); //Fenster wird erstellt

	if (window == NULL)
	{
		std::cout << "Es gab einen Fehler bei der Erschaffung des Fensters" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //Damit GLFW weiß, welches Fenster es benutzen soll

	gladLoadGL(); //Lädt OpenGL Funktionen

	glViewport(0, 0, 1280, 720); //Größe des Erstellten Bildes von OpenGL, am besten nimmt man die Auflösung des Fensters(muss noch flexibel gemacht werden)
	
	

	GLfloat vertices[] = //Koordinaten von einem Dreieck
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f
	};

	GLuint indices[] =
	{
		0, 3, 5,
		3, 2, 4,
		5, 4, 1
	};
	//Shader-Programm erstellen; siehe shaderClass.h/.cpp
	Shader shaderProgram("default.vert", "default.frag");
	//VAO erstellen und binden
	VAO VAO1;
	VAO1.Bind();
	//VBO und EBO erstellen
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	//VAO zum VBO linken(verbinden) und die Objekte wieder entlinken, damit man sich nicht wieder auf sie bezieht
	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	glfwSwapBuffers(window);
	//Haupt-Loop
	while (!glfwWindowShouldClose(window)) //Fenster muss offen bleiben, solange es nicht manuell geschlossen wird
	{
		glClearColor(0.1f, 0.16f, 0.16f, 1.0f); //Färbt den Bereich in dieser Farbe
		glClear(GL_COLOR_BUFFER_BIT); //Erstellen vom Buffer
		shaderProgram.Activate(); //Dieses Shader-Prgramm wird jetzt genutzt
		VAO1.Bind(); //Binden, damit OpenGL weiß, welches es verwenden muss
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window); //Letzten Frame(schwarz, auch Buffer) durch neuen Frame(Buffer) ersetzen
		glfwPollEvents(); //Damit das Fenster auf Eingaben reagiert
	}

	//Löschung der erstellten Elemente nach dem Schließen
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}