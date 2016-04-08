#include "glCircle.h"

GlCircle::GlCircle(float radius, float w, GLuint shaderProgram)
: m_radius(radius), m_w(w)
{
    // create vertices for circle centred at origin
    glm::vec4 vertices[] = {
        { -m_radius, -m_radius, 0.0f, w },
        { m_radius, -m_radius, 0.0f, w },
        { m_radius, m_radius, 0.0f, w },
        { -m_radius, m_radius, 0.0f, w }
    };
    // create elements that specify the vertices.
    GLint elements[6] = { 0, 1, 2, 2, 3, 0 };

    // circle is centred at the origin. Later, in the Paint method, we transform the location to
    // where we want it.
    // VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    // upload vertex data
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW);
    // upload element data
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLint), elements, GL_STATIC_DRAW);

    // set up position attribute used in circle vertex shader
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    // set up the uniform arguments
    m_outerRadius = glGetUniformLocation(shaderProgram, "outerRadius");
    m_viewDimensions = glGetUniformLocation(shaderProgram, "viewDimensions");
    m_transform = glGetUniformLocation(shaderProgram, "transform");
    m_color = glGetUniformLocation(shaderProgram, "color");
    glBindVertexArray(0);           // unbind the VAO
}

GlCircle::~GlCircle() noexcept 
{
    // clean up buffers
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void GlCircle::Paint(glm::mat4& transform, const glm::vec4& color) const noexcept
{
    glBindVertexArray(m_vao);
    glUniformMatrix4fv(m_transform, 1, GL_FALSE, glm::value_ptr(transform));
    // set outer radius for circle here. We will be modulating it in later
    // example
    glUniform1f(m_outerRadius, m_radius);
    glUniform2f(m_viewDimensions, 2.0f * m_w, 2.0f * m_w);
    glUniform4f(m_color, color.r, color.g, color.b, color.a);
    // draw the square that will contain the circle.
    // The circle is created inside the square in the circle fragment shader
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);           // unbind the VAO
}
