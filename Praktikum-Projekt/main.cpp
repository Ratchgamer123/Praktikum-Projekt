#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
" fragColor = vec4(0.0f, 1f, 0.0f, 1.0);\n"
"}\0";


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Hauptversion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //Nebenversion
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] = //Koordinaten von einem Dreieck
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

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
	
	//Kompilation des Vertex-Shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Kompilation des Fragment-Shaders
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Beide Shader müssen jetzt an das eigentliche Shader-Programm angehängt werden
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Shader sind schon im Programm, daher kann man sie löschen
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//Ein Vertex Buffer Object, welches verwendet wird, um die Koordinaten zur GPU zu senden//könne mehrere sein
	GLuint VAO, VBO;
	//Erstellung des VAOs, welches als pointer und Speichermodul der Typen von dem Koordinaten-Array ist
	glGenVertexArrays(1, &VAO);
	//Erstellung des VO Buffers als Typ
	glGenBuffers(1, &VBO);
	//Das VAO binden, damit man sich auf dieses bezieht
	glBindVertexArray(VAO);
	//Das Buffer(VBO) binden, damit man sich auf dieses bezieht
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Hier werden die Daten des Arrays zu einem Buffer zugehörigem Typs erstellt
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//Zeigt zum index 0 im Shader, wo der Input für die floats im Shader ist und aktiviert den VAO(s) an der Stelle
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Ablösen der Buffer, um eine Wiederverwendung zu verhindern
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.1f, 0.16f, 0.16f, 1.0f); //Färbt den Bereich in dieser Farbe
	glClear(GL_COLOR_BUFFER_BIT); //Erstellen vom Buffer
	glfwSwapBuffers(window); //Letzten Frame(schwarz, auch Buffer) durch neuen Frame(Buffer) ersetzen

	//Haupt-Loop
	while (!glfwWindowShouldClose(window)) //Fenster muss offen bleiben, solange es nicht manuell geschlossen wird
	{
		glClearColor(0.1f, 0.16f, 0.16f, 1.0f); //Färbt den Bereich in dieser Farbe
		glClear(GL_COLOR_BUFFER_BIT); //Erstellen vom Buffer
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window); //Letzten Frame(schwarz, auch Buffer) durch neuen Frame(Buffer) ersetzen
		glfwPollEvents(); //Damit das Fenster auf Eingaben reagiert
	}

	//Löschung der erstellten Elemente nach dem Schließen
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}