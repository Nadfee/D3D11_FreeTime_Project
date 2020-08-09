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

private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> worldMatrixBuffer;
	ComPtr<ID3D11ShaderResourceView> diffuseTextureSRV;
	DeviceContextPtr devCon;
	UINT stride;
	UINT offset;
	unsigned int vertexElementCount;



};

