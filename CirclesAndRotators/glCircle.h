#pragma once
#include "GL/glew.h"
#include "wx/glcanvas.h"
#define GLM_FORCE_CXX14
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GlCircle
{
public:
    GlCircle(float radius, float w, GLuint shaderProgram);
    virtual ~GlCircle() noexcept;
    void Paint(glm::mat4& transform, const glm::vec4& color) const noexcept;

private:
    float m_radius;
    float m_w;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
//    GLuint m_circleShaderProg;
    GLint m_transform;
    GLint m_viewDimensions;
    GLint m_outerRadius;
    GLint m_color;
};