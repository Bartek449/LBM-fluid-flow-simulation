#include <SFML/Window.hpp>
#include <GL/glew.h>

#include "Simulation.h"

const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoord;

        out vec2 texCoord;

        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            texCoord = aTexCoord;
        }
    )";

const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 texCoord;
        out vec4 FragColor;

        uniform sampler2D uTexture;

        void main() {
            float color = texture(uTexture, texCoord).r;
            if (color == -1.0) { 
                FragColor = vec4(0.58, 0.29, 0.0, 1.0); 
            } else {
                FragColor = vec4(color, color, color, 1.0); 
            }
        }
    )";

void updateTextureData(Simulation& simulation, vector<float>& pixelData, int rows, int columns) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            Cell cell = simulation.get_matrix().get_element(i, j);
            float color;
            if (cell.get_fun(FUN_IN) == WALL) color = -1.0f;
            else color = 1 - cell.get_density();

            pixelData[i * columns + j] = color;
        }
    }
}

void checkShaderCompilation(GLuint shader, const string& shaderType) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw runtime_error(shaderType + " Shader Compilation Failed: " + string(infoLog));
    }
}

void checkProgramLinking(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        throw runtime_error("Shader Program Linking Failed: " + string(infoLog));
    }
}

int main() {
    const int rows = 50, columns = 109;

    Simulation simulation(rows, columns);
    simulation.get_matrix().prepare_environment();

    sf::Window window(sf::VideoMode(800, 600), "LGA Simulation", sf::Style::Default, sf::ContextSettings(24));
    glewInit();

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, columns, rows, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    vector<float> pixelData(rows * columns);

    GLuint shaderProgram = glCreateProgram();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader, "Vertex");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader, "Fragment");

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinking(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    sf::Clock logic_clock, render_clock;

    int n = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        if (logic_clock.getElapsedTime().asMilliseconds() >= 100) {
            logic_clock.restart();
            updateTextureData(simulation, pixelData, rows, columns);

            simulation.collision();
            simulation.streaming();

            n++;

            cout << "Iteracja: " << n << endl;

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, columns, rows, GL_RED, GL_FLOAT, pixelData.data());
        }

        if (render_clock.getElapsedTime().asMilliseconds() >= 16) {
            render_clock.restart();
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            window.display();
        }
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);

    return 0;
}
