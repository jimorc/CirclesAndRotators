#include "wx/wxprec.h"
#include "CirclesAndRotatorsFrame.h"
#include "CirclesAndRotatorsCanvas.h"

const int crCanvasID = 2000;		// canvas widget ID

CirclesAndRotatorsFrame::CirclesAndRotatorsFrame(wxWindow* parent, const std::wstring& title, const wxPoint& pos,
	const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, L"Circles and Rotators")
{
	CirclesAndRotatorsCanvas* canvas = new CirclesAndRotatorsCanvas(this, crCanvasID, nullptr, { 0, 0 },
	{ 800, 800 });
	Fit();
	Centre();
}


CirclesAndRotatorsFrame::~CirclesAndRotatorsFrame()
{
}

