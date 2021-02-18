#pragma once
#include <d3d9.h>

#define FVF D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL // flexible vertex format

struct Vertex
{
	// position
	FLOAT x;
	FLOAT y;
	FLOAT z;

	// normal
	FLOAT nx;
	FLOAT ny;
	FLOAT nz;

	// color
	D3DCOLOR color;

	// texture coordinates
	FLOAT u;
	FLOAT v;

	Vertex(FLOAT x, FLOAT y, FLOAT z) : color(D3DCOLOR_XRGB(255, 255, 255)), u(0.0f), v(0.0f), nx(0.0f), ny(0.0f), nz(0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vertex(FLOAT x, FLOAT y, FLOAT z, D3DCOLOR color) : Vertex(x, y, z)
	{
		this->color = color;
	}

	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) : Vertex(x, y, z)
	{
		this->u = u;
		this->v = v;
	}

	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v, FLOAT nx, FLOAT ny, FLOAT nz) : Vertex(x, y, z, u, v)
	{
		this->nx = nx;
		this->ny = ny;
		this->nz = nz;
	}
};