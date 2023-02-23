#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vertex shader -> convert to normalized device coordinate is the main goal
const char* vertexShaderSource = \
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment shader -> computing color of pixels is main goal
const char* vertexFragmentSource = \
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

// Callback to resize Viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Process Input and how to handle it
void processInput(GLFWwindow* window);

int logShaderCompile(const unsigned int shaderId, const char* shaderType);

int logProgramLink(const unsigned int shaderProgram);

int main(int argc, char** argv)
{
    // ------------------------------------
    //      Init glfw, glad, window
    // ------------------------------------
    // glfw: initialize and configure
    // ----------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent( window );
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ------------------------------------
    //      SHADER
    // ------------------------------------
    // compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // attach shader src code to shader object and compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if compiled properly and log
    logShaderCompile(vertexShader, "VERTEX");

    // compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &vertexFragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Log error fragment shader compiler
    logShaderCompile(fragmentShader, "FRAGMENT");

    // We now need a shader program to link the vertex and fragment shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Log linking errors
    logProgramLink(shaderProgram);

    // Now that it the shaders are linked into the program -> delete
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Use the program that was just created
    glUseProgram(shaderProgram);        // Every shader and redering call after will now use this program


    // ------------------------------------
    //          SHAPE
    // ------------------------------------
    //      Triangle
    // ------------------------------------
    //  Triangle vertices in Normalized Device Coordinates /output of vertex shader has to be in it
    float vertices[] {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    // generate vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // and then bind it. Only can bind one type at a time.
    // All operations on a GL_ARRAY_BUFFER will be done on VBO as it is currently bound.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // copy vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // We can give as many attributes to each vertex, we need to specify clearly what lies where
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 0: we set location=0 for the position in vertex shader
    // 3: size of vertex attribute, we have a vec3 -> 3 values
    // GL_FLOAT: dtype
    // GL_FALSE: do not normalize
    // 3*sizeof(float): stride = space between consecutive vertex attribute
    // (void*)0: offset of where the position data begins in the buffer

    // Enable vertex attribute
    glEnableVertexAttribArray(0);

    // Now we should use some function for drawing, but if we have lots of objects -> cumbersome
    // -> use Vertex Array Object (core opengl requires VAO)

    // ..:: Initialization code
    // 0. create Vertex Attribute Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 1. bind VAO
    glBindVertexArray(VAO);
    // 2. copy our vertices in a buffer for opengl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw
    // glDrawArrays(GL_TRIANGLES, 0, 3);


    // ---------------------------------
    //      Rectangle
    // ---------------------------------
    // If we want to draw a rectnagle which is 2 triangle, we have many duplicate vertices
    // Instead do this:
    /*
    float verticesRectangle[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // process inputs
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Select shader program, bind VAO and draw!
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffers and poll IO events (key pressed/released, ...)
        // -----------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clear all previous allocated GLFW resources
    // ------------------------------------------------------------
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int logShaderCompile(const unsigned int shaderId, const char* shaderType)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return success;
};

int logProgramLink(const unsigned int shaderProgram)
{
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return success;
};
