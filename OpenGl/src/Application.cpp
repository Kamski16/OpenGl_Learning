#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "voidox", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "err";
    }

    std::cout << glGetString(GL_VERSION) << "\n";
    {
        float positions[] = {
           100.0f, 100.0f, 0.0f, 0.0f,
           200.0f, 100.0f, 1.0f, 0.0f,
           200.0f, 200.0f, 1.0f, 1.0f,
           100.0f, 200.0f, 0.0f, 1.0f
        };

        unsigned int indicies[] =
        {
            0 , 1 , 2,
            2 , 3 , 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions,4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
    
        IndexBuffer ib(indicies, 6);

        glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200,200,0));

        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color",0.8f,0.3f,0.2f,1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);


        Texture texture("res/textures/MPAY.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture",0);

        Renderer renderer;

        float r = 0.0f;
        float d = 0.0f;
        float increment = 0.05f;
        float incrementd = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            //shader.SetUniform4f("u_Color", r, d, 0.2f, 1.0f);

            renderer.Draw(va,ib,shader);


            if (r > 1.0f) {
                increment = -0.05f;
                incrementd = -0.01f;
            }
            else if (r < 0.0f) {
                increment = 0.05f;
                incrementd = 0.01f;
            }
            r += increment;
            d += incrementd;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}