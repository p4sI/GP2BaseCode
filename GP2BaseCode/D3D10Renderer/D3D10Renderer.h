#pragma once

//http://www.gamedev.net/topic/506017-quick-question-about-id3d10inputlayout/
//The header file for the renderer interface
#include "../Renderer/Renderer.h"

#include <Windows.h>

#include <D3D10.h>
#include <D3DX10.h>

#define _XM_NO_INTRINSICS_
#include <xnamath.h>

#include "Vertex.h"


//D3D10Renderer implements the Renderer interface
class D3D10Renderer:public IRenderer
{
public:
	D3D10Renderer();
	~D3D10Renderer();
	//notice these have the same signature as the pure methods
	//in the interface
	bool init(void *pWindowHandle,bool fullScreen);
	void clear(float r,float g,float b,float a);
	void present();
	void render();

	ID3D10Effect * loadEffectFromMemory(const char *pMem);
	ID3D10Effect * loadEffectFromFile(const char *pFilename);

	ID3D10Buffer * createVertexBuffer(int size,Vertex *pVerts);
	ID3D10Buffer * createIndexBuffer(int size,int *pIndices);
private:
	bool createDevice(HWND pWindowHandle,int windowWidth, int windowHeight,
bool fullScreen);
	bool createInitialRenderTarget(int windowWidth, int windowHeight);

	////temp for the exercise
	//bool loadEffectFromMemory(const char *pMem);
	//bool loadEffectFromFile(const char *pFilename);
	//bool createBuffer();
	bool createVertexLayout();
	//void createCamera(XMVECTOR &position, XMVECTOR &focu,XMVECTOR &up,
	//	float fov,float aspectRatio,float nearClip,float farClip);
	//void setSquarePosition(float x,float y,float z);
private:
	//D3D10 stuff
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;

	//Vertex Layout
	ID3D10InputLayout*      m_pDefaultVertexLayout;
	//this will be used if we have no Effect
	ID3D10Effect * m_pDefaultEffect;

	//Effect Variables
	ID3D10EffectMatrixVariable * m_pWorldEffectVariable;
	ID3D10EffectMatrixVariable * m_pProjectionEffectVariable;
	ID3D10EffectMatrixVariable * m_pViewEffectVariable;
};