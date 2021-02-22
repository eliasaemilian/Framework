#pragma once
#include "Material.h"
class SkyboxMaterial :
    public Material
{

protected:
    virtual int createMainTextureAndSampler( ID3D11Device* pD3DDevice, LPCWSTR textureName ) override;

};

