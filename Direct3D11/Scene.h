#pragma once
#include "Gameobject.h"
#include "Camera.h"
#include <vector>

class Scene
{
public:
	void init( ID3D11Device* dev, ID3D11DeviceContext* devCon, Camera* cam );
	void initGO( ID3D11Device* dev, ID3D11DeviceContext* devCon, Camera* camera, char* filenameModel, Material* material, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale );
	void render();
	void update();
private:
	std::vector<Gameobject> sceneObjects = {};;
};

