#include "DXWindow.h"

bool	UpdateCam, UpdateWnd, isActive;
float	ryFactor, rxFactor, yAngle = 0.0f, xAngle = 0.0f,
		mSpeed = 0.3f,
		sFactor = 1.0f;

D3DXVECTOR3 Position(0.0f, 0.0f, -5.0f),
			At(0.0f, 0.0f, 0.0f),
			Up(0.0f, 1.0f, 0.0f);

USHORT	x, y;
int		CentX, CentY, CurrX, CurrY;
RECT    WndR;
SHORT	MDelta;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (UpdateWnd)
	{
		GetWindowRect(hWnd, &WndR);
		unsigned int Width = WndR.right - WndR.left,
					 Height = WndR.bottom - WndR.top;

		CentX = static_cast<int>(WndR.left + (Width * 0.5f));
		CentY = static_cast<int>(WndR.top + (Height * 0.5f));

		ryFactor = static_cast<float>(((2.0f * PI) / Width));	// 360° Angle
		rxFactor = static_cast<float>(((PI)		   / Height));	// 180° Angle

		ClipCursor(&WndR);
		UpdateWnd = false;
	}

	switch (Message)
	{
	case WM_CREATE:
	case WM_WINDOWPOSCHANGED:
	case WM_SIZE:
		UpdateWnd = true;
		break;
	case WM_MOUSEMOVE:
		CurrX = GET_X_LPARAM(lParam);
		CurrY = GET_Y_LPARAM(lParam);
		break;
	case WM_ACTIVATE:
		isActive = LOWORD(wParam) != 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		case 'W':
			Position.x += mSpeed * sinf(yAngle);
			Position.z += mSpeed * cosf(yAngle);
			Position.y += mSpeed * -sinf(xAngle);
			UpdateCam = true;
			break;
		case 'S':
			Position.x -= mSpeed * sinf(yAngle);
			Position.z -= mSpeed * cosf(yAngle);
			Position.y += mSpeed * sinf(xAngle);
			UpdateCam = true;
			break;
		case 'Q':
			Position.x -= mSpeed * sinf(static_cast<float>(yAngle + PI * 0.5f));
			Position.z -= mSpeed * cosf(static_cast<float>(yAngle + PI * 0.5f));
			UpdateCam = true;
			break;
		case 'D':
			Position.x -= mSpeed * sinf(static_cast<float>(yAngle - PI * 0.5f));
			Position.z -= mSpeed * cosf(static_cast<float>(yAngle - PI * 0.5f));
			UpdateCam = true;
		default:
			break;
		}
		break;
	case WM_MOUSEWHEEL:
		MDelta	 = GET_WHEEL_DELTA_WPARAM(wParam);
		sFactor += ((MDelta > 0) ? 1 : -1) * 0.01f;
		if (sFactor < 0.01f)
			sFactor = 0.01f;
		if (sFactor > 1.0f)
			sFactor = 1.0f;

		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return 0;
}

bool cDXWindow::Prepare(const std::string &wName)
{
	if (! pClass(WndProc) || !pWindow(WS_OVERLAPPEDWINDOW, wName))
		return false;

	m_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_Interface)
	{
		D3DPRESENT_PARAMETERS DP;
		ZeroMemory(&DP, sizeof DP);
		DP.BackBufferCount = 1;
		DP.BackBufferWidth = m_Width;
		DP.BackBufferHeight = m_Height;
		DP.BackBufferFormat = D3DFMT_R5G6B5;
		DP.Windowed = true;
		DP.hDeviceWindow = m_Window;
		DP.EnableAutoDepthStencil = true;
		DP.AutoDepthStencilFormat = D3DFMT_D24S8;
		DP.MultiSampleType = D3DMULTISAMPLE_NONE;
		DP.MultiSampleQuality = 0;
		DP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		DP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		DP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

		if (FAILED(
			m_Interface->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_Window,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&DP,
			&m_Device)
			))
			return false;

		return true;
	}

	return false;
}

void cDXWindow::Setup()
{
	D3DXMATRIX	View;

	D3DXMatrixLookAtLH(&View,
		&Position,
		&At,
		&Up);
	m_Device->SetTransform(D3DTS_VIEW, &View);

	D3DXMATRIX Projection;

	D3DXMatrixPerspectiveFovLH(&Projection,
		static_cast<float>(PI * 0.5),
		static_cast<float>(m_Width) / static_cast<float>(m_Height),
		0.01f,
		400.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &Projection);

	D3DXCreateTeapot(m_Device,
	&m_Teapot,
	NULL);

	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof Light);
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	Light.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	m_Device->SetLight(0, &Light);
	m_Device->LightEnable(0, true);
	m_Device->SetRenderState(D3DRS_LIGHTING, true);

	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof Material);
	Material.Diffuse = Material.Ambient =
	Material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	Material.Power = 2.0f;
	m_Device->SetMaterial(&Material);
}

void cDXWindow::Update()
{
	D3DXMATRIX View;
	bool CursorDiff = (CurrX != CentX || CurrY != CentY) && isActive;

	if (UpdateCam || CursorDiff)
	{
		if (CursorDiff)
		{
			yAngle = (CurrX - CentX) * ryFactor;
			xAngle = (CurrY - CentY) * rxFactor;
		}

		At.x = Position.x + sin(yAngle);
		At.y = Position.y - sin(xAngle);
		At.z = Position.z + cos(yAngle);

		D3DXMatrixLookAtLH(&View, &Position, &At, &Up);
		m_Device->SetTransform(D3DTS_VIEW, &View);

		UpdateCam = false;
	}
}

void cDXWindow::Run()
{
	MSG Message;
	do
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);

			if(Message.message != WM_COMMAND)
				DispatchMessage(&Message);
			else
			{
				switch (LOWORD(Message.wParam))
				{
				case ID_SOLID:
					m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
					break;
				case ID_WIREFRAME:
					m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
					break;
				case ID_LOADMESH:
				{
					OPENFILENAME OFN;
					char		 cFile[MAX_PATH] = { 0 };

					ZeroMemory(&OFN, sizeof(OFN));
					OFN.lStructSize = sizeof OFN;
					OFN.lpstrFilter = ".X Files (*.x)\0*.x\0";
					OFN.lpstrFile	= cFile;
					OFN.nMaxFile	= sizeof cFile;
					OFN.Flags		= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
					OFN.hwndOwner	= m_Window;

					if (GetOpenFileName(&OFN))
					{
						m_Custom.End();
						m_Custom.LoadFile(m_Device, cFile);
					}
				}
				default:
					break;
				}
			}
		}
		else
		{
			Update();
			Render();
		}
	} while (Message.message != WM_QUIT);

}

void cDXWindow::Render() const
{
	m_Device->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0,
		0);

	m_Device->BeginScene();

		if (m_Custom.isLoaded())
		{
			D3DXMATRIX ScaleMatrix;
			D3DXMatrixScaling(&ScaleMatrix, sFactor, sFactor, sFactor);
			m_Device->SetTransform(D3DTS_WORLD, &ScaleMatrix);

			m_Custom.Draw();
		}
		else
		{
			m_Teapot->DrawSubset(0);
		}

	m_Device->EndScene();

	m_Device->Present(0, 0, 0, 0);
}