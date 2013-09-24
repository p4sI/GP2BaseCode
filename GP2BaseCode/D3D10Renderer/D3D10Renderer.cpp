#include "D3D10Renderer.h"

#include <D3D10.h>
#include <D3DX10.h>

// Constructor
D3D10Renderer::D3D10Renderer()
{
	m_pD3D10Device = NULL;
	m_pRendererTargetView = NULL;
	m_pSwapChain = NULL;
	m_pDepthStencilTexture = NULL;
	m_pDepthStencilView = NULL;
}

//Decontrcutor
D3D10Renderer::~D3D10Renderer()
{
	if(m_pD3D10Device)
		m_pD3D10Device->ClearState();

	if(m_pRenderTargetView)
		m_pRenderTargetView->Release();
	if(m_pDepthStencilView)
		m_pDepthStencilView->Release();
	if(m_pDepthStencilTexture)
		m_pDepthStencilTexture->Release();
	if(m_pSwapChain)
		m_pSwapChain->Release();
	if(m_pD3D10Device)
		m_pD3D10Device->Release();
}

bool D3D10Renderer::init(void *pWindowHandle, bool fullScreen)
{
	HWND window = (HWND)pWindowHandle;
	RECT windowRect;
	GetClientRect(window, &windowRect);

	UINT width = windowRect.right - windowRect.left;
	UINT height = windowRect.bottom - windowRect.top;

	if(!createDevice(window, width, height, fullScreen))
		return false;
	if(!createInitialRenderTarget(width, height))
		return false;

	return true;
}