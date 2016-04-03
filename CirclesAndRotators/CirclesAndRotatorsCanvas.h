#pragma once

#include <memory>
#include "wx/glcanvas.h"

class CirclesAndRotatorsCanvas : public wxGLCanvas
{
public:
	CirclesAndRotatorsCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
		const int* attribList = 0, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0L,
		const wxString& name = L"GLCanvas",
		const wxPalette& palette = wxNullPalette);

	virtual ~CirclesAndRotatorsCanvas();
	CirclesAndRotatorsCanvas(const CirclesAndRotatorsCanvas& canvas) = delete;
	CirclesAndRotatorsCanvas(CirclesAndRotatorsCanvas&& canvas) = delete;
	CirclesAndRotatorsCanvas& operator=(const CirclesAndRotatorsCanvas& canvas) = delete;
	CirclesAndRotatorsCanvas& operator=(CirclesAndRotatorsCanvas&& canvas) = delete;

private:
	void InitializeGLEW();
	void SetupGraphics();
	void CreateSquareForCircle();
	void BuildCircleShaderProgram();
	void BuildCircleVertexShader();
	void BuildCircleFragmentShader();
	void CheckShaderCompileStatus(GLuint shader, const std::string& msg) const;
	void OnPaint(wxPaintEvent& event);

	std::unique_ptr<wxGLContext> m_context;
	// circle stuff
	GLuint m_circleVbo;
	GLuint m_circleVao;
	GLuint m_circleEbo;
	GLuint m_circleVertexShader;
	GLuint m_circleFragmentShader;
	GLuint m_circleShaderProgram;
	GLint m_circleOuterRadius;
	GLint m_viewDimensions;
    GLint m_transform;
};

