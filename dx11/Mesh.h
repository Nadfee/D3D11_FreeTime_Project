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
		DeviceContextPtr devCon);
	~Mesh();

	void Draw();
	void UpdateWorldMatrix(Vector3 newPos);

private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> worldMatrixBuffer;
	ComPtr<ID3D11Texture2D> diffuseTexture;
	DeviceContextPtr devCon;
	UINT stride;
	UINT offset;
	unsigned int vertexElementCount;



};

