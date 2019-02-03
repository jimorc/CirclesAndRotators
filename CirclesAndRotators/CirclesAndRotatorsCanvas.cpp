#include "wx/wxprec.h"
#include <GL/glew.h>
#define GLM_FORCE_CXX17
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CirclesAndRotatorsCanvas.h"
#include "glCircle.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")

CirclesAndRotatorsCanvas::CirclesAndRotatorsCanvas(wxWindow* parent, wxWindowID id,
	const int* attribList, const wxPoint& pos,
	const wxSize& size, long style,
	const wxString& name,
	const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
{ 
	m_context = std::make_unique<wxGLContext>(this);
	Bind(wxEVT_PAINT, &CirclesAndRotatorsCanvas::OnPaint, this);
    Bind(wxEVT_TIMER, &CirclesAndRotatorsCanvas::OnTimer, this);

    m_startTime = std::chrono::high_resolution_clock::now();
    m_timer = std::make_unique<wxTimer>(this, TIMERNUMBER);
    m_timer->Start(INTERVAL);

	SetCurrent(*m_context);
	InitializeGLEW();
	SetupGraphics();
}


CirclesAndRotatorsCanvas::~CirclesAndRotatorsCanvas()
{
    m_timer->Stop();
	// clean up GPU resources
	SetCurrent(*m_context);
	glDeleteProgram(m_circleShaderProgram);
	glDeleteShader(m_circleFragmentShader);
	glDeleteShader(m_circleVertexShader);
}

void CirclesAndRotatorsCanvas::InitializeGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		const GLubyte* msg = glewGetErrorString(err);
		throw std::exception(reinterpret_cast<const char*>(msg));
	}
}

void CirclesAndRotatorsCanvas::SetupGraphics()
{
	BuildCircleShaderProgram();
    CreateCircles();
}

void CirclesAndRotatorsCanvas::OnPaint(wxPaintEvent& event)
{
	// set background to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use the circleShaderProgram
	glUseProgram(m_circleShaderProgram);
    // set the transform
    wxSize canvasSize = GetSize();
    float w = static_cast<float>(canvasSize.x) / 2.0f;
    glm::mat4 transform(1);
    transform = glm::translate(transform, glm::vec3(220.0f / w, -150.0f / w, 0.0f / w));
    glm::mat4 rotation;
    auto t_now = std::chrono::high_resolution_clock::now();
    auto time = (t_now - m_startTime).count();
    rotation = glm::rotate(rotation, time * 2.5e-10f, glm::vec3(0.0f, 0.0f, 1.0f));    glm::mat4 transrotate;
    transrotate = rotation * transform;
    m_circle1->Paint(transrotate, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    transform = glm::mat4();
    transform = glm::translate(transform, glm::vec3(-200.0f / w, -75.0f / w, 0.0f / w));
    rotation = glm::rotate(rotation, time * 5e-10f, glm::vec3(0.0f, 0.0f, 1.0f));
    transrotate = rotation * transform;
    m_circle2->Paint(transrotate, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	glFlush();
	SwapBuffers();
}

void CirclesAndRotatorsCanvas::OnTimer(wxTimerEvent& event)
{
    ProcessEvent(wxPaintEvent());
}


void CirclesAndRotatorsCanvas::CreateCircles()
{
	// define vertices for the two triangles
    wxSize canvasSize = GetSize();
    float w = static_cast<float>(canvasSize.x) / 2.0f; 
    m_circle1 = std::make_unique<GlCircle>(80.0f, w, m_circleShaderProgram);
    m_circle2 = std::make_unique<GlCircle>(30.0f, w, m_circleShaderProgram);
}

void CirclesAndRotatorsCanvas::BuildCircleShaderProgram()
{
	// build the circle shaders
	BuildCircleVertexShader();
	BuildCircleFragmentShader();
	// create and link circle shader program
	m_circleShaderProgram = glCreateProgram();
	glAttachShader(m_circleShaderProgram, m_circleVertexShader);
	glAttachShader(m_circleShaderProgram, m_circleFragmentShader);
	glBindFragDataLocation(m_circleShaderProgram, 0, "outColor");
	glLinkProgram(m_circleShaderProgram);
}

void CirclesAndRotatorsCanvas::BuildCircleVertexShader()
{
    const GLchar* vertexSource =
        "#version 330 core\n"
        "in vec4 position;"
        "uniform mat4 transform;"
		"void main()"
		"{"
		"    gl_Position = transform * position;"
		"}";
	m_circleVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_circleVertexShader, 1, &vertexSource, NULL);
	glCompileShader(m_circleVertexShader);
	CheckShaderCompileStatus(m_circleVertexShader, "Circle Vertex Shader did not compile.");
}

void CirclesAndRotatorsCanvas::BuildCircleFragmentShader()
{
    const GLchar* fragmentSource =
        "#version 330 core\n"
        "uniform vec2 viewDimensions;"
        "uniform float outerRadius;"
        "uniform mat4 transform;"
        "uniform vec4 color;"
        "out vec4 outColor;"
        "void main()"
        "{"
        // transform the center of the circle. We need this later to determine if the
        // fragment is inside or outside the circle.
        "   vec4 center = transform * vec4(0.0f, 0.0f, 0.0f, viewDimensions.x / 2.0f);"
        // translate fragment coordinate to coordinate relative to center of circle
        "   float x = gl_FragCoord.x - center.x - viewDimensions.x / 2.0f;"
        "   float y = gl_FragCoord.y - center.y - viewDimensions.y / 2.0f;"
        // discard fragment if outside the circle
        "   float len = sqrt(x * x + y * y);"
        "	if (len > outerRadius) {"
        "		discard;"
        "	}"
        // else set its colour to green
        "	outColor = color;"
		"}";
	m_circleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_circleFragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(m_circleFragmentShader);
	CheckShaderCompileStatus(m_circleFragmentShader, "Circle Fragment Shader did not compile");
}

void CirclesAndRotatorsCanvas::CheckShaderCompileStatus(GLuint shader, const std::string& msg) const
{
	// check shader compile status, and throw exception if compile failed
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		throw std::exception(msg.c_str());
	}
}
