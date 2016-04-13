#pragma once
#include "GL/glew.h"
#include "wx/glcanvas.h"
#define GLM_FORCE_CXX14
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GlEquilateralTriangle
{
public:
    GlEquilateralTriangle(float radius, float w, GLuint shaderProgram);
    virtual ~GlEquilateralTriangle() noexcept;
    void Paint(glm::mat4& transform) const noexcept;
private:
    float m_radius;
    float m_w;

    GLuint m_vao;
    GLuint m_vertexVbo;
    GLuint m_colorVbo;
    GLint m_transform;
};

