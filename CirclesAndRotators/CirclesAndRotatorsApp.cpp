#include "wx/wxprec.h"
#include "CirclesAndRotatorsApp.h"
#include "CirclesAndRotatorsFrame.h"

#ifdef _UNICODE
#ifdef NDEBUG
#pragma comment(lib, "wxbase31u.lib")
#else
#pragma comment(lib, "wxbase31ud.lib")
#endif
#else
#ifndef NDEBUG
#pragma comment(lib, "wxbase31.lib")
#else
#pragma comment(lib, "wxbase31d.lib")
#endif
#endif



CirclesAndRotatorsApp::CirclesAndRotatorsApp()
{
}


CirclesAndRotatorsApp::~CirclesAndRotatorsApp()
{
}

bool CirclesAndRotatorsApp::OnInit()
{
	try {
		CirclesAndRotatorsFrame* mainFrame = new CirclesAndRotatorsFrame(nullptr, L"Circles and Rotators");
		mainFrame->Show(true);
	}
	catch (std::exception& e) {
		wxMessageBox(e.what(), "CirclesAndRotators");
	}
	return true;
}

wxIMPLEMENT_APP(CirclesAndRotatorsApp);
