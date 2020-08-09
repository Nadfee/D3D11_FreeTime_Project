#include "Mesh.h"

Mesh::Mesh() :
	stride(0),
	offset(0),
	vertexBuffer(nullptr),
	worldMatrixBuffer(nullptr),
	devCon(nullptr),
	vertexElementCount(0) {
}

Mesh::Mesh(ComPtr<ID3D11Buffer> vBuf, unsigned int elementStride, unsigned int elementCount,
	ComPtr<ID3D11Buffer> matBuf,
	DeviceContextPtr devCon) :
	stride(elementStride),
	offset(0),
	vertexBuffer(vBuf),
	worldMatrixBuffer(matBuf),
	devCon(devCon),
	vertexElementCount(elementCount)
{

}

Mesh::~Mesh()
{
}

void Mesh::Draw()
{
	devCon->VSSetConstantBuffers(0, 1, worldMatrixBuffer.GetAddressOf());
	devCon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devCon->Draw(vertexElementCount, 0);
}

void Mesh::UpdateWorldMatrix(Vector3 newPos)
{
	D3D11_MAPPED_SUBRESOURCE subres;
	HRESULT hr = devCon->Map(worldMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subres);

	Matrix newMat(
		Vector4(1.f, 0.f, 0.f, 0.f),
		Vector4(0.f, 1.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f),
		Vector4(newPos.x, newPos.y, newPos.z, 1.f)
	);

	Matrix* mat = (Matrix*)subres.pData;
	*mat = newMat;

	devCon->Unmap(worldMatrixBuffer.Get(), 0);
}
