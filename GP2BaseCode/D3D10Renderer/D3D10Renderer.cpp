#include "D3D10Renderer.h"



struct Vertex
{
	float x,y,z;
	float nx,ny,nz;
};

/* this defines the layout of a single vertex and includes the name of the 
semantic(helps to bind the vertex elements to the vertex elements contained 
in the effect, the format of the element(type and quantity) and the offset from 
the start of the memory location where the element is located inside the vertex stream. 
*/
const D3D10_INPUT_ELEMENT_DESC VertexLayout[] =
{		
    { "POSITION",
	0, 
	DXGI_FORMAT_R32G32B32_FLOAT, 
	0, 
	0, 
	D3D10_INPUT_PER_VERTEX_DATA, 
	0 },
	{ "NORMAL",
	0, 
	DXGI_FORMAT_R32G32B32_FLOAT, 
	0, 
	12, 
	D3D10_INPUT_PER_VERTEX_DATA, 
	0 }

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
	m_View = XMMatrixIdentity();
	m_Projection = XMMatrixIdentity();
	m_World = XMMatrixIdentity();
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
	if(m_pTempVertexLayout)
		m_pTempVertexLayout->Release();
	if(m_pTempIndexBuffer)
		m_pTempIndexBuffer->Release();
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
	if(!loadEffectFromFile(TEXT("Effects/specular.fx")))
		return false;
	if(!createVertexLayout())
		return false;
	if(!createBuffer())
		return false;

	XMFLOAT3 cameraPos = XMFLOAT3(0.0f, 3.0f, -10.0f);	// camera Pos: x = 0, y = 0, z = -10
	XMFLOAT3 focusPos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// look at 0/0/0
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);			// y-axis goes up
	//ambient
	m_ambientMaterial = XMFLOAT4(0.4f, 0.0f, 0.0f, 1.0f);
	m_ambientLightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//diffuse
	m_diffuseMaterial = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_diffuseLightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightDirection = XMFLOAT3(0.0f, -0.5f, -0.5f);
	//specular
	m_specularMaterial = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_specularLightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_specularPower = 30.0f;
	m_CameraPos = XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0);

	createCamera(XMLoadFloat3(&cameraPos), 
				XMLoadFloat3(&focusPos), 
				XMLoadFloat3(&up),
				XM_PI/4,								// FOV
				(float)width/(float)height,				// ratio of screen dimension
				0.1f,									// near clip
				100.f);									// far clip

	positionObject(0,0,0);

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
	m_pWorldEffectVariable->SetMatrix((float*)&m_World);
	m_pViewEffectVariable->SetMatrix((float*)&m_View);
	m_pProjectionEffectVariable->SetMatrix((float*)&m_Projection);
	m_pAmbientMaterialVariable->SetFloatVector((float*)&m_ambientMaterial);
	m_pAmbientLightColourVariable->SetFloatVector((float*)&m_ambientLightColour);
	m_pDiffuseMaterialVariable->SetFloatVector((float*)&m_diffuseMaterial);
	m_pDiffuseLightColourVariable->SetFloatVector((float*)&m_diffuseLightColour);
	m_pLightDirectionVariable->SetFloatVector((float*)&m_lightDirection);
	m_pSpecularMaterialVariable->SetFloatVector((float*)&m_diffuseMaterial);
	m_pSpecularLightColourVariable->SetFloatVector((float*)&m_diffuseLightColour);
	m_pSpecularPower->SetFloat(m_specularPower); //right??
	m_pCameraPosVariable->SetFloatVector((float*)&m_CameraPos);
	
	/* tell the pipeline what primitives it will draw and the input-layout of the vertices. 
	Input-layout objects describe how vertex buffer data is streamed into the IA pipeline stage*/
	m_pD3D10Device->IASetPrimitiveTopology(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	m_pD3D10Device->IASetInputLayout(m_pTempVertexLayout);


    UINT stride = sizeof( Vertex );		// A stride is the size (in bytes) of the elements that are to be used from that vertex buffer.
    UINT offset = 0;					// A offset is the number of bytes between the first element of a vertex buffer and the first element that will be used

	/* IASetVertexBuffers(): Bind an array of vertex buffers to the input-assembler stage.
	   http://msdn.microsoft.com/en-us/library/windows/desktop/bb173591%28v=vs.85%29.aspx 
	*/
    m_pD3D10Device->IASetVertexBuffers( 
		0,					// The first input slot for binding.
		1,					// The number of vertex buffers in the array. 
		&m_pTempBuffer,		// A pointer to an array of vertex buffers
		&stride,			// Pointer to an array of stride values; one stride value for each buffer in the vertex-buffer array
		&offset );			// Pointer to an array of offset values; one offset value for each buffer in the vertex-buffer array

	// binding the index buffer
	m_pD3D10Device->IASetIndexBuffer(
		m_pTempIndexBuffer,
		DXGI_FORMAT_R32_UINT,
		0);


	// to get information about the technique such as the number of passes
	D3D10_TECHNIQUE_DESC techniqueDesc;
	m_pTempTechnique->GetDesc(&techniqueDesc);

	for (unsigned int i = 0; i < techniqueDesc.Passes; ++i)	// loop through all the passes in the technique
	{
		ID3D10EffectPass *pCurrentPass = m_pTempTechnique->GetPassByIndex(i);	// grab the pass
		pCurrentPass->Apply(0);			// apply it(this ensures that the pipeline states and shaders are all bound to the pipeline) 
		m_pD3D10Device->DrawIndexed(36,0,0);		// DrawIndexed(number of vertices, start location in the buffer, base vertex location)
	}

	
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
	
	Vertex verts_old[] = {			// standart triangle
		{-1.0f, -1.0f, 0.0f},	// lower left corner
		{ 0.0f,  1.0f, 0.0f},	// mid of top
		{ 1.0f, -1.0f, 0.0f}	// lower right corner
	};

	Vertex verts1[] = {			// right angled triangle
		{ 0.0f, -1.0f, 0.0f},	// mid of bottom
		{ 0.0f,  1.0f, 0.0f},	// mid of top
		{ 1.0f, -1.0f, 0.0f}	// lower right corner
	};

	Vertex vertsSquare[] = {			// square
		{-1.0f, -1.0f, 0.0f},	// 
		{-1.0f,  1.0f, 0.0f},	// 
		{ 1.0f, -1.0f, 0.0f},	// 
		{ 1.0f,  1.0f, 0.0f}	// 
	};

	Vertex verts[] = {										// cube			
		// x, 	 y,     z 		normals: x	y		z
		{-1.0f, -1.0f,  1.0f,		0.0f,  0.5f,  0.5f},	// front face 
		{-1.0f,  1.0f,  1.0f,		0.0f,  0.5f,  0.5f},	// 
		{ 1.0f, -1.0f,  1.0f,		0.0f, -0.5f,  0.5f},	// 
		{ 1.0f,  1.0f,  1.0f,		0.0f, -0.5f,  0.5f},	//

		{-1.0f, -1.0f, -1.0f,		0.0f,  0.5f, -0.5f},	// back face
		{-1.0f,  1.0f, -1.0f,		0.0f,  0.5f, -0.5f},	// 
		{ 1.0f, -1.0f, -1.0f,		0.0f, -0.5f, -0.5f},	// 
		{ 1.0f,  1.0f, -1.0f,		0.0f, -0.5f, -0.5f}	    // 
	};

	
	// Defines the propertys of the buffer
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;			// Identify how the buffer is expected to be read from and written to
	bd.ByteWidth = sizeof( Vertex ) * 8;	// buffer is big enough for 8 vertices
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;// Identify how the buffer will be bound to the pipeline
	bd.CPUAccessFlags = 0;					// CPU access flags ( 0 if no CPU access is necessary)
	bd.MiscFlags = 0;						// Miscellaneous flags ( 0 if unused)

	// prepare the data to copy it into the buffer
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &verts;

	// CreateBufer(): Create a buffer (vertex buffer, index buffer, or shader-constant buffer).
	//   http://msdn.microsoft.com/en-us/library/windows/desktop/bb173544%28v=vs.85%29.aspx
	
	if (FAILED(m_pD3D10Device->CreateBuffer(
			&bd,				// Pointer to a buffer description
			&InitData,			// Pointer to the initialization data
			&m_pTempBuffer )))	// Address of a pointer to the buffer created 
	{
		OutputDebugStringA("Cant create buffer");	
	}
	

	int indices[36] = {0, 1, 2, // front face
					  1, 2, 3,

					  4, 5, 6,	// back face
					  5, 6, 7,

					  4, 5, 0,	// left face
					  5, 0, 1,

					  2, 3, 6,	// right face
					  3, 6, 7,

					  1, 5, 3,	// top face
					  5, 3, 7,

					  0, 4, 2,	// bottom face
					  4, 2, 6};

	D3D10_BUFFER_DESC indexBD;
	indexBD.Usage = D3D10_USAGE_DEFAULT;
	indexBD.ByteWidth = sizeof(int) * 36;
	indexBD.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBD.CPUAccessFlags = 0;
	indexBD.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitlBData;
	InitlBData.pSysMem = &indices;

	if (FAILED(m_pD3D10Device->CreateBuffer(
			&indexBD,			// Pointer to a buffer description
			&InitlBData,		// Pointer to the initialization data
			&m_pTempIndexBuffer )))	// Address of a pointer to the buffer created 
	{
		OutputDebugStringA("Cant create buffer");	
	}




	return true;
}

bool D3D10Renderer::createVertexLayout()
{
	/* We have to retrieve the pass description from the technqiue as this is 
	used to bind the vertex in our C++ code to the vertex in the 
	effect (PassDesc.pIAInputSignature and PassDesc.IAInputSignatureSize).
	*/
	UINT numElements = sizeof( VertexLayout ) / sizeof(D3D10_INPUT_ELEMENT_DESC);
    D3D10_PASS_DESC PassDesc;
    m_pTempTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );

	/* createInputLayout(): Create an input-layout object to describe the input-buffer data for the input-assembler stage.
	   http://msdn.microsoft.com/en-us/library/windows/desktop/bb173550%28v=vs.85%29.aspx 
	*/
    if (FAILED(m_pD3D10Device->CreateInputLayout( 
		VertexLayout,					// An array of the input-assembler stage input data types
		numElements,					// The number of input-data types in the array of input-elements.
		PassDesc.pIAInputSignature,		// A pointer to the compiled shader.
        PassDesc.IAInputSignatureSize,	// Size of the compiled shader.
		&m_pTempVertexLayout )))		// A pointer to the input-layout object created
	{
		OutputDebugStringA("Can't create layout");
	}

	
	return true;
}

bool D3D10Renderer::loadEffectFromFile(WCHAR* pFilename)
{
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob *pErrorBuffer = NULL;
	if (FAILED(D3DX10CreateEffectFromFile(
		pFilename,			// pointer to the file which holds the effect
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

	m_pWorldEffectVariable = m_pTempEffect->GetVariableByName("matWorld")->AsMatrix();
	m_pViewEffectVariable = m_pTempEffect->GetVariableByName("matView")->AsMatrix();
	m_pProjectionEffectVariable = m_pTempEffect->GetVariableByName("matProjection")->AsMatrix();
	m_pAmbientMaterialVariable = m_pTempEffect->GetVariableByName("ambientMaterial")->AsVector();
	m_pAmbientLightColourVariable = m_pTempEffect->GetVariableByName("ambientLightColour")->AsVector();
	m_pDiffuseMaterialVariable = m_pTempEffect->GetVariableByName("diffuseMaterial")->AsVector();
	m_pDiffuseLightColourVariable = m_pTempEffect->GetVariableByName("diffuseLightColour")->AsVector();
	m_pLightDirectionVariable = m_pTempEffect->GetVariableByName("lightDirection")->AsVector();
	m_pSpecularMaterialVariable = m_pTempEffect->GetVariableByName("specularMaterial")->AsVector();
	m_pSpecularLightColourVariable = m_pTempEffect->GetVariableByName("specularLightColour")->AsVector();
	m_pSpecularPower = m_pTempEffect->GetVariableByName("specularPower")->AsScalar();
	m_pCameraPosVariable = m_pTempEffect->GetVariableByName("cameraPosition")->AsVector();

	m_pTempTechnique=m_pTempEffect->GetTechniqueByName("Render");	//  retrieve the technique by name.

	return true;
}
void D3D10Renderer::createCamera(XMVECTOR &position, XMVECTOR &focus, XMVECTOR &up,
			float fov, float aspectRatio, float nearClip, float farClip)
{
	// create ViewMAtrix, represents camera
	m_View = XMMatrixLookAtLH(position, focus, up);		
	//proj matrix represents a virtual capped pyramid which governs what the camera can see
	m_Projection = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearClip, farClip);
}

void D3D10Renderer::positionObject(float x, float y, float z)
{
	m_World = XMMatrixTranslation(x, y, z);
}