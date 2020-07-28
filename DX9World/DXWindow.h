#ifndef __DXWINDOW_H__
#define __DXWINDOW_H__

#include "Window.h"
#include "Mesh.h"
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define PI 3.1415

class cDXWindow : public cWindow
{
private:
	IDirect3D9	*m_Interface;
	IDirect3DDevice9 *m_Device;
	ID3DXMesh	*m_Teapot;
	cMesh		 m_Custom;

public:

	cDXWindow(HINSTANCE Instance, UINT Width, UINT Height)
	: cWindow(Instance, Width, Height)
	{
	}

	virtual ~cDXWindow()
	{
		SAFE_RELEASE(m_Teapot);
		SAFE_RELEASE(m_Device);
		SAFE_RELEASE(m_Interface);
	}

	bool Prepare(const std::string &);
	void Setup();
	void Update();
	void Render() const;
	void Run();
};

#endif