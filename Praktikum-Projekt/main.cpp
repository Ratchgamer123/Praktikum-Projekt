#include "model.h"

//Variablen
const unsigned int width = 1024;
const unsigned int height = 1024;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Hauptversion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); //Nebenversion
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
	
	//Shader-Programm erstellen; siehe shaderClass.h/.cpp
	Shader shaderProgram("main.vert", "main.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.7f, 0.4f, 0.7f);
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
	lightModelMatrix = glm::translate(lightModelMatrix, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model("Models/sword/scene.gltf");

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Löschen vom Front-Buffer

		camera.updateMatrix(50.0f, 0.01f, 100.0f);
		
		model.Draw(shaderProgram, camera);

		glfwSwapBuffers(window); //Letzten Frame(schwarz, auch Buffer) durch neuen Frame(Buffer) ersetzen
		glfwPollEvents(); //Damit das Fenster auf Eingaben reagiert
	}

	//Löschung der erstellten Elemente nach dem Schließen
	shaderProgram.Delete();
	
	

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}