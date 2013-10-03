#include "D3D10Renderer.h"

#include <D3D10.h>
#include <D3DX10.h>

struct Vertex
{
	float x,y,z;
};

const char basicEffect[]=\
	"float4 VS( float4 Pos : POSITION ) : SV_POSITION"\
	"{"\
	"	return Pos;"\
	"}"\
	"float4 PS( float4 Pos : SV_POSITION ) : SV_Target"\
	"{"\
	"	return float4( 1.0f, 1.0f, 0.0f, 1.0f );"\
	"}"\
	"technique10 Render"\
	"{"\
	"	pass P0"\
	"	{"\
	"		SetVertexShader( CompileShader( vs_4_0, VS() ) );"\
	"		SetGeometryShader( NULL );"\
	"		SetPixelShader( CompileShader( ps_4_0, PS() ) );"\
	"	}"\
	"}";



// Constructor (sets every variables to NULL)
D3D10Renderer::D3D10Renderer()
{
	m_pD3D10Device = NULL;
	m_pRenderTargetView = NULL;
	m_pSwapChain = NULL;
	m_pDepthStencilTexture = NULL;
	m_pDepthStencilView = NULL;
	m_pTempEffect = NULL;
	m_pTempTechnique = NULL;
	m_pTempBuffer = NULL;
	m_pTempVertexLayout = NULL;
}

//Deconstructor (Releases each of the Direct3D10 interfaces)
D3D10Renderer::~D3D10Renderer()
{
	if(m_pD3D10Device)
		m_pD3D10Device->ClearState();
	
	if(m_pTempBuffer)
		m_pTempBuffer->Release();
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
	if(m_pTempEffect)
		m_pTempEffect->Release();
}

/* 
   This function calculates the actual size of the window to be drawn(this is less than the size of window). 
   Once this size of the drawing area is calculate, the function calls the createDevice 
   and createInitialRenderTarget functions which are contained in the D3D10Renderer class 
*/
bool D3D10Renderer::init(void *pWindowHandle, bool fullScreen)
{
	HWND window = (HWND)pWindowHandle;
	RECT windowRect;

	/*
	GetClientRect () Retrieves the coordinates of a window's client area. The client coordinates specify the 
	upper-left and lower-right corners of the client area. Because client coordinates are relative to the 
	upper-left corner of a window's client area, the coordinates of the upper-left corner are (0,0).
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms633503%28v=vs.85%29.aspx
	*/
	GetClientRect(window, &windowRect);

	UINT width = windowRect.right - windowRect.left;
	UINT height = windowRect.bottom - windowRect.top;

	if(!createDevice(window, width, height, fullScreen))
		return false;
	if(!createInitialRenderTarget(width, height))
		return false;
	if(!loadEffectFromMemory(basicEffect)) // what input??
		return false;
	if(!createVertexLayout())
		return false;
	if(!createBuffer())
		return false;
	

	return true;
}

/* 
   This function simply creates the device to interface with the graphics hardware 
   and also a swapchain which holds a series of drawing buffers 
*/
bool D3D10Renderer::createDevice(HWND window, int windowWidth, int windowHeight, bool fullScreen)
{
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags|=D3D10_CREATE_DEVICE_DEBUG;
#endif

	/* DXGI_SWAP_CHAIN_DESC describes a swap chain
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb173075%28v=vs.85%29.aspx
	*/
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	if(fullScreen)
		sd.BufferCount = 2;
	else
		sd.BufferCount = 1;

	sd.OutputWindow = window;
	sd.Windowed = (BOOL)(!fullScreen);
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferDesc.Width = windowWidth;
	sd.BufferDesc.Height = windowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	
	/* D3D10CreateDeviceAndSwapChain()
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb205087%28v=vs.85%29.aspx
	*/
	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, 
					D3D10_DRIVER_TYPE_HARDWARE,
					NULL,
					createDeviceFlags,
					D3D10_SDK_VERSION,
					&sd,
					&m_pSwapChain,
					&m_pD3D10Device)))
		return false;
	
	return true;

}

/* 
   This function grabs the backbuffer from the swap chain 
   and then creates a depth stencil texture.
   This function also creates the DepthStencilView and the RenderTargetView, 
   binds them both to the pipeline and then finally creates a viewport; 
   which is used in the pipeline for some of the transformations. 
   The reason why we need to create views is that D3D10 doesn’t directly render to the buffers 
   it requires views as intermediary between the device and the actual surface, 
   this helps to protect the video drivers from a crash and also the integrity of video memory. 
*/
bool D3D10Renderer::createInitialRenderTarget(int windowWidth, int windowHeight)
{
	/* ID3D10Texture2D: A 2D texture interface manages texel data, which is structured memory.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb173867%28v=vs.85%29.aspx
	*/
	ID3D10Texture2D *pBackBuffer;

	/* GetBuffer() Accesses one of the swap-chain's back buffers.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb174570%28v=vs.85%29.aspx
	*/
	if (FAILED(m_pSwapChain->GetBuffer(0, 
								__uuidof(ID3D10Texture2D), 
								(void**)&pBackBuffer)))
		return false;

	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = windowWidth;
	descDepth.Height = windowHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	/* CreateTexture2D creates an array of 2D textures.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb173560%28v=vs.85%29.aspx

	descDepth is type of D3D10_TEXTURE2D_DESC: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172480%28v=vs.85%29.aspx
	*/
	if (FAILED(m_pD3D10Device->CreateTexture2D(&descDepth, 
									NULL, 
									&m_pDepthStencilTexture)))
		return false;

	/* D3D10_DEPTH_STENCIL_VIEW_DESC 
	Specifies the subresource(s) from a texture that are accessible using a depth-stencil view.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb205037%28v=vs.85%29.aspx
	*/
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	/*ID3D10Device::CreateDepthStencilView method
	Create a depth-stencil view for accessing resource data.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb173547%28v=vs.85%29.aspx
	*/
	if (FAILED(m_pD3D10Device->CreateDepthStencilView(m_pDepthStencilTexture,
									&descDSV, 
									&m_pDepthStencilView)))
		return false;

	if (FAILED(m_pD3D10Device->CreateRenderTargetView(pBackBuffer,
									NULL,
									&m_pRenderTargetView)))
	{
		pBackBuffer->Release();
		return false;
	}
	pBackBuffer->Release();

	/*ID3D10Device::OMSetRenderTargets method
	Bind one or more render targets and the depth-stencil buffer to the output-merger stage.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb173597%28v=vs.85%29.aspx
	*/
	m_pD3D10Device->OMSetRenderTargets(1,
		&m_pRenderTargetView,
		m_pDepthStencilView);

	/*D3D10_VIEWPORT structure
	Defines the dimensions of a viewport.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb172500%28v=vs.85%29.aspx
	*/
	D3D10_VIEWPORT vp;
	vp.Width = windowWidth;
	vp.Height = windowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	/*ID3D10Device::RSSetViewports method
	Bind an array of viewports to the rasterizer stage of the pipeline.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb173613%28v=vs.85%29.aspx
	*/
	m_pD3D10Device->RSSetViewports(1, &vp);
	return true;
}

/*
  This function simply clears all buffers (via the views)
  ClearRenderTargetView: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173539%28v=vs.85%29.aspx
  ClearDepthStencilView: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173538%28v=vs.85%29.aspx
*/
void D3D10Renderer::clear(float r, float g, float b, float a)
{
	const float ClearColor[4] = {r, g, b, a};
	m_pD3D10Device->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pD3D10Device->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}

/*
  This function presents the swapchain which flip the back and the front buffers 
  -> an image will appear
*/
void D3D10Renderer::present()
{
	m_pSwapChain->Present(0, 0);
}

void D3D10Renderer::render()
{
	
}

bool D3D10Renderer::loadEffectFromMemory(const char* pMem)
{
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob *pErrorBuffer = NULL;
	if (FAILED(D3DX10CreateEffectFromMemory(
		pMem,				// pointer to the memory which holds the effect
		strlen(pMem),		// the size of the effect in memory
		NULL,
		NULL,
		NULL,
		"fx_4_0",			// effect profile which tells the compiler what shader model to target
		dwShaderFlags,		// shader flags which may add extra information into the compiled effect including debugging information
		0,
		m_pD3D10Device,		// D3D10 Device
		NULL, 
		NULL, 
		&m_pTempEffect,		// hold our Effect object
		&pErrorBuffer,		// hold all error messages
		NULL )))
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
		return false;
	}

	m_pTempTechnique=m_pTempEffect->GetTechniqueByName("Render");	//  retrieve the technique by name.

	return true;
}

bool D3D10Renderer::createBuffer()
{
	Vertex verts[] = {
		{-1.0f, -1.0f, 0.0f},
		{ 0.0f,  1.0f, 0.0f},
		{ 1.0f, -1.0f, 0.0f}
	};
	
	// Defines the propertys of the buffer
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Vertex ) * 3;	// buffer is big enough for 3 vertices
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	// prepare the data to copy it into the buffer
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &verts;

	//buffer creation
	if (FAILED(m_pD3D10Device->CreateBuffer(
			&bd,
			&InitData,
			&m_pTempBuffer )))
	{
		OutputDebugStringA("Cant create buffer");
		
	}

	return true;
}

bool D3D10Renderer::createVertexLayout()
{
	return true;
}