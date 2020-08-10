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
		ComPtr<ID3D11ShaderResourceView> textureSRV,
		DeviceContextPtr devCon);
	~Mesh();

	void Draw();
	void UpdateWorldMatrix(const Matrix& newMat);
	bool ShouldRender() { return renderOn; }

	const ComPtr<ID3D11Buffer>& GetVertexBuffer() { return vertexBuffer; };
	const ComPtr<ID3D11Buffer>& GetWorldMatrixBuffer() { return worldMatrixBuffer; };
	const ComPtr<ID3D11ShaderResourceView>& GetDiffusedTextureSRV() { return diffuseTextureSRV; };
	const UINT& GetStride() { return stride; };
	const UINT& GetOffset() { return offset; };
	const unsigned int& GetVertexCount() { return vertexElementCount; };


private:
	bool renderOn;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> worldMatrixBuffer;			// Maybe change to templated ConstantBuffer
	ComPtr<ID3D11ShaderResourceView> diffuseTextureSRV;
	DeviceContextPtr devCon;
	UINT stride;
	UINT offset;
	unsigned int vertexElementCount;



};

