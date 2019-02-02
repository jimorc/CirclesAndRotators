#include "wx/wxprec.h"
#include <GL/glew.h>
#include "CirclesAndRotatorsCanvas.h"

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

	SetCurrent(*m_context);
	InitializeGLEW();
	SetupGraphics();
}


CirclesAndRotatorsCanvas::~CirclesAndRotatorsCanvas()
{
	// clean up GPU resources
	SetCurrent(*m_context);
	glDeleteProgram(m_circleShaderProgram);
	glDeleteShader(m_circleFragmentShader);
	glDeleteShader(m_circleVertexShader);

	glDeleteBuffers(1, &m_circleVbo);
	glDeleteBuffers(1, &m_circleEbo);
	glDeleteVertexArrays(1, &m_circleVao);
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
	CreateSquareForCircle();
	BuildCircleShaderProgram();
}

void CirclesAndRotatorsCanvas::OnPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);
	// set background to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use the circleShaderProgram
	glUseProgram(m_circleShaderProgram);
	// set outer radius for circle here. We will be modulating it in later
	// example
	glUniform1f(m_circleOuterRadius, 0.2f);
	// draw the square that will contain the circle.
	// The circle is created inside the square in the circle fragment shader
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glFlush();
	SwapBuffers();
}

void CirclesAndRotatorsCanvas::CreateSquareForCircle()
{
	// define vertices for the two triangles
	float points[] = {
		-0.2f, -0.2f,
		0.2f, -0.2f,
		0.2f, 0.2f,
		-0.2f, 0.2f
	};
	// define the indices for the triangles
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	// setup vertex array object
	glGenVertexArrays(1, &m_circleVao);
	glBindVertexArray(m_circleVao);
	// upload vertex data
	glGenBuffers(1, &m_circleVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_circleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// upload element data
	glGenBuffers(1, &m_circleEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_circleEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
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

	// set up position attribute used in circle vertex shader
	GLint posAttrib = glGetAttribLocation(m_circleShaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	// set up the uniform arguments
	m_circleOuterRadius = glGetUniformLocation(m_circleShaderProgram, "outerRadius");
	m_viewDimensions = glGetUniformLocation(m_circleShaderProgram, "viewDimensions");
	// The canvas size is fixed (and should be square), so initialize the value here
	glUseProgram(m_circleShaderProgram);
	wxSize canvasSize = GetSize();
	glUniform2f(m_viewDimensions, static_cast<float>(canvasSize.x),
		static_cast<float>(canvasSize.y));
}

void CirclesAndRotatorsCanvas::BuildCircleVertexShader()
{
	const GLchar* vertexSource =
		"#version 330 core\n"
		"in vec2 position;"
		"void main()"
		"{"
		"    gl_Position = vec4(position, 0.0, 1.0);"
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
		"out vec4 outColor;"
		"void main()"
		"{"
		// convert fragment coordinate (i.e. pixel) to view coordinate
		"   float x = (gl_FragCoord.x - viewDimensions.x / 2.0f) / (viewDimensions.x / 2.0f);"
		"   float y = (gl_FragCoord.y - viewDimensions.y / 2.0f) / (viewDimensions.y / 2.0f);"
		// discard fragment if outside the circle
		"   float len = sqrt(x * x + y * y);"
		"	if (len > outerRadius) {"
		"		discard;"
		"	}"
		// else set its colour to green
		"	outColor = vec4(0.0, 1.0, 0.0, 1.0);"
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
