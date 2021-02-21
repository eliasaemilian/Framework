#include "MirrorMaterial.h"

bool MirrorMaterial::createReflectionTexture( ID3D11Device* device, LPCWSTR textureName, UINT width, UINT height )
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


	// Initialize the render target texture description.
	ZeroMemory( &textureDesc, sizeof( textureDesc ) );

	// Setup the render target texture description.
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = device->CreateTexture2D( &textureDesc, NULL, &m_renderTargetTexture );
	if (FAILED( result ))
	{
		return false;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView( m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView );
	if (FAILED( result ))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView( m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView );
	if (FAILED( result ))
	{
		return false;
	}

	return true;
}

void MirrorMaterial::setReflectionBuffer( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* reflectionMatrix )
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map( _pReflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data );
	if (FAILED( hr )) return;

	XMMATRIX mReflect = XMLoadFloat4x4( reflectionMatrix );

	mReflect = XMMatrixTranspose( mReflect );


	ReflectionBuffer* pBuffer = reinterpret_cast< ReflectionBuffer* >( data.pData );
	XMStoreFloat4x4( &pBuffer->reflectionMatrix, mReflect );

	pD3DDeviceContext->Unmap( _pReflectionBuffer, 0 );

	pD3DDeviceContext->VSSetConstantBuffers( 2, 1, &_pReflectionBuffer );
}

int MirrorMaterial::createReflectionBuffer( ID3D11Device* pD3DDevice )
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof( ReflectionBuffer );
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer( &desc, nullptr, &_pReflectionBuffer );
	if (FAILED( hr )) return 46;

	return 0;
}