
#include <stb_image.h>
#include <shader.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <learnopengl/filesystem.h>

#include <iostream>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

using namespace dlib;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadtexture(const char* path);

//缩放
float processScale_x(GLFWwindow *window, float & param);
float processScale_y(GLFWwindow *window, float & param);
float processScale_z(GLFWwindow *window, float & param);
//旋转
float processRotate_x(GLFWwindow *window, float & param);
float processRotate_y(GLFWwindow *window, float & param);
float processRotate_z(GLFWwindow *window, float & param);
//平移
float processTranslate_x(GLFWwindow *window, float & param);
float processTranslate_y(GLFWwindow *window, float & param);
float processTranslate_z(GLFWwindow *window, float & param);

void renderArrays();

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float fov = 35.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int bg_VBO, bg_VAO;

int main()
{
	//using dlib

	frontal_face_detector detector = get_frontal_face_detector();

	string dlibmodelpath = "../../load3dmodel/models/shape_predictor_68_face_landmarks.dat";
	shape_predictor sp;
	deserialize(dlibmodelpath) >> sp;


	image_window win, win_faces;

	string imagePath = "../../load3dmodel/textures/people.jpg";
	cout << "processing image " << imagePath << endl;
	array2d<rgb_pixel> img;
	load_image(img, imagePath);
	// Make the image larger so we can detect small faces.
	pyramid_up(img);

	std::vector<rectangle> dets = detector(img);
	cout << "Number of faces detected: " << dets.size() << endl;


	std::vector<full_object_detection> shapes;
	for (unsigned long j = 0; j < dets.size(); ++j)
	{
		full_object_detection shape = sp(img, dets[j]);
		cout << "number of parts: " << shape.num_parts() << endl;
		cout << "pixel position of first part:  " << shape.part(0) << endl;
		cout << "pixel position of second part: " << shape.part(1) << endl;
		// You get the idea, you can get all the face part locations if
		// you want them.  Here we just store them in shapes so we can
		// put them on the screen.
		shapes.push_back(shape);
	}

	// Now let's view our face poses on the screen.
	win.clear_overlay();
	win.set_image(img);
	win.add_overlay(render_face_detections(shapes));

	// We can also extract copies of each face that are cropped, rotated upright,
	// and scaled to a standard size as shown here:
	dlib::array<array2d<rgb_pixel> > face_chips;
	extract_image_chips(img, get_face_chip_details(shapes), face_chips);
	win_faces.set_image(tile_images(face_chips));

	cout << "Hit enter to process the next image..." << endl;
	cin.get();








	return 0;






	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(false);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("../../load3dmodel/shader/vertex.glsl", "../../load3dmodel/shader/frag.glsl");

	// load models
	// -----------
	Model ourModel(FileSystem::getPath("learning/load3dmodel/models/v_mask/v_mask.obj"));
	//Model ourModel(FileSystem::getPath("learning/load3dmodel/models/Mask_Diavolo/13551_Diavolo_Mask_v1_l3.obj"));
	//Model ourModel(FileSystem::getPath("learning/load3dmodel/models/Mask_Gatto_Rombi/13550_Gatto_Rombi_Mask_v1_l3.obj"));
	//Model ourModel(FileSystem::getPath("learning/load3dmodel/models/Mask_Volto/13549_Volto_Mask_L2.obj"));


	Shader bg_Shader("../../load3dmodel/shader/bg_vertex.glsl", "../../load3dmodel/shader/bg_frag.glsl");

	unsigned int texture1 = loadtexture("../../load3dmodel/textures/people.jpg");
	bg_Shader.use();
	bg_Shader.setInt("texture1", 0);
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//缩放
	float param_scale_x = 0.0;
	float param_scale_y = 0.0;
	float param_scale_z = 0.0;
	//旋转
	float param_rotate_x = 0.0;
	float param_rotate_y = 0.0;
	float param_rotate_z = 0.0;
	//平移
	float param_translate_x = 0.0;
	float param_translate_y = 0.0;
	float param_translate_z = 0.0;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// activate shader
		bg_Shader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 bg_projection = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.0f);
		bg_Shader.setMat4("projection", bg_projection);

		// camera/view transformation
		glm::mat4 bg_view = glm::mat4(1.0f); //glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		bg_view = camera.GetViewMatrix();
		bg_Shader.setMat4("view", bg_view);

		// render boxes
		//glBindVertexArray(bg_VAO);
		glm::mat4 bg_model = glm::mat4(1.0f);
		//bg_model = glm::rotate(bg_model, glm::radians(10.0f)*currentFrame, glm::vec3(0.0f, 0.0f, 0.1f));
		bg_Shader.setMat4("model", bg_model);

		//model = glm::translate(model, cubePositions[i]);

		renderArrays();


		// don't forget to enable shader before setting uniforms
		// 3D 模型导入及显示
		ourShader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		//缩放
		param_scale_x = processScale_x(window, param_scale_x);
		param_scale_y = processScale_y(window, param_scale_y);
		param_scale_z = processScale_z(window, param_scale_z);
		std::cout << "my param_scale_x is " << param_scale_x << std::endl;
		std::cout << "my param_scale_y is " << param_scale_y << std::endl;
		std::cout << "my param_scale_z is " << param_scale_z << std::endl;
		model = glm::scale(model, glm::vec3(param_scale_x + 0.1f, param_scale_y + 0.1f, param_scale_z + 0.1f));	// it's a bit too big for our scene, so scale it down
		//旋转
		param_rotate_x = processRotate_x(window, param_rotate_x);
		param_rotate_y = processRotate_y(window, param_rotate_y);
		param_rotate_z = processRotate_z(window, param_rotate_z);
		std::cout << "my param_rotate_x is " << param_rotate_x << std::endl;
		std::cout << "my param_rotate_y is " << param_rotate_y << std::endl;
		std::cout << "my param_rotate_z is " << param_rotate_z << std::endl;
		model = glm::rotate(model, glm::radians(param_rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(param_rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(param_rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
		//平移
		param_translate_x = processTranslate_x(window, param_translate_x);
		param_translate_y = processTranslate_y(window, param_translate_y);
		param_translate_z = processTranslate_z(window, param_translate_z);
		std::cout << "my param_scale_x is " << param_translate_x << std::endl;
		std::cout << "my param_scale_y is " << param_translate_y << std::endl;
		std::cout << "my param_scale_z is " << param_translate_z << std::endl;
		model = glm::translate(model, glm::vec3(param_translate_x, param_translate_y, param_translate_z)); // translate it down so it's at the center of the scene

		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &bg_VAO);
	glDeleteBuffers(1, &bg_VBO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



unsigned int loadtexture(const char* path)
{
	// load and create a texture 
// -------------------------
	unsigned int texture;
	GLenum format;
	GLenum format_internal;

	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (nrChannels == 1)
	{
		format_internal = GL_RED;
		format = GL_RED;
	}
	else if (nrChannels == 3)
	{
		format_internal = GL_RGB;
		format = GL_RGB;
	}
	else
	{
		format_internal = GL_RGBA;
		format = GL_RGBA;
	}

	if (data)
	{
		std::cout << "pic width is " << width << ", height is " << height << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, format_internal, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_image_free(data);
	return texture;
}
//缩放
float processScale_x(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS))
	{
		if (param < 1.0)
		{
			param += 0.01;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS))
	{
		if (param > 0.0)
		{
			param -= 0.01;
		}
	}
	return param;
}

float processScale_y(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS))
	{
		if (param < 1.0)
		{
			param += 0.01;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS))
	{
		if (param > 0.0)
		{
			param -= 0.01;
		}
	}
	return param;
}

float processScale_z(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS))
	{
		if (param < 2.0)
		{
			param += 0.01;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS))
	{
		if (param > 0.0)
		{
			param -= 0.01;
		}
	}
	return param;
}
//旋转
float processRotate_x(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS))
	{
		if (param < 180.0)
		{
			param += 1.0;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS))
	{
		if (param > -180.0)
		{
			param -= 1.0;
		}
	}
	return param;
}

float processRotate_y(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS))
	{
		if (param < 180.0)
		{
			param += 1.0;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS))
	{
		if (param > -180.0)
		{
			param -= 1.0;
		}
	}
	return param;
}

float processRotate_z(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS))
	{
		if (param < 180.0)
		{
			param += 1.0;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS))
	{
		if (param > -180.0)
		{
			param -= 1.0;
		}
	}
	return param;
}
//平移
float processTranslate_x(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS))
	{
		if (param < 2.0)
		{
			param += 0.01;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS))
	{
		if (param > -2.0)
		{
			param -= 0.01;
		}
	}
	return param;
}

float processTranslate_y(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS))
	{
		if (param < 2.0)
		{
			param += 0.01;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS))
	{
		if (param > -2.0)
		{
			param -= 0.01;
		}
	}
	return param;
}

float processTranslate_z(GLFWwindow * window, float & param)
{
	if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS))
	{
		if (param < 2.0)
		{
			param += 0.01;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS))
	{
		if (param > -2.0)
		{
			param -= 0.01;
		}
	}
	return param;
}


void renderArrays()
{
	float vertices[] = {
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,


	};

	//unsigned int VBO, VAO;
	glGenVertexArrays(1, &bg_VAO);
	glGenBuffers(1, &bg_VBO);

	glBindVertexArray(bg_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, bg_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 6);




}