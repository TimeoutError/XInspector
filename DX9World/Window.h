#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Clean.h"
#include "resource.h"

#include <Windows.h>
#include <Windowsx.h>
#include <timeapi.h>
#include <string>

#pragma comment(lib, "winmm.lib")

#define ID_WIREFRAME	1
#define ID_SOLID		2
#define ID_LOADMESH		3

class cWindow
{
	private:
		HINSTANCE		m_Instance;
		std::string		m_WindowName;
	public:
		unsigned int	m_Width, m_Height;
		HWND			m_Window;
		static bool		m_Class;

		cWindow(HINSTANCE Instance, UINT Width, UINT Height)
		: m_Instance(Instance), m_Width(Width), m_Height(Height)
		{ }

		const bool pClass(WNDPROC);
		const bool pWindow(const DWORD, const std::string &);
		void updWindowName(const std::string &);
		void addMenu();
		const std::string &getTitle();
};


#endif