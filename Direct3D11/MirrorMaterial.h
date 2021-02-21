#pragma once
#include "Material.h"
class MirrorMaterial :
    public Material
{
public:
    bool createReflectionTexture( ID3D11Device* pD3DDevice, LPCWSTR textureName, UINT width, UINT height ); // NO

    int createReflectionBuffer( ID3D11Device* pD3DDevice );
    void setReflectionBuffer( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* reflectionMatrix );

private:
    // textures and samplers
    ID3D11ShaderResourceView* _pReflectionTexture = nullptr;
    ID3D11SamplerState* _pReflectionSampler = nullptr;

    ID3D11Texture2D* m_renderTargetTexture;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11ShaderResourceView* m_shaderResourceView;

    ID3D11Buffer* _pReflectionBuffer = nullptr;
    struct ReflectionBuffer
    {
        XMFLOAT4X4 reflectionMatrix;
    };

};

