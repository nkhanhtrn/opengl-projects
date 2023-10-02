#include "main.h"

int main()
{
    // create window via GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // load OpenGL with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
   
    // set openGL viewport
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /******
     * openGL stuffs
    */
    /* vertex shader */
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // error handling
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /* fragment shader - Orange */
    unsigned int fragmentShader_Orange;
    fragmentShader_Orange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_Orange, 1, &fragmentShaderSource_Orange, NULL);
    glCompileShader(fragmentShader_Orange);
    /* fragment shader - Yellow */
    unsigned int fragmentShader_Yellow;
    fragmentShader_Yellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_Yellow, 1, &fragmentShaderSource_Yellow, NULL);
    glCompileShader(fragmentShader_Yellow);

    /* linking shader program */
    unsigned int shaderProgram1;
    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader_Orange);
    glLinkProgram(shaderProgram1);

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader_Yellow);
    glLinkProgram(shaderProgram2);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader_Orange);
    glDeleteShader(fragmentShader_Yellow);

    /* Buffers */
    float vertices_1[] = {
        0.0f,  0.5f, 0.0f,  // top right
        0.5f, 0.0f, 0.0f,  // bottom right
        -0.5f, 0.0f, 0.0f,  // bottom left
    };
    float vertices_2[] = {
        0.5f, 0.0f, 0.0f,  // bottom right
        -0.5f, 0.0f, 0.0f,  // bottom left
        0.0f,  -0.5f, 0.0f,  // top right
    };

    unsigned int VAO_1, VBO_1, VAO_2, VBO_2;
    glGenVertexArrays(1, &VAO_1);
    glGenVertexArrays(1, &VAO_2);
    glGenBuffers(1, &VBO_1);
    glGenBuffers(1, &VBO_2);

    // buffer array
    glBindVertexArray(VAO_1); // VAO match attribPointer vs VBO to allow faster switching vertex data
    // buffer: VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO_2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // rendering loop
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO_1); // rebind if there're multiple VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO_2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        // final steps
        glfwPollEvents();
        glfwSwapBuffers(window); // swap back-front color buffer
    }

    // cleanup
    glDeleteVertexArrays(1, &VAO_1);
    glDeleteVertexArrays(1, &VAO_2);
    glDeleteBuffers(1, &VBO_1);
    glDeleteBuffers(1, &VBO_2);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}