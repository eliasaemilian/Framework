#include "Skybox.h"
#include "Vertex.h"
#include <DDSTextureLoader.h>

void Skybox::init( ID3D11Device* dev )
{
    // Scene Initialization
    CreateSphere( dev, 10, 10 );

    // create vertex , pixel shader
    dev->CreateVertexShader( SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS );
    dev->CreatePixelShader( SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS );



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
    CreateDDSTextureFromFile( dev, L"cubemapSky", ( ID3D11Resource** )&SMTexture, ( ID3D11ShaderResourceView** )&SMViewDesc, 0, nullptr );




    // create rasterizer state
    D3D11_RASTERIZER_DESC cmdesc;
    cmdesc.CullMode = D3D11_CULL_NONE;
    dev->CreateRasterizerState( &cmdesc, &RSCullNone );
    // create depth stencil state
    D3D11_DEPTH_STENCIL_DESC dssDesc;
    ZeroMemory( &dssDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
    dssDesc.DepthEnable = true;
    dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    dev->CreateDepthStencilState( &dssDesc, &DSLessEqual );
}

void Skybox::CreateSphere( ID3D11Device* dev, int LatLines, int LongLines )
{
    NumSphereVertices = ( ( LatLines - 2 ) * LongLines ) + 2;
    NumSphereFaces = ( ( LatLines - 3 ) * ( LongLines ) * 2 ) + ( LongLines * 2 );

    float sphereYaw = 0.0f;
    float spherePitch = 0.0f;

    std::vector<Vertex> vertices;

    XMVECTOR currVertPos = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );

    vertices[0].position.x = 0.0f;
    vertices[0].position.y = 0.0f;
    vertices[0].position.z = 1.0f;

    for (DWORD i = 0; i < LatLines - 2; ++i)
    {
        spherePitch = ( i + 1 ) * ( 3.14 / ( LatLines - 1 ) );
        Rotationx = XMMatrixRotationX( spherePitch );
        for (DWORD j = 0; j < LongLines; ++j)
        {
            sphereYaw = j * ( 6.28 / ( LongLines ) );
            Rotationy = XMMatrixRotationZ( sphereYaw );
            currVertPos = XMVector3TransformNormal( XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), ( Rotationx * Rotationy ) );
            currVertPos = XMVector3Normalize( currVertPos );
            vertices[i * LongLines + j + 1].position.x = XMVectorGetX( currVertPos );
            vertices[i * LongLines + j + 1].position.y = XMVectorGetY( currVertPos );
            vertices[i * LongLines + j + 1].position.z = XMVectorGetZ( currVertPos );
        }
    }

    vertices[NumSphereVertices - 1].position.x = 0.0f;
    vertices[NumSphereVertices - 1].position.y = 0.0f;
    vertices[NumSphereVertices - 1].position.z = -1.0f;


    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof( Vertex ) * NumSphereVertices;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;

    ZeroMemory( &vertexBufferData, sizeof( vertexBufferData ) );
    vertexBufferData.pSysMem = &vertices[0];
    int hr = dev->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &sphereVertBuffer );


    std::vector<DWORD> indices( NumSphereFaces * 3 );

    int k = 0;
    for (DWORD l = 0; l < LongLines - 1; ++l)
    {
        indices[k] = 0;
        indices[k + 1] = l + 1;
        indices[k + 2] = l + 2;
        k += 3;
    }

    indices[k] = 0;
    indices[k + 1] = LongLines;
    indices[k + 2] = 1;
    k += 3;

    for (DWORD i = 0; i < LatLines - 3; ++i)
    {
        for (DWORD j = 0; j < LongLines - 1; ++j)
        {
            indices[k] = i * LongLines + j + 1;
            indices[k + 1] = i * LongLines + j + 2;
            indices[k + 2] = ( i + 1 ) * LongLines + j + 1;

            indices[k + 3] = ( i + 1 ) * LongLines + j + 1;
            indices[k + 4] = i * LongLines + j + 2;
            indices[k + 5] = ( i + 1 ) * LongLines + j + 2;

            k += 6; // next quad
        }

        indices[k] = ( i * LongLines ) + LongLines;
        indices[k + 1] = ( i * LongLines ) + 1;
        indices[k + 2] = ( ( i + 1 ) * LongLines ) + LongLines;

        indices[k + 3] = ( ( i + 1 ) * LongLines ) + LongLines;
        indices[k + 4] = ( i * LongLines ) + 1;
        indices[k + 5] = ( ( i + 1 ) * LongLines ) + 1;

        k += 6;
    }

    for (DWORD l = 0; l < LongLines - 1; ++l)
    {
        indices[k] = NumSphereVertices - 1;
        indices[k + 1] = ( NumSphereVertices - 1 ) - ( l + 1 );
        indices[k + 2] = ( NumSphereVertices - 1 ) - ( l + 2 );
        k += 3;
    }

    indices[k] = NumSphereVertices - 1;
    indices[k + 1] = ( NumSphereVertices - 1 ) - LongLines;
    indices[k + 2] = NumSphereVertices - 2;

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory( &indexBufferDesc, sizeof( indexBufferDesc ) );

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof( DWORD ) * NumSphereFaces * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;

    iinitData.pSysMem = &indices[0];
    dev->CreateBuffer( &indexBufferDesc, &iinitData, &sphereIndexBuffer );
}

void Skybox::update( float deltatime )
{
    ////Reset sphereWorld
    //sphereWorld = XMMatrixIdentity();

    ////Define sphereWorld's world space matrix
    //Scale = XMMatrixScaling( 5.0f, 5.0f, 5.0f );
    ////Make sure the sphere is always centered around camera
    //Translation = XMMatrixTranslation( XMVectorGetX( camPosition ), XMVectorGetY( camPosition ), XMVectorGetZ( camPosition ) );

    ////Set sphereWorld's world space using the transformations
    //sphereWorld = Scale * Translation;
}

void Skybox::render( ID3D11DeviceContext* devCon, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix )
{
    UINT stride = sizeof( Vertex );
    UINT offset = 0;

    //Set the spheres index buffer
    devCon->IASetIndexBuffer( sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    //Set the spheres vertex buffer
    devCon->IASetVertexBuffers( 0, 1, &sphereVertBuffer, &stride, &offset );


    //Set the WVP matrix and send it to the constant buffer in effect file
    XMMATRIX WVP = sphereWorld * XMLoadFloat4x4( viewMatrix) * XMLoadFloat4x4( projectionMatrix);
    //cbPerObj.WVP = XMMatrixTranspose( WVP );
    //cbPerObj.World = XMMatrixTranspose( sphereWorld );
    //devCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
    //devCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
    //Send our skymap resource view to pixel shader
    devCon->PSSetShaderResources( 0, 1, &smrv );
    //devCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

    //Set the new VS and PS shaders
    devCon->VSSetShader( SKYMAP_VS, 0, 0 );
    devCon->PSSetShader( SKYMAP_PS, 0, 0 );
    //Set the new depth/stencil and RS states
    devCon->OMSetDepthStencilState( DSLessEqual, 0 );
    devCon->RSSetState( RSCullNone );
    devCon->DrawIndexed( NumSphereFaces * 3, 0, 0 );

    //Set the default VS shader and depth/stencil state
    //devCon->VSSetShader( VS, 0, 0 );
    devCon->OMSetDepthStencilState( NULL, 0 );
}

void Skybox::deInit()
{
    sphereIndexBuffer->Release();
    sphereVertBuffer->Release();

    SKYMAP_VS->Release();
    SKYMAP_PS->Release();
    SKYMAP_VS_Buffer->Release();
    SKYMAP_PS_Buffer->Release();

    smrv->Release();

    DSLessEqual->Release();
    RSCullNone->Release();
}
