#include "RenderWindow.h"

#include "ptmath/vec.h"

#include <cmath>
#include <stdexcept>
#include <iostream>
#include <cstddef>
#include <algorithm>

const std::string_view RenderWindow::m_vsSource = R"glsl(
#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out VS_OUT
{
    vec2 texCoord;
} vs_out;

void main()
{
    vs_out.texCoord = texCoord;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}

)glsl";

const std::string_view RenderWindow::m_fsSource = R"glsl(
#version 330 core

out vec4 FragColor;

in VS_OUT
{
    vec2 texCoord;
} fs_in;

uniform sampler2D u_texture;

void main()
{
   FragColor = texture(u_texture, fs_in.texCoord);
}

)glsl";

const RenderWindow::vertex_t RenderWindow::m_vertices[4] = {
    // Bottom left
    RenderWindow::vertex_t {
        { -1.0, -1.0 },
        {  0.0,  0.0 }
    },
    // Bottom right
    RenderWindow::vertex_t {
        {  1.0, -1.0 },
        {  1.0,  0.0 }
    },
    // Top left
    RenderWindow::vertex_t {
        { -1.0,  1.0 },
        {  0.0,  1.0 }
    },
    // Top right
    RenderWindow::vertex_t {
        {  1.0,  1.0 },
        {  1.0,  1.0 }
    }
};

const GLuint RenderWindow::m_indices[6] = {
    0, 1, 2,
    1, 3, 2
};

size_t RenderWindow::width() const noexcept
{
    return m_width;
}

size_t RenderWindow::height() const noexcept
{
    return m_height;
}

RenderWindow::RenderWindow(size_t width, size_t height, const std::string& title):
    m_width{ width },
    m_height{ height }
{
    if(GLFW_TRUE != glfwInit()) {
        throw std::runtime_error("glfwInit");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);

    m_window = glfwCreateWindow((int)m_width, (int)m_height,
                                title.c_str(), nullptr, nullptr);
    if(nullptr == m_window) {
        throw std::runtime_error("glfwCreateWindow");
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);

    if(GLEW_OK != glewInit()) {
        throw std::runtime_error("glewInit");
    }

    struct {
        GLint major = 0;
        GLint minor = 0;
        char *vendor;
        char *renderer;
        char *glslVersion;
    } glVersion;

    glGetIntegerv(GL_MAJOR_VERSION, &glVersion.major);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion.minor);
    glVersion.vendor = (char*)glGetString(GL_VENDOR);
    glVersion.renderer = (char*)glGetString(GL_RENDERER);
    glVersion.glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "OpenGL info\n"
              << "Vendor: " << glVersion.vendor << "\n"
              << "Renderer: " << glVersion.renderer << "\n"
              << "Version: " << glVersion.major << '.' << glVersion.minor << "\n"
              << "GLSL: " << glVersion.glslVersion << "\n";

    auto compileShader = [](const std::string_view& src, GLenum type) -> GLuint {
        GLuint id = glCreateShader(type);
        const char *src_data = src.data();
        glShaderSource(id, 1, &src_data, nullptr);
        glCompileShader(id);

        GLint compileStatus = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
        if(GL_TRUE != compileStatus) {
            const auto errorLength = 512;
            auto compileError = std::make_unique<char[]>(errorLength);
            glGetShaderInfoLog(id, errorLength, nullptr, compileError.get());
            throw std::runtime_error(std::string(compileError.get()));
        }
        return id;
    };
    GLuint vs = compileShader(m_vsSource, GL_VERTEX_SHADER);
    GLuint fs = compileShader(m_fsSource, GL_FRAGMENT_SHADER);

    m_program = glCreateProgram();
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);
    glLinkProgram(m_program);

    GLint linkStatus = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
    if(GL_TRUE != linkStatus) {
        const auto errorLength = 512;
        auto linkError = std::make_unique<char[]>(errorLength);
        glGetProgramInfoLog(m_program, errorLength, nullptr, linkError.get());
        throw std::runtime_error(std::string(linkError.get()));
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(m_program);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (void*)offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (void*)offsetof(vertex_t, texCoord));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glGenTextures(1, &m_to);
    glBindTexture(GL_TEXTURE_2D, m_to);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, (int)m_width, (int)m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glUniform1i(glGetUniformLocation(m_program, "u_texture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_to);

    auto genPbo = [this](GLuint& pbo) -> void {
        glGenBuffers(1, &pbo);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * sizeof(color_t), nullptr, GL_STREAM_DRAW);
    };

    genPbo(m_pboFront);
    genPbo(m_pboBack);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pboFront);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (int)m_width, (int)m_height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    m_colorData = std::make_unique<vec3_t[]>(m_width * m_height);
}

bool RenderWindow::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

RenderWindow::~RenderWindow() noexcept
{
    glDeleteProgram(m_program);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void RenderWindow::swapBuffers()
{
    auto latch = std::unique_lock<std::mutex>(mutex);

    std::swap(m_pboFront, m_pboBack);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pboFront);

    static const auto componentToInt = [](const double v) -> std::uint8_t {
        return (std::uint8_t)(v * std::numeric_limits<std::uint8_t>::max());
    };
    
    color_t *pixelData = (color_t*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    for(auto x = 0u; x < m_width; ++x) {
        for(auto y = 0u; y < m_height; ++y) {
            pixelData[y * m_width + x].r = componentToInt(m_colorData[y * m_width + x].r);
            pixelData[y * m_width + x].g = componentToInt(m_colorData[y * m_width + x].g);
            pixelData[y * m_width + x].b = componentToInt(m_colorData[y * m_width + x].b);
        }
    }
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (int)m_width, (int)m_height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

vec3_t& RenderWindow::operator[](const size_t idx)
{
    return m_colorData[idx];
}

void RenderWindow::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}
