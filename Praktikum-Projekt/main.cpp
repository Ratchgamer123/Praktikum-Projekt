#include "model.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//Variablen
const unsigned int width = 1024;
const unsigned int height = 1024;

//Skybox Test
float skyboxVertices[] =
{
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
};

unsigned int skyboxIndices[] = 
{
	//rechts
	1, 2, 6,
	6, 5, 1,
	//links
	0, 4, 7,
	7, 3, 0,
	//oben
	4, 5, 6,
	6, 7, 4,
	//unten
	0, 3, 2,
	2, 1, 0,
	//hinten
	0, 1, 5,
	5, 4, 0,
	//vorne
	3, 7, 6,
	6, 2, 3
};

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
	Shader skyboxShaderProgram("skybox.vert", "skybox.frag");

	glm::vec4 lightColor = glm::vec4(0.99215f, 0.95686f, 0.86274f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.7f, 0.4f, 0.7f);
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
	lightModelMatrix = glm::translate(lightModelMatrix, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	skyboxShaderProgram.Activate();
	glUniform1i(glGetUniformLocation(skyboxShaderProgram.ID, "skybox"), 0);

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model("Models/sphere/scene.gltf");

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string cubemapFaces[6] =
	{
		"SkyboxTextures/px.png",
		"SkyboxTextures/nx.png",
		"SkyboxTextures/py.png",
		"SkyboxTextures/ny.png",
		"SkyboxTextures/pz.png",
		"SkyboxTextures/nz.png",
	};

	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++) 
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(cubemapFaces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else 
		{
			std::cout << "Cubemap konnte nicht geladen werden!" << std::endl;
			stbi_image_free(data);
		}
	}

	glfwSwapInterval(0);

	float prevTime = 0.0;
	float prevFrameTime = 0.0;
	float crntTime = 0.0;
	float deltaTime;
	float fpsTime;
	unsigned int counter = 0;

	//Haupt-Loop
	while (!glfwWindowShouldClose(window)) //Fenster muss offen bleiben, solange es nicht manuell geschlossen wird
	{
		crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;
		fpsTime = crntTime - prevFrameTime;
		counter++;
		if (fpsTime >= 1.0 / 15.0)
		{
			std::string fps = std::to_string((1.0 / fpsTime) * counter);
			std::string ms = std::to_string((fpsTime / counter) * 1000);
			std::string newTitle = "Praktikum-Projekt --- " + fps + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			counter = 0;
			prevFrameTime = crntTime;
		}
		
		prevTime = crntTime;
		camera.Inputs(window, deltaTime);
		glClearColor(0.1f, 0.16f, 0.16f, 1.0f); //Färbt den Bereich in dieser Farbe
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Löschen vom Front-Buffer

		camera.updateMatrix(50.0f, 0.01f, 100.0f);
		
		model.Draw(shaderProgram, camera);

		glDepthFunc(GL_LEQUAL);

		skyboxShaderProgram.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(50.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram.ID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram.ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window); //Letzten Frame(schwarz, auch Buffer) durch neuen Frame(Buffer) ersetzen
		glfwPollEvents(); //Damit das Fenster auf Eingaben reagiert
	}

	//Löschung der erstellten Elemente nach dem Schließen
	shaderProgram.Delete();
	
	

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}