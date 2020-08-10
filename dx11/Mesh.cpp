#include "Mesh.h"

Mesh::Mesh() :
	stride(0),
	offset(0),
	vertexBuffer(nullptr),
	worldMatrixBuffer(nullptr),
	vertexElementCount(0),
	renderOn(false),
	managerKey(0),
	objectMatrix(Matrix::Identity)
{
}

Mesh::Mesh(ComPtr<ID3D11Buffer> vBuf, unsigned int elementStride, unsigned int elementCount,
	ComPtr<ID3D11Buffer> matBuf,
	ComPtr<ID3D11ShaderResourceView> textureSRV) :
	diffuseTextureSRV(textureSRV),
	stride(elementStride),
	offset(0),
	vertexBuffer(vBuf),
	worldMatrixBuffer(matBuf),
	vertexElementCount(elementCount),
	renderOn(false),
	managerKey(0),
	objectMatrix(Matrix::Identity)

{

}

Mesh::~Mesh()
{
}

void Mesh::Draw(bool shouldDraw)
{
	renderOn = shouldDraw;
}

void Mesh::UpdateWorldMatrix(const Matrix& newMat)
{
	objectMatrix = newMat;
}
