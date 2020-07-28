#include "Window.h"

std::string g_ClassName = "WorldAppClass";

bool cWindow::m_Class = false;

const bool cWindow::pClass(WNDPROC Pump)
{
	if (! m_Class)
	{
		WNDCLASSEX wClass;
		ZeroMemory(&wClass, sizeof wClass);
		wClass.cbSize	= sizeof(wClass);
		wClass.hCursor	= LoadCursor(NULL, IDC_NO);
		wClass.hIconSm	= LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
		wClass.hIcon	= LoadIcon(NULL, IDI_SHIELD);
		wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wClass.hInstance = m_Instance;
		wClass.style	= CS_VREDRAW | CS_HREDRAW;
		wClass.lpszClassName = g_ClassName.c_str();
		wClass.lpfnWndProc = Pump;

		if (FAILED(RegisterClassEx(&wClass)))
			return false;

		m_Class = true;
	}

	return true;
}

const bool cWindow::pWindow(const DWORD dwStyle, const std::string &wName)
{
	m_WindowName.assign(wName);

	m_Window = CreateWindow(g_ClassName.c_str(), m_WindowName.c_str(),
		dwStyle, 0, 0,
		m_Width, m_Height, NULL,
		NULL, m_Instance,
		NULL);

	if (m_Window)
	{
		addMenu();
		ShowWindow(m_Window, SW_SHOW);
		return true;
	}

	return false;
}

void cWindow::updWindowName(const std::string &nWName)
{
	if (m_Window && m_WindowName.compare(nWName))
	{
		m_WindowName.assign(nWName);
		SetWindowText(m_Window, m_WindowName.c_str());
	}
}

void cWindow::addMenu()
{

	HMENU Menu = CreateMenu(),
		  File = CreateMenu(),
		  Fill = CreateMenu();

	AppendMenu(Menu, MF_POPUP, (UINT_PTR) File, "File");
	AppendMenu(File, MF_STRING, ID_LOADMESH,	"Load Mesh");
	AppendMenu(Menu, MF_POPUP, (UINT_PTR) Fill,	"Fill mode");
	AppendMenu(Fill, MF_STRING, ID_SOLID,		"Solid");
	AppendMenu(Fill, MF_STRING, ID_WIREFRAME,	"Wireframe");

	SetMenu(m_Window, Menu);
}

const std::string &cWindow::getTitle()
{
	return m_WindowName;
}