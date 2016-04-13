#pragma once

#include <memory>
#include <chrono>
#include "wx/glcanvas.h"

class GlCircle;                     // forward declaration
class GlEquilateralTriangle;        // forward declaration

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
	void CreateCircles();
    void CreateTriangles();
	void BuildCircleShaderProgram();
	void BuildCircleVertexShader();
	void BuildCircleFragmentShader();
    void BuildEquilatoralTriangleShaderProgram();
    void BuildEquilatoralTriangleVertexShader();
    void BuildEquilatoralTriangleFragmentShader();
    void CheckShaderCompileStatus(GLuint shader, const std::string& msg) const;
	void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);

    static const int INTERVAL = 1000 / 60;
    static const int TIMERNUMBER = 3;
    std::unique_ptr<wxTimer> m_timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;

	std::unique_ptr<wxGLContext> m_context;
	// circle stuff
    std::unique_ptr<GlCircle> m_circle1;
    std::unique_ptr<GlCircle> m_circle2;
	GLuint m_circleVertexShader;
	GLuint m_circleFragmentShader;
	GLuint m_circleShaderProgram;

    // triangle stuff
    std::unique_ptr<GlEquilateralTriangle> m_triangle1;
    GLuint m_triangleVertexShader;
    GLuint m_triangleFragmentShader;
    GLuint m_triangleShaderProgram;
};

