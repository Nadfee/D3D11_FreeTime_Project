#pragma once

#include "DXDeviceManager.h"

#include <wrl/client.h>


using Microsoft::WRL::ComPtr;

class Mesh
{
public:
	Mesh();
	Mesh(ComPtr<ID3D11Buffer> vBuf, unsigned int elementStride, unsigned int vBufElementCount, 
		ComPtr<ID3D11Buffer> matBuf,
		ComPtr<ID3D11ShaderResourceView> textureSRV
	);
	~Mesh();

	void Draw(bool shouldDraw);
	void UpdateWorldMatrix(const Matrix& newMat);
	bool ShouldRender() { return renderOn; }

	void SetManagerKey(const std::size_t& key) { managerKey = key; }

	const ComPtr<ID3D11Buffer>& GetVertexBuffer() { return vertexBuffer; }
	const ComPtr<ID3D11Buffer>& GetWorldMatrixBuffer() { return worldMatrixBuffer; }
	const Matrix& GetWorldMatrix() { return objectMatrix; }
	const std::size_t& GetID() { return managerKey; }
	const ComPtr<ID3D11ShaderResourceView>& GetDiffusedTextureSRV() { return diffuseTextureSRV; }
	const UINT& GetStride() { return stride; }
	const UINT& GetOffset() { return offset; }
	const unsigned int& GetVertexCount() { return vertexElementCount; }


private:
	bool renderOn;
	std::size_t managerKey;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> worldMatrixBuffer;					// Maybe change to templated ConstantBuffer
	ComPtr<ID3D11ShaderResourceView> diffuseTextureSRV;
	UINT stride;
	UINT offset;
	Matrix objectMatrix;
	unsigned int vertexElementCount;



};

