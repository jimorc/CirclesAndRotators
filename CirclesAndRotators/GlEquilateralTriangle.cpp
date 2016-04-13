#include "GlEquilateralTriangle.h"

GlEquilateralTriangle::GlEquilateralTriangle(float radius, float w, GLuint shaderProgram)
    : m_radius(radius), m_w(w)
{
    // create vertices for triangle centred at origin
    float x = m_radius * sin(60.0f * 3.1415926f / 180.0f);
    float y = m_radius * cos(60.0f * 3.1415926f / 180.0f);
    glm::vec4 vertices[] = {
        { -x, -y, 0.0f, w },
        { x, -y, 0.0f, w },
        { 0.0f, m_radius, 0.0f, w }
    };

    glm::vec4 vertexColors[] = {
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f }
    };

    GLint elements[3] = { 0, 1, 2 };
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    // upload vertex data
    glGenBuffers(1, &m_vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set up position attribute used in triangle vertex shader
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(posAttrib);
    // set up color attribute used in triangle vertex shader
    // upload color data
    glGenBuffers(1, &m_colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "inColor");
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colAttrib);
    // set up the uniform arguments
    m_transform = glGetUniformLocation(shaderProgram, "transform");
    glBindVertexArray(0);           // unbind the VAO
}

GlEquilateralTriangle::~GlEquilateralTriangle() noexcept
{
    glDeleteBuffers(1, &m_colorVbo);
    glDeleteBuffers(1, &m_vertexVbo);
    glDeleteVertexArrays(1, &m_vao);
}

void GlEquilateralTriangle::Paint(glm::mat4& transform) const noexcept
{
    glBindVertexArray(m_vao);
    glUniformMatrix4fv(m_transform, 1, GL_FALSE, glm::value_ptr(transform));
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);           // unbind the VAO
}