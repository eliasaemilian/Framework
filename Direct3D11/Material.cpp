#pragma once
#include "Material.h"
#include "WICTextureLoader11.h"
#include <d3dcompiler.h>
#include "Utils.h"
#include <d3d11.h>
#include <vector>
#include <DDSTextureLoader.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

int Material::init( ID3D11Device* pD3DDevice, LPCWSTR textureName, LPCWSTR normalMap, LPCWSTR additionalTex, LPCWSTR  vertexShader, LPCWSTR pixelShader )
{
	int error = createVertexShader( pD3DDevice, vertexShader );
	if (error != 0) return error;

	error = createPixelShader( pD3DDevice, pixelShader );
	if (error != 0) return error;

	error = createMaterialBuffer( pD3DDevice );
	if (error != 0) return error;

	if (textureName != NULL)
	{
		error = createMainTextureAndSampler( pD3DDevice, textureName );
		if (error != 0) return error;
	}

	if (normalMap != NULL)
	{
		error = createNormalMap( pD3DDevice, normalMap );
		if (error != 0) return error;
	}

	if (additionalTex != NULL)
	{
		error = createAdditonalTexture( pD3DDevice, additionalTex );
		if (error != 0) return error;
	}

	return 0;
}

void Material::render( ID3D11DeviceContext* pD3DDeviceContext, MaterialBuffer* mBuf )
{
	pD3DDeviceContext->IASetInputLayout( _pInputLayout );
	pD3DDeviceContext->VSSetShader( _pVertexShader, nullptr, 0 );
	pD3DDeviceContext->PSSetShader( _pPixelShader, nullptr, 0 );

	setMaterialBuffer( pD3DDeviceContext, &mBuf->WORLD_MATRIX, &mBuf->PARAM_FLOAT4_1, &mBuf->PARAM_FLOAT4_2, &mBuf->PARAM_FLOAT4_3, &mBuf->PARAM_FLOAT4_4 );


	if (&_pMainTexture != nullptr)
	{
		pD3DDeviceContext->PSSetShaderResources( 0, 1, &_pMainTexture );
		pD3DDeviceContext->PSSetSamplers( 0, 1, &_pMainSampler );
	}

	if (&_pNormalMap != nullptr)
	{
		pD3DDeviceContext->PSSetShaderResources( 2, 1, &_pNormalMap );
	}

	if (&_pAdditionalTex != nullptr)
	{
		pD3DDeviceContext->PSSetShaderResources( 3, 1, &_pAdditionalTex );
	}

}

void Material::deInit()
{
	safeRelease<ID3D11ShaderResourceView>( _pMainTexture );
	safeRelease<ID3D11SamplerState>( _pMainSampler );
	safeRelease<ID3D11Buffer>( _pMaterialBuffer );
	safeRelease<ID3D11VertexShader>( _pVertexShader );
	safeRelease<ID3D11PixelShader>( _pPixelShader );
	safeRelease<ID3D11InputLayout>( _pInputLayout );
}

int Material::createVertexShader( ID3D11Device* pD3DDevice, LPCWSTR vertexShader )
{
	ID3DBlob* pCompiledCode = nullptr;

	std::wstring comp = std::wstring( vertexShader ) + L".cso";
	HRESULT hr = D3DReadFileToBlob( comp.c_str(), &pCompiledCode );


	if (FAILED(hr))
	{
		std::wstring raw = std::wstring( vertexShader ) + L".hlsl";

		 hr = D3DCompileFromFile(
			 raw.c_str(), // shader filename
			nullptr, nullptr, // optional macros & includes
			"main", // entry point function name
			"vs_4_0", // shader type & version
			0, 0, // optional flags
			&pCompiledCode, // compiled code target
			nullptr // optional blob for all compile errors
		);

		 if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		 {
			 MessageBox( NULL, std::to_wstring( hr ).c_str(), vertexShader, 0 );
		 }
	}


	if (FAILED( hr )) return 40;

	hr = pD3DDevice->CreateVertexShader( pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pVertexShader );
	if (FAILED( hr )) return 42;

	int error = createInputLayout( pD3DDevice, pCompiledCode );
	if (error != 0) return error;

	safeRelease<ID3DBlob>( pCompiledCode );

	return 0;
}

int Material::createPixelShader( ID3D11Device* pD3DDevice, LPCWSTR pixelShader )
{
	ID3DBlob* pCompiledCode = nullptr;

	std::wstring comp = std::wstring( pixelShader ) + L".cso";
	HRESULT hr = D3DReadFileToBlob( comp.c_str(), &pCompiledCode );

	if (FAILED( hr ))
	{
		std::wstring raw = std::wstring( pixelShader ) + L".hlsl";

		hr = D3DCompileFromFile(
			raw.c_str(), // shader filename
			nullptr, nullptr, // optional macros & includes
			"main", // entry point function name
			"vs_4_0", // shader type & version
			0, 0, // optional flags
			&pCompiledCode, // compiled code target
			nullptr // optional blob for all compile errors
		);

		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			MessageBox( NULL, std::to_wstring( hr ).c_str(), pixelShader, 0 );
		}
	}


	if (FAILED( hr )) return 46;

	hr = pD3DDevice->CreatePixelShader( pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pPixelShader );
	if (FAILED( hr )) return 48;

	safeRelease<ID3DBlob>( pCompiledCode );

	return 0;
}

int Material::createInputLayout( ID3D11Device* pD3DDevice, ID3DBlob* pBlob )
{
	D3D11_INPUT_ELEMENT_DESC elements[4] = {};

	// position
	elements[0].SemanticName = "POSITION";
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	// color
	elements[1].SemanticName = "COLOR";
	elements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// normal
	elements[2].SemanticName = "NORMAL";
	elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// uv
	elements[3].SemanticName = "TEXCOORD";
	elements[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	HRESULT hr = pD3DDevice->CreateInputLayout( elements, 4, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &_pInputLayout );
	if (FAILED( hr )) return 44;

	return 0;
}


int Material::createMaterialBuffer( ID3D11Device* pD3DDevice )
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof( MaterialBuffer );
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer( &desc, nullptr, &_pMaterialBuffer );
	if (FAILED( hr )) return 46;

	return 0;
}

void Material::setMaterialBuffer( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, XMFLOAT4* f1, XMFLOAT4* f2, XMFLOAT4* f3, XMFLOAT4* f4 )
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map( _pMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data );
	if (FAILED( hr )) return;

	XMMATRIX mWorld = XMLoadFloat4x4( worldMatrix );

	mWorld = XMMatrixTranspose( mWorld );


	XMVECTOR float1 = XMLoadFloat4( f1 );
	XMVECTOR float2 = XMLoadFloat4( f2 );
	XMVECTOR float3 = XMLoadFloat4( f3 );
	XMVECTOR float4 = XMLoadFloat4( f4 );


	MaterialBuffer* pBuffer = reinterpret_cast< MaterialBuffer* >( data.pData );
	XMStoreFloat4x4( &pBuffer->WORLD_MATRIX, mWorld );

	XMStoreFloat4( &pBuffer->PARAM_FLOAT4_1, float1 );
	XMStoreFloat4( &pBuffer->PARAM_FLOAT4_2, float2 );
	XMStoreFloat4( &pBuffer->PARAM_FLOAT4_3, float3 );
	XMStoreFloat4( &pBuffer->PARAM_FLOAT4_4, float4 );

	pD3DDeviceContext->Unmap( _pMaterialBuffer, 0 );

	pD3DDeviceContext->VSSetConstantBuffers( 1, 1, &_pMaterialBuffer );

	pD3DDeviceContext->PSSetConstantBuffers( 2, 1, &_pMaterialBuffer );
}

int Material::createMainTextureAndSampler( ID3D11Device* pD3DDevice, LPCWSTR textureName )
{
	HRESULT hr = CreateWICTextureFromFile( pD3DDevice, textureName, nullptr, &_pMainTexture );
	if (FAILED( hr )) return 48;

	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = pD3DDevice->CreateSamplerState( &desc, &_pMainSampler );
	if (FAILED( hr )) return 49;

	return 0;
}

int Material::createNormalMap( ID3D11Device* pD3DDevice, LPCWSTR textureName )
{
	HRESULT hr = CreateWICTextureFromFile( pD3DDevice, textureName, nullptr, &_pNormalMap );
	if (FAILED( hr )) return 48;

	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	return 0;
}

int Material::createAdditonalTexture( ID3D11Device* pD3DDevice, LPCWSTR textureName )
{
	HRESULT hr = CreateWICTextureFromFile( pD3DDevice, textureName, nullptr, &_pAdditionalTex );
	if (FAILED( hr )) return 48;

	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	return 0;
}

int Material::createCubeMapTextureAndSampler( ID3D11Device* dev, LPCWSTR textureName )
{
	int hr = 0;

	// texture descriptor
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 1024;
	textureDesc.Height = 1024;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* SMTexture = 0;

	// create shader resource view
	ID3D11ShaderResourceView** resview = nullptr;
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = textureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	dev->CreateShaderResourceView( SMTexture, &SMViewDesc, resview );

	// create texture
	hr = CreateDDSTextureFromFile( dev, L"cubemapSky", ( ID3D11Resource** )&SMTexture, ( ID3D11ShaderResourceView** )&SMViewDesc, 0, nullptr );


	return hr;
}
