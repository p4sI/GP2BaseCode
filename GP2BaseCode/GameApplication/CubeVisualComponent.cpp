#include "CubeVisualComponent.h"
#include "../D3D10Renderer/Vertex.h"
#include "../D3D10Renderer/D3D10Renderer.h"

bool CubeVisualComponent::create(IRenderer * pRenderer)
{
	m_iNoVerts=8;
	Vertex verts[8];
	verts[0].position=XMFLOAT3(-0.5f,-0.5f,0.5f);
	verts[0].normal=XMFLOAT3(-0.25f,-0.25f,0.5f);
	verts[0].textureCoords=XMFLOAT2(0.0f,1.0f);

	verts[1].position=XMFLOAT3(-0.5f,0.5f,0.5f);
	verts[1].normal=XMFLOAT3(-0.25f,0.25f,0.5f);
	verts[1].textureCoords=XMFLOAT2(0.0f,0.0f);
	
	verts[2].position=XMFLOAT3(0.5f,-0.5f,0.5f);
	verts[2].normal=XMFLOAT3(0.25f,-0.25f,0.5f);
	verts[2].textureCoords=XMFLOAT2(1.0f,1.0f);

	verts[3].position=XMFLOAT3(0.5f,0.5f,0.5f);
	verts[3].normal=XMFLOAT3(0.25f,0.25f,0.5f);
	verts[3].textureCoords=XMFLOAT2(1.0f,0.0f);
	
	verts[4].position=XMFLOAT3(-0.5f,-0.5f,-0.5f);
	verts[4].normal=XMFLOAT3(-0.25f,-0.25f,-0.5f);
	verts[4].textureCoords=XMFLOAT2(0.0f,1.0f);
	
	verts[5].position=XMFLOAT3(-0.5f,0.5f,-0.5f);
	verts[5].normal=XMFLOAT3(-0.25f,0.25f,-0.5f);
	verts[5].textureCoords=XMFLOAT2(0.0f,0.0f);

	verts[6].position=XMFLOAT3(0.5f,-0.5f,-0.5f);
	verts[6].normal=XMFLOAT3(0.25f,-0.25f,-0.5f);
	verts[6].textureCoords=XMFLOAT2(1.0f,1.0f);
	
	verts[7].position=XMFLOAT3(0.5f,0.5f,-0.5f);
	verts[7].normal=XMFLOAT3(0.25f,0.25f,-0.5f);
	verts[7].textureCoords=XMFLOAT2(1.0f,0.0f);


	int indices[]={0,1,2,1,3,2,		//front 
					4,5,6,5,7,6,	//back
					4,5,0,1,0,5,
					2,3,7,6,7,2,
					1,5,7,7,3,1,
					0,4,2,4,6,2
					};
	m_iNoIndices=36;

	D3D10Renderer *pD3D10Renderer=static_cast<D3D10Renderer*>(pRenderer);

	m_pVertexBuffer=pD3D10Renderer->createVertexBuffer(m_iNoVerts,verts);
	m_pIndexBuffer=pD3D10Renderer->createIndexBuffer(m_iNoIndices,indices);
	

	return true;
}