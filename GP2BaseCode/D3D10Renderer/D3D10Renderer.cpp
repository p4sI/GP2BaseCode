#include "D3D10Renderer.h"
#include "../GameApplication/GameObject.h"
#include "../GameApplication/Components.h"

// Define the input layout of the vertex, this is so we can bind a vertex to the pipeline - BMD
const D3D10_INPUT_ELEMENT_DESC VerexLayout[] =
{		
    { "POSITION", //Name of the semantic, this helps to bind the vertex inside the Vertex Shader - BMD
	0, //The index of the semantic, see above - BMD
	DXGI_FORMAT_R32G32B32_FLOAT, //The format of the element, in this case 32 bits of each sub element - BMD
	0, //Input slot - BMD
	0, //Offset, this will increase as we add more elements(such texture coords) to the layout - BMD
	D3D10_INPUT_PER_VERTEX_DATA, //Input classification - BMD
	0 }, //Instance Data slot - BMD
	
    { "TEXCOORD", //Name of the semantic, this helps to bind the vertex inside the Vertex Shader - BMD
	0, //The index of the semantic, see above - BMD
	DXGI_FORMAT_R32G32_FLOAT, //The format of the element, in this case 32 bits of each sub element - BMD
	0, //Input slot - BMD
	12, //Offset, this will increase as we add more elements(such texture coords) to the layout - BMD
	D3D10_INPUT_PER_VERTEX_DATA, //Input classification - BMD
	0 }, //Instance Data slot - BMD

    { "NORMAL", //Name of the semantic, this helps to bind the vertex inside the Vertex Shader - BMD
	0, //The index of the semantic, see above - BMD
	DXGI_FORMAT_R32G32B32_FLOAT, //The format of the element, in this case 32 bits of each sub element - BMD
	0, //Input slot - BMD
	20, //Offset, this will increase as we add more elements(such texture coords) to the layout - BMD
	D3D10_INPUT_PER_VERTEX_DATA, //Input classification - BMD
	0 }, //Instance Data slot - BMD
};

const char basicEffect[]=\
	"float4 VS( float4 Pos : POSITION) : SV_POSITION"\
	"{"\
	"	return Pos;"\
	"}"\
	"float4 PS( float4 Pos : SV_POSITION ) : SV_Target"\
	"{"\
	"	return float4( 1.0f, 1.0f, 1.0f, 1.0f );"\
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

D3D10Renderer::D3D10Renderer()
{
	m_pD3D10Device=NULL;
	m_pRenderTargetView=NULL;
	m_pSwapChain=NULL;
	m_pDepthStencelView=NULL;
	m_pDepthStencilTexture=NULL;
	m_pDefaultVertexLayout=NULL;
	m_pDefaultEffect=NULL;
}

D3D10Renderer::~D3D10Renderer()
{
	if (m_pD3D10Device)
		m_pD3D10Device->ClearState();
	if (m_pDefaultEffect)
		m_pDefaultEffect->Release();
	if (m_pDefaultVertexLayout)
		m_pDefaultVertexLayout->Release();
	if (m_pRenderTargetView)
		m_pRenderTargetView->Release();
	if (m_pDepthStencelView)
		m_pDepthStencelView->Release();
	if (m_pDepthStencilTexture)
		m_pDepthStencilTexture->Release();
	if (m_pSwapChain)
		m_pSwapChain->Release();
	if (m_pD3D10Device)
		m_pD3D10Device->Release();
}

bool D3D10Renderer::init(void *pWindowHandle,bool fullScreen)
{
	HWND window=(HWND)pWindowHandle;
	RECT windowRect;
	GetClientRect(window,&windowRect);

	UINT width=windowRect.right-windowRect.left;
	UINT height=windowRect.bottom-windowRect.top;

	if (!createDevice(window,width,height,fullScreen))
		return false;
	if (!createInitialRenderTarget(width,height))
		return false;

	//create default effect, please note we are create an input layout based
	//on this. There should be a better way
	m_pDefaultEffect=loadEffectFromMemory(basicEffect);
	if (!m_pDefaultEffect)
	{
		return false;
	}
	m_pDefaultTechnique=m_pDefaultEffect->GetTechniqueByIndex(0);
	m_pDefaultVertexLayout=createVertexLayout(m_pDefaultEffect);

	return true;
}

bool D3D10Renderer::createDevice(HWND window,int windowWidth, int windowHeight,bool fullScreen)
{
	UINT createDeviceFlags=0;
#ifdef _DEBUG
	createDeviceFlags|=D3D10_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd;
       ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	if (fullScreen)
		sd.BufferCount = 2;
	else 
		sd.BufferCount=1;
	sd.OutputWindow = window;
	sd.Windowed = (BOOL)(!fullScreen);
       sd.SampleDesc.Count = 1;
       sd.SampleDesc.Quality = 0;
       sd.BufferDesc.Width = windowWidth;
       sd.BufferDesc.Height = windowHeight;
       sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
       sd.BufferDesc.RefreshRate.Numerator = 60;
       sd.BufferDesc.RefreshRate.Denominator = 1;

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

bool D3D10Renderer::createInitialRenderTarget(int windowWidth, int windowHeight)
{
	ID3D10Texture2D *pBackBuffer;
	
	if (FAILED(m_pSwapChain->GetBuffer(0, 
		__uuidof(ID3D10Texture2D),
		(void**)&pBackBuffer))) 
		return false;

	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width=windowWidth;
	descDepth.Height=windowHeight;
	descDepth.MipLevels=1;
	descDepth.ArraySize=1;
	descDepth.Format=DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count=1;
	descDepth.SampleDesc.Quality=0;
	descDepth.Usage=D3D10_USAGE_DEFAULT;
	descDepth.BindFlags=D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags=0;
	descDepth.MiscFlags=0;

	if (FAILED(m_pD3D10Device->CreateTexture2D(&descDepth,NULL,
			&m_pDepthStencilTexture)))
		return false;

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format=descDepth.Format;
	descDSV.ViewDimension=D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice=0;

	if (FAILED(m_pD3D10Device->CreateDepthStencilView(m_pDepthStencilTexture,
                   &descDSV,&m_pDepthStencelView)))
		return false;

	if (FAILED(m_pD3D10Device->CreateRenderTargetView( pBackBuffer, 
		NULL, 
		&m_pRenderTargetView ))){
             pBackBuffer->Release();
		return  false;
	}
       pBackBuffer->Release();

	m_pD3D10Device->OMSetRenderTargets(1, 
		&m_pRenderTargetView,		
		m_pDepthStencelView);
	
	D3D10_VIEWPORT vp;
   	vp.Width = windowWidth;
    vp.Height = windowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    
	m_pD3D10Device->RSSetViewports( 1 
		, &vp );
	return true;
}

void D3D10Renderer::clear(float r,float g,float b,float a)
{
    // Just clear the backbuffer, colours start at 0.0 to 1.0
	// Red, Green , Blue, Alpha - BMD
    const float ClearColor[4] = { r, g, b, a}; 
	//Clear the Render Target
	//http://msdn.microsoft.com/en-us/library/bb173539%28v=vs.85%29.aspx - BMD
    m_pD3D10Device->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	m_pD3D10Device->ClearDepthStencilView(m_pDepthStencelView,D3D10_CLEAR_DEPTH,1.0f,0);
}

void D3D10Renderer::render()
{
	int noIndices=0;
	int noVerts=0;
	ID3D10Buffer *pIndexBuffer=NULL;
	ID3D10Buffer *pVertexBuffer=NULL;
	ID3D10Effect *pCurrentEffect=m_pDefaultEffect;
	ID3D10EffectTechnique *pCurrentTechnique=m_pDefaultTechnique;
	ID3D10InputLayout *pCurrentLayout=m_pDefaultVertexLayout;

	XMFLOAT3 cameraPos=XMFLOAT3(0.0f,0.0f,-10.0f);
	XMFLOAT3 focusPos=XMFLOAT3(0.0f,0.0f,0.0f);
	XMFLOAT3 up=XMFLOAT3(0.0f,1.0f,0.0f);
	XMMATRIX view=XMMatrixLookAtLH(XMLoadFloat3(&cameraPos),XMLoadFloat3(&focusPos),XMLoadFloat3(&up));
	XMMATRIX projection=XMMatrixPerspectiveFovLH(XM_PI/4,800.0f/640.0f,0.1f,100.0f);
	XMMATRIX world=XMMatrixIdentity();

	m_pD3D10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	

	while(!m_RenderQueue.empty())
	{
		GameObject * pObject=m_RenderQueue.front();
		if(pObject)
		{
			//Grab Transform
			Transform transform=pObject->getTransfrom();

			//Now grab Visual Component
			VisualComponent *pVisualComponent=static_cast<VisualComponent *>(pObject->getComponent("Visual"));
			if (pVisualComponent)
			{
				pIndexBuffer=pVisualComponent->getIndexBuffer();
				pVertexBuffer=pVisualComponent->getVertexBuffer();
				noVerts=pVisualComponent->getNoVerts();
				noIndices=pVisualComponent->getNoIndices();

				m_pD3D10Device->IASetIndexBuffer(pIndexBuffer,DXGI_FORMAT_R32_UINT,0);

				//Get the stride(size) of the a vertex, we need this to tell the pipeline the size of one vertex - BMD
				UINT stride = sizeof( Vertex );
				//The offset from start of the buffer to where our vertices are located - BMD
				UINT offset = 0;
				//Bind the vertex buffer to input assembler stage - BMD
				 //   http://msdn.microsoft.com/en-us/library/bb173591%28v=VS.85%29.aspx - BMD
				m_pD3D10Device->IASetVertexBuffers( 
					0, //The input slot to bind, zero indicates the first slot - BMD
					1, //The number of buffers - BMD
					&pVertexBuffer, //A pointer to an array of vertex buffers - BMD
					&stride, //Pointer to an array of strides of vertices in the buffer - BMD
					&offset );//Pointer to an array of offsets to the vertices in the vertex buffers - BMD

				if (pVisualComponent->getVertexLayout())
				{
					pCurrentLayout=pVisualComponent->getVertexLayout();
				}
			}

			m_pD3D10Device->IASetInputLayout(pCurrentLayout);


			//Do we have an Effect? If we don't then use default
			Material *pMaterial=static_cast<Material*>(pObject->getComponent("Material"));
			if (pMaterial)
			{
				if (pMaterial->getEffect())
				{
					pCurrentEffect=pMaterial->getEffect();
				}
				if (pMaterial->getCurrentTechnique())
				{
					pCurrentTechnique=pMaterial->getCurrentTechnique();
				}
				//Retrieve & send material stuff
			}

			ID3D10EffectMatrixVariable * pWorldMatrixVar=pCurrentEffect->GetVariableByName("matWorld")->AsMatrix();
			ID3D10EffectMatrixVariable * pViewMatrixVar=pCurrentEffect->GetVariableByName("matView")->AsMatrix();
			ID3D10EffectMatrixVariable * pProjectionMatrixVar=pCurrentEffect->GetVariableByName("matProjection")->AsMatrix();

			if (pWorldMatrixVar)
			{
				pWorldMatrixVar->SetMatrix((float*)&transform.getWorld());
			}
			if (pViewMatrixVar)
			{
				pViewMatrixVar->SetMatrix((float*)&view);
			}
			if (pProjectionMatrixVar)
			{
				pProjectionMatrixVar->SetMatrix((float*)&projection);
			}

			D3D10_TECHNIQUE_DESC techniqueDesc;
			pCurrentTechnique->GetDesc(&techniqueDesc); 

			for (unsigned int i=0;i<techniqueDesc.Passes;++i)
			{
				ID3D10EffectPass *pCurrentPass=pCurrentTechnique->GetPassByIndex(i);
				pCurrentPass->Apply(0);

				if (pIndexBuffer)
					m_pD3D10Device->DrawIndexed(noIndices,0,0);
				else if (pVertexBuffer)
					m_pD3D10Device->Draw(noVerts,0);
			}

		}

		m_RenderQueue.pop();
	}
}

void D3D10Renderer::present()
{
	//Swaps the buffers in the chain, the back buffer to the front(screen)
	//http://msdn.microsoft.com/en-us/library/bb174576%28v=vs.85%29.aspx - BMD
    m_pSwapChain->Present( 0, 0 );
}


ID3D10Effect * D3D10Renderer::loadEffectFromMemory(const char *pMem)
{
	ID3D10Effect *pEffect=NULL;
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program. - BMD
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob * pErrorBuffer=NULL;
	if (FAILED(D3DX10CreateEffectFromMemory(pMem,
		strlen(pMem),
		NULL,
		NULL,
		NULL,
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        m_pD3D10Device, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&pEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		&pErrorBuffer, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
		return NULL;
	}
	return pEffect;
}

ID3D10Effect * D3D10Renderer::loadEffectFromFile(const char *pFilename)
{
	ID3D10Effect *pEffect=NULL;
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program. - BMD
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob * pErrorBuffer=NULL;
	if (FAILED(D3DX10CreateEffectFromFileA(pFilename,
		NULL,
		NULL,
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        m_pD3D10Device, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&pEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		&pErrorBuffer, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
		return NULL;
	}

	return pEffect;
}

ID3D10Buffer * D3D10Renderer::createVertexBuffer(int size,Vertex *pVerts)
{
	ID3D10Buffer *pBuffer=NULL;

	//Buffer desc
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;//Usuage flag,this describes how the buffer is read/written to. Default is the most common - BMD
	bd.ByteWidth = sizeof( Vertex ) * size;//The size of the buffer, this is the size of one vertex * by the num of vertices -BMD
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;//BindFlags, says how the buffer is going to be used. In this case as a Vertex Buffer - BMD
	bd.CPUAccessFlags = 0;//CPUAccessFlag, sepcfies if the CPU can access the resource. 0 means no CPU access - BMD
	bd.MiscFlags = 0;//MiscCreation flags, this will be zero most of the time - BMD

	//This is used to supply the initial data for the buffer - BMD
	//http://msdn.microsoft.com/en-us/library/bb172456%28VS.85%29.aspx - BMD
	D3D10_SUBRESOURCE_DATA InitData;
	//A pointer to the initial data
	InitData.pSysMem = pVerts;
    
	//Create the Buffer using the buffer description and initial data - BMD
	//http://msdn.microsoft.com/en-us/library/bb173544%28v=VS.85%29.aspx - BMD
	if (FAILED(m_pD3D10Device->CreateBuffer( 
		&bd, //Memory address of a buffer description - BMD
		&InitData, //Memory address of the initial data - BMD
		&pBuffer )))//A pointer to a memory address of a buffer, this will be initialise after - BMD
	{
		OutputDebugStringA("Can't create index buffer");
		return NULL;
	}

	return pBuffer;
}

ID3D10Buffer * D3D10Renderer::createIndexBuffer(int size,int *pIndices)
{
	ID3D10Buffer *pBuffer=NULL;

	//Buffer desc
	D3D10_BUFFER_DESC indexbd;
	indexbd.Usage = D3D10_USAGE_DEFAULT;//Usuage flag,this describes how the buffer is read/written to. Default is the most common - BMD
	indexbd.ByteWidth = sizeof( int ) * size;//The size of the buffer, this is the size of one vertex * by the num of vertices -BMD
	indexbd.BindFlags = D3D10_BIND_INDEX_BUFFER;//BindFlags, says how the buffer is going to be used. In this case as a Vertex Buffer - BMD
	indexbd.CPUAccessFlags = 0;//CPUAccessFlag, sepcfies if the CPU can access the resource. 0 means no CPU access - BMD
	indexbd.MiscFlags = 0;//MiscCreation flags, this will be zero most of the time - BMD

	//This is used to supply the initial data for the buffer - BMD
	//http://msdn.microsoft.com/en-us/library/bb172456%28VS.85%29.aspx - BMD
	D3D10_SUBRESOURCE_DATA InitIBData;
	//A pointer to the initial data
	InitIBData.pSysMem = pIndices;
    
	//Create the Buffer using the buffer description and initial data - BMD
	//http://msdn.microsoft.com/en-us/library/bb173544%28v=VS.85%29.aspx - BMD
	if (FAILED(m_pD3D10Device->CreateBuffer( 
		&indexbd, //Memory address of a buffer description - BMD
		&InitIBData, //Memory address of the initial data - BMD
		&pBuffer )))//A pointer to a memory address of a buffer, this will be initialise after - BMD
	{
		OutputDebugStringA("Can't create index buffer");
		return NULL;
	}

	return pBuffer;
}

ID3D10InputLayout * D3D10Renderer::createVertexLayout(ID3D10Effect * pEffect)
{
	ID3D10InputLayout*      pVertexLayout=NULL;

	//Number of elements in the layout - BMD
    UINT numElements = sizeof( VerexLayout ) / sizeof(D3D10_INPUT_ELEMENT_DESC);
	//Get the Pass description, we need this to bind the vertex to the pipeline - BMD
    D3D10_PASS_DESC PassDesc;
	pEffect->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc(&PassDesc);
	//Create Input layout to describe the incoming buffer to the input assembler - BMD
    if (FAILED(m_pD3D10Device->CreateInputLayout( VerexLayout, //The layout describing our vertices - BMD
		numElements, //The number of elements in the layout
		PassDesc.pIAInputSignature,//Input signature of the description of the pass - BMD
        PassDesc.IAInputSignatureSize, //The size of this Signature size of the pass - BMD
		&pVertexLayout ))) //The pointer to an address of Vertex Layout - BMD
	{
		OutputDebugStringA("Can't create layout");
	}

	return pVertexLayout;
}


void D3D10Renderer::addToRenderQueue(GameObject *pObject)
{
	m_RenderQueue.push(pObject);
}
