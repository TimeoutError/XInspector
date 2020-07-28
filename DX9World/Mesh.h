#ifndef __MESH_H__
#define __MESH_H__

#include <d3dx9.h>
#include "Clean.h"

class cMesh
{
private:
	ID3DXMesh			*m_Mesh;
	DWORD				 m_numMaterials;
	IDirect3DDevice9	*m_Device;
	D3DMATERIAL9		*m_fMaterials;
	LPDIRECT3DTEXTURE9	*m_fTextures;
	bool				 m_Loaded;

public:
	cMesh()
	{
		m_Mesh = NULL;
	}

	~cMesh()
	{
		End();
	}

	const bool LoadFile(IDirect3DDevice9 *, const char *);
	const bool isLoaded() const;
	void  Draw() const;
	void  End();
};

#endif