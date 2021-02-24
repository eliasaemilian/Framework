#include "PlanarReflection.h"
#include "Utils.h"

void PlanarReflection::init( ID3D11Device* dev, UINT width, UINT height )
{
	initReflectionTexture( dev, width, height );
	initBuffer( dev );
}

bool PlanarReflection::initReflectionTexture( ID3D11Device* dev, UINT width, UINT height )
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
	result = dev->CreateTexture2D( &textureDesc, NULL, &_pRenderTargetTexture );
	if (FAILED( result ))
	{
		return false;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = dev->CreateRenderTargetView( _pRenderTargetTexture, &renderTargetViewDesc, &_pRenderTargetView );
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
	result = dev->CreateShaderResourceView( _pRenderTargetTexture, &shaderResourceViewDesc, &_pShaderResourceView );
	if (FAILED( result ))
	{
		return false;
	}

	return true;
}

void PlanarReflection::initBuffer( ID3D11Device* dev )
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof( ReflectionBuffer );
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer( &desc, nullptr, &_pReflectionBuffer );
}

void PlanarReflection::setPlanarReflection( ID3D11DeviceContext* devCon, XMFLOAT4X4* reflectionMatrix )
{
	// SET MATRIX BUFFER
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = devCon->Map( _pReflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data );
	if (FAILED( hr )) return;

	XMMATRIX mReflect = XMLoadFloat4x4( reflectionMatrix );
	mReflect = XMMatrixTranspose( mReflect );
	ReflectionBuffer* pBuffer = reinterpret_cast< ReflectionBuffer* >( data.pData );
	XMStoreFloat4x4( &pBuffer->reflectionMatrix, mReflect );

	devCon->Unmap( _pReflectionBuffer, 0 );
	devCon->VSSetConstantBuffers( 2, 1, &_pReflectionBuffer );

	// SET TEXTURE
	devCon->PSSetShaderResources( 1, 1, &_pShaderResourceView );
}

void PlanarReflection::setRenderTargetToTexture( ID3D11DeviceContext* devCon, ID3D11DepthStencilView* depthStencilView )
{
	devCon->OMSetRenderTargets( 1, &_pRenderTargetView, depthStencilView );
}

void PlanarReflection::clearRenderTargetView( ID3D11DeviceContext* devCon )
{
	const FLOAT color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	devCon->ClearRenderTargetView( _pRenderTargetView, color );
}

void PlanarReflection::deinit()
{
	safeRelease<ID3D11Texture2D>( _pRenderTargetTexture );
	safeRelease<ID3D11RenderTargetView>( _pRenderTargetView );
	safeRelease<ID3D11ShaderResourceView>( _pShaderResourceView );
	safeRelease<ID3D11Buffer>( _pReflectionBuffer );

}




