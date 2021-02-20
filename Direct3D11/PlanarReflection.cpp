#include "PlanarReflection.h"



void PlanarReflection::render( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix, FLOAT time, Material::MaterialBuffer* mBuffer )
{
	pD3DDeviceContext->IASetInputLayout( _pInputLayout );
	pD3DDeviceContext->VSSetShader( _pVertexShader, nullptr, 0 );
	pD3DDeviceContext->PSSetShader( _pPixelShader, nullptr, 0 );

	setMatrixBuffer( pD3DDeviceContext, worldMatrix, viewMatrix, projectionMatrix, time );

	// Set Relfection Buffer here

	// -> Obsolete bc reflection will be set in material buffer now so you can prolly delete all this

	pD3DDeviceContext->PSSetShaderResources( 0, 1, &_pMainTexture );
	pD3DDeviceContext->PSSetSamplers( 0, 1, &_pMainSampler );
}
