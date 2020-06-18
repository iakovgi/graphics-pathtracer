#ifndef PT_RENDERWINDOW_H__
#define PT_RENDERWINDOW_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <string_view>
#include <mutex>

struct vec3_t;

class RenderWindow
{
public:
    struct color_t
    {
        std::uint8_t r, g, b;
    };
    
    RenderWindow(size_t width, size_t height, const std::string& title = "RenderWindow");
    ~RenderWindow() noexcept;

    void display();
    void swapBuffers();
    bool shouldClose();

    vec3_t& operator[](const size_t idx);

    size_t width() const noexcept;
    size_t height() const noexcept;

    std::mutex mutex;
    
private:
    GLFWwindow *m_window;

    const size_t m_width;
    const size_t m_height;

    static const std::string_view m_vsSource;
    static const std::string_view m_fsSource;
    GLuint m_program;

    struct vertex_t
    {
        float position[2];
        float texCoord[2];
    };

    static const vertex_t m_vertices[4];
    static const GLuint m_indices[6];

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;

    std::unique_ptr<vec3_t[]> m_colorData;

    GLuint m_pboFront;
    GLuint m_pboBack;

    GLuint m_to;
};

#endif //PT_RENDERWINDOW_H__
