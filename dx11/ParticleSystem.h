#pragma once
#include "Renderer.h"
#include <math.h>

using RendererPtr = std::shared_ptr<Renderer>;

// Lets try to just draw something with Draw Indirect using the funneling method via structured buffer!
class ParticleSystem
{
	struct Particle
	{
		Vector3 pos;
		float lifetime;
		Vector3 color;
		float padding2;
	};

	struct ParticleCount
	{
		Vector4 numParticles;	// Note X is numParticles!
	};

public:
	ParticleSystem(RendererPtr renderer);
	~ParticleSystem();

	void Render(double deltaTime);

private:
	bool firstTime = true;
	bool simPhaseOne = false;

	unsigned int numParticleCount = 512;

	// Temporary buffer for simulation
	float counter;
	float wave;
	ComPtr<ID3D11Buffer> simulationStatBuffer;

	RendererPtr renderer;
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> gsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> csBlob;
	ComPtr<ID3D11VertexShader> vs;
	ComPtr<ID3D11GeometryShader> gs;
	ComPtr<ID3D11PixelShader> ps;
	ComPtr<ID3D11ComputeShader> cs;

	ComPtr<ID3D11Buffer> bufferA;					// Initial Consume Buffer (init data here)
	ComPtr<ID3D11Buffer> bufferB;					// Initial Append Buffer
	ComPtr<ID3D11UnorderedAccessView> bufferAUAV;
	ComPtr<ID3D11UnorderedAccessView> bufferBUAV;
	ComPtr<ID3D11ShaderResourceView> bufferASRV;
	ComPtr<ID3D11ShaderResourceView> bufferBSRV;

	// Non-owning pointers used for swapping
	ID3D11UnorderedAccessView* activeConsumeUAV;
	ID3D11UnorderedAccessView* activeAppendUAV;

	ID3D11ShaderResourceView* activeAppendSRV;		// Looks at the finalized simulation buffer - Used for Funneling! (Points to bufferA/bufferB SRV)

	ComPtr<ID3D11Buffer> particleCountBuffer;		// Used to send into the CS for Dynamic check of structure count DURING Dispatch!


	ComPtr<ID3D11Buffer> indirectArgsBuffer;

	// Not used (2) below
	ComPtr<ID3D11Buffer> vertexFunnelBuffer;
	ComPtr<ID3D11ShaderResourceView> vertexFunnelView;

	Vector3 position;
	Matrix worldMatrix;
	ComPtr<ID3D11Buffer> worldMatrixBuffer;
	ComPtr<ID3D11Buffer> viewMatrixBuffer;
	ComPtr<ID3D11Buffer> projectionMatrixBuffer;

	void LoadShaders();
	void CreateBuffers();
	void SwitchSimulationBuffers();

};

