#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <dirent.h>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <tuple>
#include <string>
#include <direct.h>
#include <C:\Users\Raheem\Downloads\bass24\c\bass.h>
#include "FilePicker.h"
#include <filesystem>
#include <vector>
bool result = FALSE;
int isPlaying = 0;
HCHANNEL channel = NULL;
HSTREAM stream = NULL;
int pos = 0;
int gosh = 0;
static char playListName[128] = "";
int lengthOfSong = 0;
int volume = 5;
bool isMediaLoaded = false;
bool isMediaPaused = false;

float fft[256];

// Vertex Shader source code

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float size;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(size * aPos.x, size * aPos.y, size * aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

int GetDuration(HSTREAM h) {
	QWORD len = BASS_ChannelGetLength(h, BASS_POS_BYTE); // the length in bytes
	int timeR = BASS_ChannelBytes2Seconds(h, len); // the length in seconds
	
	lengthOfSong = timeR;
	return timeR;
}

int ConvertTime(int n)
{	
	return n / 60;
}

int main()
{
	// Initialize GLFW
	DIR* d;
	struct dirent* dir;
	std::vector<std::string> dirlist;
	int i = 0;
	d = opendir("Playlists");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			i++;
			//cout<<dir->d_name<<endl;
			dirlist.push_back(dir->d_name);
		}
		for (int i = 0; i < dirlist.size(); i++)
			std::cout << dirlist[i] << std::endl;
	}
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "Elm Media Player", NULL, NULL);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 900, 900);

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);




	// Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Variables to be changed in the ImGUI window
	bool loopMedia = true;
	float size = 1.0f;
	float color[4] = { 3.8f, 10.3f, 0.02f, 1.0f };


	// Main while loop

	while (!glfwWindowShouldClose(window))
	{

		int realPos;
		const int buf = 4096;
		QWORD trackpos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
		realPos = BASS_ChannelBytes2Seconds(stream, trackpos);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		

	
		glColor3f(0, 0, 0);
		glVertex2f(12800, 7200);
		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		
		

		// Tell OpenGL which Shader Program we want to use

		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// ImGUI window creation
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		//ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		
			// Draw the triangle using the GL_TRIANGLES primitive
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		
		ImGui::Begin("Elm Player", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
		// Text that appears in the window
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Song")) {
					result = openFile();
					if (result && isPlaying == 1) {
						BASS_ChannelFree(stream);
						BASS_ChannelSetPosition(stream, 0, BASS_POS_BYTE);
						QWORD tpos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
						int trealPos = BASS_ChannelBytes2Seconds(stream, tpos);
					}
					BASS_ChannelFree(stream);
					BASS_Init(-1, 44100, 0, 0, NULL);
					BASS_SetVolume(.02);
					HSAMPLE sample = BASS_SampleLoad(false, sFilePath.c_str(), 0, 0, 1, BASS_SAMPLE_MONO);

					stream = BASS_StreamCreateFile(FALSE, sFilePath.c_str(), 0, 0, 0);
					channel = BASS_SampleGetChannel(stream, FALSE);
					
					isMediaLoaded = true;
					GetDuration(stream);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Playlist"))
			{
				
				ImGui::Text("Create New Playlist");
				
				
				if (ImGui::InputTextWithHint("Playlist Name", "Enter Playlist Name", playListName, IM_ARRAYSIZE(playListName), ImGuiInputTextFlags_CallbackCompletion| ImGuiInputTextFlags_EnterReturnsTrue)) {
					static char playlistFolder[50]=".\\Playlists\\";
					strcat_s(playlistFolder, playListName);
					
					
					_mkdir(playlistFolder);
				}
				if (ImGui::BeginTabBar("PlayListBar"))
				{
					if (ImGui::TabItemButton("Add current", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip)) {
						ImGui::OpenPopup("MyHelpMenu");
					}

					if (ImGui::BeginPopup("MyHelpMenu"))
					{
						const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
						static int item_current_idx = 0; // Here we store our selection data as an index.
						if (ImGui::BeginListBox("Available Playlists"))
						{
							for (int n = 0; n <dirlist.size(); n++)
							{
								const bool is_selected = (item_current_idx == n);
								if (ImGui::Selectable(dirlist[n].c_str(), is_selected))
									item_current_idx = n;

								// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndListBox();
						}
						ImGui::EndPopup();
					}
				}
				//std::cout << playListName;
				ImGui::EndMenu();

			
				
				
			}
			if (ImGui::BeginMenu("Playback"))
			{
				
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help "))
			{
				ImGui::MenuItem("Dummy");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Text("Now Playing:"); ImGui::SameLine();
		ImGui::Text (sSelectedFile.c_str());
		// Checkbox that appears in the window
		ImGui::Checkbox("Loop", &loopMedia);
		// Slider that appears in the window
		int actTime=ConvertTime(lengthOfSong);
		
		std::string s = std::to_string(lengthOfSong);
		
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth()/1.1);
		if (ImGui::SliderInt(s.c_str(), &realPos, 0, lengthOfSong)) {
			QWORD loc;
			loc = BASS_ChannelSeconds2Bytes(stream, realPos);
			BASS_ChannelSetPosition(stream, loc, BASS_POS_BYTE);
			BASS_ChannelPlay(stream, FALSE);
		}
		
		ImGui::Text("%d", realPos);
		if (ImGui::SliderInt("Volume", &volume, 0, 100)) {
			float m = static_cast<float>(volume);
			BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL,volume);
		}

		if (ImGui::Button("Play", ImVec2(60, 30)) && isMediaLoaded==true) {
			if (isMediaPaused == true) {
				isMediaPaused = false;
				QWORD loc;
				loc = BASS_ChannelSeconds2Bytes(stream, realPos);
				BASS_ChannelSetPosition(stream, loc, BASS_POS_BYTE);
				BASS_ChannelPlay(stream, FALSE);
			}
			else {
				BASS_ChannelPlay(stream, TRUE);
			}
		}
		ImGui::SameLine();
		
		
		
		BASS_ChannelGetData(stream, fft, BASS_DATA_FFT256);
		for (int a = 0; a < 256; a++) {
			//printf("%d: %f\n", a, fft[a]);
			//size = fft[a];
			
			
			
			glUseProgram(shaderProgram);
			glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
			glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);
		}
		for (int a = 0; a < 512; a++) {
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw the line
			glDrawArrays(GL_LINES, 0, 2);

			// Swap front and back buffers
			

			// Poll for and process events
			glfwPollEvents();
			
		}
		if (ImGui::Button("Stop", ImVec2(60, 30))) {
			BASS_ChannelStop(stream);
		} ImGui::SameLine();

		if (ImGui::Button("Pause", ImVec2(60, 30))) {
			BASS_ChannelPause(stream);
			isMediaPaused = true;
		}

		if (loopMedia == true && realPos == lengthOfSong) {
			BASS_ChannelPlay(stream, TRUE);

		}
		
		ImGui::PlotLines("Visualizer", fft, IM_ARRAYSIZE(fft));
		ImGui::End();

		// Export variables to shader
		
		

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}


