#pragma once
#include <iostream>
#include <fstream>
#include <wtypes.h>
#include "Mesh.h"
#include <Vector>


using namespace std;

class Modelmporter
{
public:
	int init( char* filename, Mesh& mesh );
	
private:


	bool ReadFileCounts( char*, int&, int&, int&, int& );
	bool LoadDataStructures( char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, Mesh& mesh );
};

