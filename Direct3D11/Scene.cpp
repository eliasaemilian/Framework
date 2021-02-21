#include "Scene.h"
#include "Modelmporter.h"
#include "SkyboxMaterial.h"
#include "PlanarReflection.h"

int Scene::init( ID3D11Device* devIN, ID3D11DeviceContext* devConIN, ID3D11DepthStencilView*  depthStencilViewIN, FLOAT width, FLOAT height )
{
	// STORE DEPENDENCIES
	dev = devIN;
	devCon = devConIN;
	depthStencilView = depthStencilViewIN;

	// INIT GLOBAL SHADER PARAMETER BUFFER
	createGlobalParamsBuffer( dev );

	// INIT CAMERA
	initCamera(width, height);

	// INIT PLANAR REFLECTIONS
	initPlanarReflection( width, height );


	// INIT LIGHT
	initLight();

	// INIT MESHES
	initMesh( ( char* )"inSphere.obj" );
	initMesh( ( char* )"Obelisk2.obj" );
	initMesh( ( char* )"waterplane2.obj" );


	// INIT MATERIALS
	initDDSMaterial( L"cubemapSky.DDS", L"VS_Skybox.hlsl", L"PS_Skybox.hlsl" );
	initMaterial( L"Obelisk_low_Obelisk_BaseColor.png", L"LightVertexShader.hlsl", L"LightPixelShader.hlsl" );
	//initMaterial( L"water_normals.jpg", L"VS_WaterShader.hlsl", L"PS_WaterShader.hlsl" );
	initMaterial( L"water_normals.jpg", L"VS_WaterShader.hlsl", L"PS_WaterShader.hlsl" );
	//initMaterial( L"water_normals.jpg", L"VS_PlanarReflection.hlsl", L"PS_PlanarReflection.hlsl" ); // BUILD THESE TODO

//	initPlanarReflectionMaterial( L"water_normals.jpg", L"VS_PlanarReflection.hlsl", L"PS_PlanarReflection.hlsl" );

	// MATERIAL PARAMS
	XMFLOAT4 mDataSkybox[4] = {};
	XMFLOAT4 mDataObelisk[4] = {};
	XMFLOAT4 mDataWater[4] = {};

	// WATER
	//mDataWater[0] = XMFLOAT4()


	// INIT GAMEOBJECTS
	// INDEX	| POSITION					| ROTATION						| SCALE
	initGO( 0, XMFLOAT3( 0.5f, -1.5f, 2.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 0.8f, 0.8f, 0.8f ), mDataSkybox );			// [ 0 ] SKYBOX
	initGO( 1, XMFLOAT3( 0.0f, -12.0f, 40.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ), mDataObelisk );			// [ 1 ] OBELISK
	initGO( 2, XMFLOAT3( 0.0f, -3.5f, 2.0f ), XMFLOAT3( -0.1f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ), mDataWater );			// [ 2 ] WATER



	return 0;

}

void Scene::initGO( int index, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 MatData_FLOAT[4] )
{
	Gameobject* model = new Gameobject();
	sceneObjects.push_back( model );
	model->init( dev, devCon, camera, materials[index], meshes[index] );
	model->SetRotation( rot );
	model->SetScale( scale );
	model->SetPosition( pos );

	// INIT MATERIAL DATA
	initMaterialData( *model->getWorldMatrix(), MatData_FLOAT[0], MatData_FLOAT[1], MatData_FLOAT[2], MatData_FLOAT[3] );

}

void Scene::initMesh( char* filenameModel )
{
	Mesh* mesh = new Mesh();
	meshes.push_back( mesh );
	Modelmporter modelimporter;
	modelimporter.init( filenameModel, *mesh );
	mesh->init( dev );

}

void Scene::initMaterialData( XMFLOAT4X4 mWorld, XMFLOAT4 f1, XMFLOAT4 f2, XMFLOAT4 f3, XMFLOAT4 f4 )
{
	Material::MaterialBuffer* mBuf = new Material::MaterialBuffer();
	materialData.push_back( mBuf );

	mBuf->WORLD_MATRIX = mWorld;


	mBuf->PARAM_FLOAT4_1 = f1;
	mBuf->PARAM_FLOAT4_2 = f2;
	mBuf->PARAM_FLOAT4_3 = f3;
	mBuf->PARAM_FLOAT4_4 = f4;
}

void Scene::initMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new Material();
	materials.push_back( mat );

	mat->init( dev, textureName, vertexShader, pixelShader );
	mat->setLight( devCon, *light );
}

void Scene::initDDSMaterial( LPCWSTR textureName, LPCWSTR vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new SkyboxMaterial();
	materials.push_back( mat );

	mat->init( dev, textureName, vertexShader, pixelShader );
	mat->setLight( devCon, *light );
}

void Scene::initPlanarReflection( UINT width, UINT height )
{
	// INIT TEXTURE (?)
	initRTexture( width, height );



	// INIT BUFFER
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof( ReflectionBuffer );
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer( &desc, nullptr, &_pReflectionBuffer );

}

bool Scene::initRTexture(UINT width, UINT height)
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
	result = dev->CreateTexture2D( &textureDesc, NULL, &m_renderTargetTexture );
	if (FAILED( result ))
	{
		return false;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = dev->CreateRenderTargetView( m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView );
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
	result = dev->CreateShaderResourceView( m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView );
	if (FAILED( result ))
	{
		return false;
	}

	return true;
}


void Scene::setPlanarReflection()
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = devCon->Map( _pReflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data );
	if (FAILED( hr )) return;

	XMMATRIX mReflect = XMLoadFloat4x4( camera->getReflectionMatrix() );

	mReflect = XMMatrixTranspose( mReflect );


	ReflectionBuffer* pBuffer = reinterpret_cast< ReflectionBuffer* >( data.pData );
	XMStoreFloat4x4( &pBuffer->reflectionMatrix, mReflect );

	devCon->Unmap( _pReflectionBuffer, 0 );

	devCon->VSSetConstantBuffers( 2, 1, &_pReflectionBuffer );

	setPlanarReflectionTexture();
}

void Scene::setPlanarReflectionTexture( )
{
	devCon->PSSetShaderResources( 1, 1, &m_shaderResourceView ); // but set the fucking texture 
	
}

void Scene::initMirrorMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader )
{
	MirrorMaterial* mat = new MirrorMaterial();
	pMirrorMaterial = mat;
	materials.push_back( mat );

	
	mat->init( dev, textureName, vertexShader, pixelShader );
	//mat->createReflectionBuffer( dev );
}

void Scene::render( FLOAT time )
{
	// SET GLOBAL PARAMETERS
	setGlobalParameters( devCon, camera->getViewMatrix(), camera->getProjectionMatrix(), time );
	
	setPlanarReflection();

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->render( devCon, materialData[i] );
	}

	//sceneObjects[0]->render( devCon, materialData[0] );


}

void Scene::renderPrecall( FLOAT time )
{
	// RENDER REFLECTIONS TO TEXTURE

	// set render target to texture
	devCon->OMSetRenderTargets( 1, &m_renderTargetView, depthStencilView );

	// clear render target
	const FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devCon->ClearRenderTargetView( m_renderTargetView, color );

	// RENDER SCENE OBJECTS (NOT WATER & PASS REFLECTION MATRIX AS VIEW MATRIX
	camera->RenderReflection( -3.5f );
	setGlobalParameters( devCon, camera->getReflectionMatrix(), camera->getProjectionMatrix(), time );
	for (int i = 0; i < sceneObjects.size() - 1; i++)
	{
		sceneObjects[i]->render( devCon, materialData[i] );
	}

}

void Scene::update( FLOAT deltatime )
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->update( deltatime );
	}
}

int Scene::createGlobalParamsBuffer( ID3D11Device* pD3DDevice )
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof( GlobalParameters );
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer( &desc, nullptr, &_globalParamsBuffer );
	if (FAILED( hr )) return 46;

	return 0;
}


void Scene::initLight()
{
	light = new Light();
	light->LightDirection = { 0.0f, 1.0f, 0.0f };
	light->AmbientColor = { 0.9f, 1.0f, 1.0f, 1.0f };
	light->DiffuseColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	light->LightIntensity = 0.5f;
}

void Scene::initCamera( FLOAT width, FLOAT height )
{
	XMFLOAT3 camPos = { 0.0f, 0.0f, -1.0f };

	camera = new Camera();
	camera->init( width, height, camPos );
}

void Scene::deInit()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
		meshes[i]->deInit();
		delete meshes[i];
		materials[i]->deInit();
		delete materials[i];
	}

	delete light;

	camera->deInit();
	delete camera;
}

void Scene::setGlobalParameters( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* view, XMFLOAT4X4* projection, FLOAT time )
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map( _globalParamsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data );
	if (FAILED( hr )) return;

	XMMATRIX viewMatrix = XMLoadFloat4x4( view );
	XMMATRIX projectionMatrix = XMLoadFloat4x4( projection );


	XMVECTOR t = XMVectorSet( time, time, time, time );

	viewMatrix = XMMatrixTranspose( viewMatrix );
	projectionMatrix = XMMatrixTranspose( projectionMatrix );

	GlobalParameters* pBuffer = reinterpret_cast< GlobalParameters* >( data.pData );
	XMStoreFloat4x4( &pBuffer->CameraViewMatrix, viewMatrix );
	XMStoreFloat4x4( &pBuffer->CameraProjectionMatrix, projectionMatrix );
	XMStoreFloat4( &pBuffer->time, t );

	pD3DDeviceContext->Unmap( _globalParamsBuffer, 0 );

	pD3DDeviceContext->VSSetConstantBuffers( 0, 1, &_globalParamsBuffer );
}
