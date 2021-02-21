#include "PlanarReflection.h"



int PlanarReflection::initReflection( ID3D11Device* pD3DDevice )
{
	//int error = createReflectionBuffer( pD3DDevice );
	//if (error != 0) return error;

	return 0;
}

void PlanarReflection::renderReflection( ID3D11DeviceContext* pD3DDeviceContext, ID3D11DepthStencilView* depthStencilView, XMFLOAT4X4* reflectionMatrix )
{

	pD3DDeviceContext->IASetInputLayout( _pInputLayout );
	pD3DDeviceContext->VSSetShader( _pVertexShader, nullptr, 0 );
	pD3DDeviceContext->PSSetShader( _pPixelShader, nullptr, 0 );

	//setMaterialBuffer( pD3DDeviceContext, &mBuf->WORLD_MATRIX, &mBuf->PARAM_FLOAT4_1, &mBuf->PARAM_FLOAT4_2, &mBuf->PARAM_FLOAT4_3, &mBuf->PARAM_FLOAT4_4 );

	// Set Relfection Buffer here
	//setReflectionBuffer( pD3DDeviceContext, reflectionMatrix );

	pD3DDeviceContext->PSSetShaderResources( 1, 1, &_pMainTexture ); // 1 -> Reflection
	pD3DDeviceContext->PSSetSamplers( 0, 1, &_pMainSampler );

}


