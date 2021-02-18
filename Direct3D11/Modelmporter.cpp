#include "Modelmporter.h"

// -> [ http://www.rastertek.com/dx11tut08.html ]
typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;


int Modelmporter::init( char* filename, Mesh& mesh )
{
	bool result;
	//char filename[256];
	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;


	// Read in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	result = ReadFileCounts( filename, vertexCount, textureCount, normalCount, faceCount );
	if (!result)
	{
		return -1;
	}

	// Display the counts to the screen for information purposes.
	cout << endl;
	cout << "Vertices: " << vertexCount << endl;
	cout << "UVs:      " << textureCount << endl;
	cout << "Normals:  " << normalCount << endl;
	cout << "Faces:    " << faceCount << endl;

	// Now read the data from the file into the data structures and then output it in our model format.
	result = LoadDataStructures( filename, vertexCount, textureCount, normalCount, faceCount, mesh );
	if (!result)
	{
		return -1;
	}

	// Notify the user the model has been converted.
	cout << "\nFile has been converted." << endl;
	cout << "\nDo you wish to exit (y/n)? ";
	cin >> garbage;

	

	return 0;
}



bool Modelmporter::ReadFileCounts( char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount )
{
	ifstream fin;
	char input;


	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open( filename );

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get( input );
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get( input );
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get( input );
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get( input );
		}

		// Start reading the beginning of the next line.
		fin.get( input );
	}

	// Close the file.
	fin.close();

	return true;
}

bool Modelmporter::LoadDataStructures( char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, Mesh& mesh )
{
	VertexType* vertices, * texcoords, * normals;
	//FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	vector<WORD> indices;
	vector<Vertex> vertOUT;


	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}

	FaceType* faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open( filename );

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get( input );
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get( input );

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;
				
				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get( input );
			if (input == ' ')
			{
				WORD i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0, i8 = 0, i9 = 0;
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get( input );
		}


		// Start reading the beginning of the next line.
		fin.get( input );
	}

	// Close the file.
	fin.close();


	int count = 0;
	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{

		vertOUT.push_back( Vertex( 0, 0, 0 ) );
		vertOUT.push_back( Vertex( 0, 0, 0 ) );
		vertOUT.push_back( Vertex( 0, 0, 0 ) );

		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;


		vertOUT[count].position.x = vertices[vIndex].x;
		vertOUT[count].position.y = vertices[vIndex].y;
		vertOUT[count].position.z = vertices[vIndex].z;

		vertOUT[count].uv.x = texcoords[tIndex].x;
		vertOUT[count].uv.y = texcoords[tIndex].y;

		vertOUT[count].normal.x = normals[nIndex].x;
		vertOUT[count].normal.y = normals[nIndex].y;
		vertOUT[count].normal.z = normals[nIndex].z;

		indices.push_back( (WORD)count );
		count++;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;


		vertOUT[count].position.x = vertices[vIndex].x;
		vertOUT[count].position.y = vertices[vIndex].y;
		vertOUT[count].position.z = vertices[vIndex].z;

		vertOUT[count].uv.x = texcoords[tIndex].x;
		vertOUT[count].uv.y = texcoords[tIndex].y;

		vertOUT[count].normal.x = normals[nIndex].x;
		vertOUT[count].normal.y = normals[nIndex].y;
		vertOUT[count].normal.z = normals[nIndex].z;

		indices.push_back( ( WORD )count );
		count++;


		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;


		vertOUT[count].position.x = vertices[vIndex].x;
		vertOUT[count].position.y = vertices[vIndex].y;
		vertOUT[count].position.z = vertices[vIndex].z;

		vertOUT[count].uv.x = texcoords[tIndex].x;
		vertOUT[count].uv.y = texcoords[tIndex].y;

		vertOUT[count].normal.x = normals[nIndex].x;
		vertOUT[count].normal.y = normals[nIndex].y;
		vertOUT[count].normal.z = normals[nIndex].z;

		indices.push_back( ( WORD )count );
		count++;

	}

	// Close the output file.
	fout.close();



	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}


	mesh.SetVertices( vertOUT, vertOUT.size() );
	mesh.SetIndices( indices, indices.size() );

	return true;
}
