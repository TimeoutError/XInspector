#include "Mesh.h"

const bool cMesh::LoadFile(IDirect3DDevice9 *Device, const char *pFile)
{
	ID3DXBuffer		*bMaterials;
	DWORD dwAttrib	= GetFileAttributes(pFile);
	m_Device		= Device;

	if (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{

		if (FAILED(
			D3DXLoadMeshFromX(pFile, D3DXMESH_SYSTEMMEM, m_Device,
			NULL, &bMaterials, NULL,
			&m_numMaterials, &m_Mesh))
			)
			return false;

		m_fMaterials = new D3DMATERIAL9[m_numMaterials];
		m_fTextures  = new LPDIRECT3DTEXTURE9[m_numMaterials];
		
		if (! m_fMaterials || ! m_fTextures)
			return false;

		D3DXMATERIAL *Materials = static_cast<LPD3DXMATERIAL>(bMaterials->GetBufferPointer());

		for (unsigned int Counter = 0; Counter < m_numMaterials; Counter++)

		{
			m_fMaterials[Counter] = Materials[Counter].MatD3D;
			m_fMaterials[Counter].Ambient = m_fMaterials[Counter].Diffuse;

			if (Materials[Counter].pTextureFilename != NULL)
			{
				if (FAILED(
					D3DXCreateTextureFromFile(
					m_Device,
					Materials[Counter].pTextureFilename,
					&m_fTextures[Counter]
					)
					))
				{
					m_fTextures[Counter] = NULL;
				}
			}
		}



		m_Loaded = true;
		return m_Loaded;
	}

	return false;
}

void cMesh::Draw() const
{
	if (m_Loaded)
	{
		D3DMATERIAL9			IMat;
		IDirect3DBaseTexture9	*ITex;
		m_Device->GetMaterial(&IMat);
		m_Device->GetTexture(0, &ITex);

		for (unsigned int Counter = 0; Counter < m_numMaterials; Counter++)
		{
			m_Device->SetMaterial(&m_fMaterials[Counter]);
			m_Device->SetTexture(0, m_fTextures[Counter]);
			m_Mesh->DrawSubset(Counter);
		}

		m_Device->SetTexture(0, ITex);
		m_Device->SetMaterial(&IMat);
	}
}

const bool cMesh::isLoaded() const
{
	return m_Loaded;
}

void cMesh::End()
{
	if (m_fTextures || m_fMaterials)
	{
		for (unsigned int Counter = 0; Counter < m_numMaterials; Counter++)
		{
			SAFE_RELEASE(m_fTextures[Counter]);
		}

		SAFE_DELETE(m_fMaterials);
		SAFE_DELETE(m_fTextures);
	} 

	m_numMaterials = 0;
	m_Loaded = false;
	SAFE_RELEASE(m_Mesh);
}