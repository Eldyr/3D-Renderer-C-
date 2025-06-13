#include "Model.h"
#include <algorithm>
#include <vector>
#include "Polygon3D.h"


Model::Model()
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();

}


//Copy Constructor
Model::Model(const Model& m)
{
	_polygons = m.GetPolygons();
	_vertices = m.GetVertices();
	_transform = m.GetTransform();

}

Model::~Model()
{

}

//Accessors and mutators
const std::vector<Polygon3D>& Model::GetPolygons() const
{
	return _polygons;
}

const std::vector<Vertex>& Model::GetVertices() const
{
	return _vertices;
}

const std::vector<Vertex>& Model::GetTransform() const
{
	return _transform;
}


size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex temp(x, y, z, 1);
	_vertices.push_back(temp);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D temp(i0, i1, i2);
	_polygons.push_back(temp);
}

void Model::ApplyTransformToLocalVertices(const Matrix &transform)
{
	_transform.clear();
	for(Vertex& LocalVertices : _vertices)
		_transform.push_back(transform * LocalVertices);
	
	
}		

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	for (Vertex& TransformedVertices : _transform)
		TransformedVertices = transform * TransformedVertices;
	
	
}

void Model::Dehomogenize()
{
	for (Vertex& dehomogenize : _transform)
		dehomogenize.Dehomogenize();
	
	
}

void Model::CalculateBackfaces(Camera camera)
{
	for(Polygon3D& Polygon : _polygons)
	//int PolygonsSize = int(_polygons.size());

	//for (int i = 0; i < PolygonsSize; i++)
	{
		//Indices
		int index = Polygon.GetIndex(0);
		int index1 = Polygon.GetIndex(1);
		int index2 = Polygon.GetIndex(2);

		//Vertices
		Vertex vertex0 = _transform[index];
		Vertex vertex1 = _transform[index1];
		Vertex vertex2 = _transform[index2];

		//Constructing vectors by subtracting vertices
		Vector3D vectorA  = vertex1.VecSub(vertex0);
		Vector3D vectorB = vertex2.VecSub(vertex0);

		//Calculate Cross Product 
		Vector3D normal = vectorA.CrossProduct(vectorB);

		//Creating an Eye Vector 
		Vector3D EyeVector = camera.GetPos().VecSub(vertex0);

		//Mark the polygons for culling
		float dotProduct = normal.DotProduct(EyeVector);
		Polygon.SetCull(dotProduct < 0);

		//Save the Normal
		Polygon.SetNormal(normal);
	}
}

void Model::Sort(void)
{
	std::sort(_polygons.begin(), _polygons.end(), [](Polygon3D const& a, Polygon3D const& b) { return a.GetAverageZ() > b.GetAverageZ(); });

}


Model& Model::operator=(const Model& rhs)
{
	//Only do the assignment if we are not assigning
	//to ourselves
	if (this != &rhs)
	{
		_polygons = rhs.GetPolygons();
		_vertices = rhs.GetVertices();
		_transform = rhs.GetTransform();
	}
	return *this;
}