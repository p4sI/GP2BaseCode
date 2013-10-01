#pragma once

//The header file for the renderer interface
#include "../Renderer/Renderer.h"
#include <Windows.h>

//forward declarations of the D3D10 interfaces
struct ID3D10Device;
struct IDXGISwapChain;
struct ID3D10RenderTargetView;
struct ID3D10DepthStencilView;
struct ID3D10Texture2D;
struct ID3D10Effect;
struct ID3D10Buffer;
struct ID3D10InputLayer;
struct ID3D10EffectTechnique;

//D3D10Renderer implements the Renderer interface
class D3D10Renderer:public IRenderer
{
public:
	//Contructor
	D3D10Renderer();
	//Deconstructor
	~D3D10Renderer();

	//notice these have the same signature as the pure methods
	bool init(void *pWindowHandle, bool fullScreen);
	void clear(float r, float g, float b, float a);
	void present();
	void render();

private:
	bool createDevice(HWND pWindowHandle, int windowWidth, int windowHeight,
			bool fullScreen);
	bool createInitialRenderTarget(int windowWidth, int windowHeight);
	bool loadEffectFromMemory(const char* pMem);
	bool createBuffer();
	bool createVertexLayout();
private:
	//D3D10 stuff
	ID3D10Device *m_pD3D10Device;
	IDXGISwapChain *m_pSwapChain;
	ID3D10RenderTargetView *m_pRenderTargetView;
	ID3D10DepthStencilView *m_pDepthStencilView;  //stencil or stencEl ?!?!
	ID3D10Texture2D *m_pDepthStencilTexture;
	ID3D10Effect *m_pTempEffect;
	ID3D10Buffer *m_pTempTechnique;
	ID3D10InputLayer *m_pTempBuffer;
	ID3D10EffectTechnique *m_pTempVertexLayout;
};
