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

    uniform sampler2D uTextureX;
    uniform sampler2D uTextureY;

    void main() {
        float u_x = texture(uTextureX, texCoord).r;
        float u_y = texture(uTextureY, texCoord).r;

        if (u_x == 111 && u_y == 111) {
            //FragColor = vec4(0.58, 0.29, 0.0, 1.0);
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        } 
        else {
            vec3 colorX;
            vec3 colorY;

            if (u_x > 0.0) {
                colorX = vec3(1.0, 1.0 - u_x * 15, 1.0 - u_x * 15); 
            } 
            else {
                colorX = vec3(1.0 - (-u_x * 15), 1.0 - (-u_x * 15), 1.0); 
            }

            if (u_y > 0.0) {
                colorY = vec3(1.0, 1.0 - u_y * 15, 1.0 - u_y * 15); 
            } 
            else {
                colorY = vec3(1.0 - (-u_y * 15), 1.0 - (-u_y * 15), 1.0); 
            }

            vec3 combinedColor = clamp(colorX + colorY - vec3(1.0), 0.0, 1.0); 
            FragColor = vec4(combinedColor, 1.0); 
        }
    }
)";





enum class DisplayMode {
    VelocityBoth,
    VelocityX,
    VelocityY
};

void updateTextureData(Simulation& simulation, vector<float>& pixelDataX, vector<float>& pixelDataY,
    int rows, int columns, DisplayMode mode) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            Cell cell = simulation.get_matrix().get_element(i, j);

            if (cell.get_fun(FUN_IN) == WALL) {
                pixelDataX[i * columns + j] = 111;
                pixelDataY[i * columns + j] = 111;
            }
            else {
                if (mode == DisplayMode::VelocityBoth || mode == DisplayMode::VelocityX) {
                    pixelDataX[i * columns + j] = cell.get_velocity()[0];
                }
                else {
                    pixelDataX[i * columns + j] = 0.0f;
                }

                if (mode == DisplayMode::VelocityBoth || mode == DisplayMode::VelocityY) {
                    pixelDataY[i * columns + j] = cell.get_velocity()[1];
                }
                else {
                    pixelDataY[i * columns + j] = 0.0f;
                }
            }
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

void runSimulationWindow(const string& windowName, int rows, int columns, int width, int height, DisplayMode mode) {
    sf::Window window(sf::VideoMode(width, height), windowName, sf::Style::Default, sf::ContextSettings(24));
    glewInit();

    GLuint vao, vbo, textureX, textureY;
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

    glGenTextures(1, &textureX);
    glBindTexture(GL_TEXTURE_2D, textureX);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, columns, rows, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &textureY);
    glBindTexture(GL_TEXTURE_2D, textureY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, columns, rows, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    glUniform1i(glGetUniformLocation(shaderProgram, "uTextureX"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTextureY"), 1);

    Simulation simulation(rows, columns);
    simulation.get_matrix().prepare_environment();

    vector<float> pixelDataX(rows * columns);
    vector<float> pixelDataY(rows * columns);

    sf::Clock logicClock, renderClock;
    int n = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
       
        if (logicClock.getElapsedTime().asMilliseconds() >= 100) {
            
            logicClock.restart();
            updateTextureData(simulation, pixelDataX, pixelDataY, rows, columns, mode);
            simulation.collision();
            simulation.streaming();
            n++;
            cout << "Okno: " << windowName << ", iteracja: " << n << endl;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureX);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, columns, rows, GL_RED, GL_FLOAT, pixelDataX.data());

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureY);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, columns, rows, GL_RED, GL_FLOAT, pixelDataY.data());
        }


        if (renderClock.getElapsedTime().asMilliseconds() >= 16) {
            renderClock.restart();
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            window.display();
        }
    }


    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &textureX);
    glDeleteTextures(1, &textureY);
    glDeleteProgram(shaderProgram);
}

int main() {
    const int rows = 50, columns = 109;

    thread window1Thread(runSimulationWindow, "Simulation LBM", rows, columns, 800, 600, DisplayMode::VelocityBoth);
    thread window2Thread(runSimulationWindow, "Horizontal Velocity", rows, columns, 400, 280, DisplayMode::VelocityX);
    thread window3Thread(runSimulationWindow, "Vertical Velocity", rows, columns, 400, 280, DisplayMode::VelocityY);

    window1Thread.join();
    window2Thread.join();
    window3Thread.join();

    return 0;
}