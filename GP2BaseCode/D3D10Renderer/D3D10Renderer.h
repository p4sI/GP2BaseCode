#pragma once

//The header file for the renderer interface
#include "../Renderer/Renderer.h"
#include <Windows.h>
#include <D3D10.h>
#include <D3DX10.h>
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

//forward declarations of the D3D10 interfaces
/*
struct ID3D10Device;
struct IDXGISwapChain;
struct ID3D10RenderTargetView;
struct ID3D10DepthStencilView;
struct ID3D10Texture2D;
struct ID3D10Effect;
struct ID3D10Buffer;
struct ID3D10InputLayout;
struct ID3D10EffectTechnique;
*/
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
	void createCamera(XMVECTOR &position, XMVECTOR &focus, XMVECTOR &up,
			float fov, float aspectRatio, float nearClip, float farClip);
	void positionObject(float x, float y, float z);

private:
	bool createDevice(HWND pWindowHandle, int windowWidth, int windowHeight,
			bool fullScreen);
	bool createInitialRenderTarget(int windowWidth, int windowHeight);
	bool loadEffectFromMemory(const char* pMem);
	bool createBuffer();
	bool createVertexLayout();
	bool loadEffectFromFile(WCHAR* pFilename);
private:
	//D3D10 stuff
	ID3D10Device *m_pD3D10Device;
	IDXGISwapChain *m_pSwapChain;
	ID3D10RenderTargetView *m_pRenderTargetView;
	ID3D10DepthStencilView *m_pDepthStencilView;  //stencil or stencEl ?!?!
	ID3D10Texture2D *m_pDepthStencilTexture;
	ID3D10Effect *m_pTempEffect;
	ID3D10Buffer *m_pTempBuffer;
	ID3D10InputLayout *m_pTempVertexLayout;
	ID3D10EffectTechnique *m_pTempTechnique;
	XMMATRIX m_View;
	XMMATRIX m_Projection;
	XMMATRIX m_World;
	ID3D10EffectMatrixVariable *m_pWorldEffectVariable;
	ID3D10EffectMatrixVariable *m_pViewEffectVariable;
	ID3D10EffectMatrixVariable *m_pProjectionEffectVariable;
	ID3D10Buffer * m_pTempIndexBuffer;
	// Ambient Effect Stuff
	ID3D10EffectVectorVariable *m_pAmbientMaterialVariable; 
	ID3D10EffectVectorVariable *m_pAmbientLightColourVariable;
	XMFLOAT4 m_ambientMaterial; 
	XMFLOAT4 m_ambientLightColour;
	// Diffuse Effect Stuff
	ID3D10EffectVectorVariable *m_pDiffuseMaterialVariable; 
	ID3D10EffectVectorVariable *m_pDiffuseLightColourVariable;
	ID3D10EffectVectorVariable *m_pLightDirectionVariable;
	XMFLOAT4 m_diffuseMaterial; 
	XMFLOAT4 m_diffuseLightColour;
	XMFLOAT3 m_lightDirection;
	// Specular Effect Stuff
	ID3D10EffectVectorVariable *m_pSpecularMaterialVariable; 
	ID3D10EffectVectorVariable *m_pSpecularLightColourVariable;
	ID3D10EffectVariable *m_pSpecularPower;
	XMFLOAT4 m_specularMaterial; 
	XMFLOAT4 m_specularLightColour;
	float m_specularPower;

};
