#pragma once

#include <windows.h>
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 textureCoords;
	XMFLOAT3 normal;
	
	Vertex()
	{
		position=XMFLOAT3(0.0f,0.0f,0.0f);
		normal=XMFLOAT3(0.0f,0.0f,0.0f);
		textureCoords=XMFLOAT2(0.0f,0.0f);
	};
};