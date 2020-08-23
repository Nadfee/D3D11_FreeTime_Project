#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(RendererPtr renderer) :
	renderer(renderer),
	viewMatrixBuffer(renderer->viewMatrixBuffer),
	projectionMatrixBuffer(renderer->projectionMatrixBuffer)
{
	LoadShaders();
	CreateBuffers();



}

ParticleSystem::~ParticleSystem()
{


}

const UINT useInternalCount = (UINT)-1;
const UINT initCountZero = 0;
ID3D11UnorderedAccessView* const uavReset[2] = { NULL, NULL };
ID3D11ShaderResourceView* const srvReset[1] = { nullptr };
ID3D11Buffer* const bufferReset[3] = { nullptr, nullptr, nullptr };

void ParticleSystem::Render(double deltaTime)
{
	counter += deltaTime;
	renderer->MapUpdate(simulationStatBuffer, &counter, sizeof(float), D3D11_MAP_WRITE_DISCARD);


	auto devCon = renderer->GetDeviceContext();

	//devCon->ClearState();


	devCon->VSSetShader(vs.Get(), NULL, NULL);
	devCon->GSSetShader(gs.Get(), NULL, NULL);
	devCon->PSSetShader(ps.Get(), NULL, NULL);
	devCon->CSSetShader(cs.Get(), NULL, NULL);

	// Simulation Part
	if (firstTime)
	{
		devCon->CSSetUnorderedAccessViews(0, 1, &activeConsumeUAV, &numParticleCount);
	    devCon->CSSetUnorderedAccessViews(1, 1, &activeAppendUAV, &initCountZero);
		//devCon->CSSetUnorderedAccessViews(0, 1, bufferAUAV.GetAddressOf(), &numParticleCount);
		//devCon->CSSetUnorderedAccessViews(1, 1, bufferBUAV.GetAddressOf(), &initCountZero);
		firstTime = false;
	}
	else
	{
		devCon->CSSetUnorderedAccessViews(0, 1, &activeConsumeUAV, &useInternalCount);
		devCon->CSSetUnorderedAccessViews(1, 1, &activeAppendUAV, &useInternalCount);

		//devCon->CSSetUnorderedAccessViews(0, 1, bufferAUAV.GetAddressOf(), &useInternalCount);
		//devCon->CSSetUnorderedAccessViews(1, 1, bufferBUAV.GetAddressOf(), &useInternalCount);
	}

	devCon->CopyStructureCount(particleCountBuffer.Get(), 0, activeConsumeUAV);
	devCon->CSSetConstantBuffers(0, 1, particleCountBuffer.GetAddressOf());


	devCon->CSSetConstantBuffers(1, 1, simulationStatBuffer.GetAddressOf());

	devCon->Dispatch(1, 1, 1);

	
	// Unbind the UAVs
	devCon->CSSetUnorderedAccessViews(0, 2, uavReset, 0);

	// Get how many vertices in the append buffer and place into indirect args buffer
	//devCon->CopyStructureCount(indirectArgsBuffer.Get(), 0, bufferBUAV.Get());
	devCon->CopyStructureCount(indirectArgsBuffer.Get(), 0, activeAppendUAV);


	// Rendering Part
	//devCon->VSSetShaderResources(5, 1, vertexFunnelView.GetAddressOf());		// Debug purpose (to check that rendering works correctly)

	//devCon->VSSetShaderResources(5, 1, bufferBSRV.GetAddressOf()); // this is what we want
	devCon->VSSetShaderResources(5, 1, &activeAppendSRV); // this is what we want


	devCon->GSSetConstantBuffers(0, 1, worldMatrixBuffer.GetAddressOf());
	devCon->GSSetConstantBuffers(1, 1, viewMatrixBuffer.GetAddressOf());
	devCon->GSSetConstantBuffers(2, 1, projectionMatrixBuffer.GetAddressOf());

	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	devCon->IASetInputLayout(nullptr);

	devCon->DrawInstancedIndirect(indirectArgsBuffer.Get(), 0);

	// Clear 
	devCon->VSSetShaderResources(5, 1, srvReset);
	devCon->GSSetConstantBuffers(0, 3, bufferReset);

	devCon->VSSetShader(nullptr, NULL, NULL);
	devCon->GSSetShader(nullptr, NULL, NULL);
	devCon->PSSetShader(nullptr, NULL, NULL);
	devCon->CSSetShader(nullptr, NULL, NULL);

	SwitchSimulationBuffers();


}

void ParticleSystem::LoadShaders()
{
	renderer->LoadShaderBlob(L"ParticleVS.hlsl", "VSMAIN", "vs_5_0", vsBlob.GetAddressOf());
	renderer->LoadShaderBlob(L"ParticleGS.hlsl", "GSMAIN", "gs_5_0", gsBlob.GetAddressOf());
	renderer->LoadShaderBlob(L"ParticlePS.hlsl", "PSMAIN", "ps_5_0", psBlob.GetAddressOf());
	renderer->LoadShaderBlob(L"ParticleCS.hlsl", "CSMAIN", "cs_5_0", csBlob.GetAddressOf());

	HRESULT hr = renderer->GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, vs.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = renderer->GetDevice()->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, gs.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = renderer->GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, ps.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = renderer->GetDevice()->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), NULL, cs.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void ParticleSystem::CreateBuffers()
{
	// We will use this buffer to update
	worldMatrixBuffer = renderer->CreateConstantBuffer(nullptr, sizeof(Matrix), true, true);

	// We will just send one particle and create a quad in GS!
	std::vector<Particle> particles;

	float min = 0.f;
	float max = 5.f;
	for (int i = 0; i < numParticleCount; ++i)
	{
		Particle particle = {
			Vector3(i / 2.f, 0.f, 0.f),	// In the Middle
			min + (rand() % static_cast<int>(max - min + 1)), //(float)(rand() % 4) + 1.f ,
			Vector3(0.f, 1.f, 0.f),	// Green color
			0.f
		};

		particles.push_back(particle);
	}

	vertexFunnelBuffer = renderer->CreateStructuredBuffer(particles.data(), sizeof(Particle), particles.size(), true, true);

	vertexFunnelView = renderer->CreateBufferShaderResourceView(vertexFunnelBuffer.Get(), particles.size());

	D3D11_BUFFER_DESC indirectArgBufferDesc;
	indirectArgBufferDesc.ByteWidth = 4 * 4;
	indirectArgBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indirectArgBufferDesc.BindFlags = 0;
	indirectArgBufferDesc.CPUAccessFlags = 0;
	indirectArgBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	indirectArgBufferDesc.StructureByteStride = 0;

	// http://gpuexperiments.blogspot.com/2008/12/draw-indirect.html
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-drawinstanced
	struct IndirectDrawArg
	{
		UINT A = 0;		// One particle, number of vertices to draw
		UINT B = 1;		
		UINT C = 0;
		UINT D = 0;
	} args;

	//args.A = numParticleCount;		// Checking so that the runtime copy count works

	D3D11_SUBRESOURCE_DATA srArg = { 0 };
	srArg.pSysMem = &args;

	HRESULT hr = renderer->GetDevice()->CreateBuffer(&indirectArgBufferDesc, &srArg, indirectArgsBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	// Create Structured Buffers for Append/Consume
	bufferA = renderer->CreateAppendConsumeStructuredBuffer(particles.data(), sizeof(Particle), particles.size(), false, true);		// Init data
	bufferB = renderer->CreateAppendConsumeStructuredBuffer(nullptr, sizeof(Particle), particles.size(), false, true);				// no init data

	// Create UAV views to them (to enable Append/Consume functionality)
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = { };
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = particles.size();
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;

	hr = renderer->GetDevice()->CreateUnorderedAccessView(bufferA.Get(), &uavDesc, bufferAUAV.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = renderer->GetDevice()->CreateUnorderedAccessView(bufferB.Get(), &uavDesc, bufferBUAV.GetAddressOf());
	assert(SUCCEEDED(hr));

	// Create two shader resource views for bufferA and bufferB so we can peek at the data! (Funnel method)
	bufferASRV = renderer->CreateBufferShaderResourceView(bufferA.Get(), particles.size());
	bufferBSRV = renderer->CreateBufferShaderResourceView(bufferB.Get(), particles.size());

	// Initial condition! First frame, finalized simulation gets into the append buffer.
	activeConsumeUAV = bufferAUAV.Get();
	activeAppendUAV = bufferBUAV.Get();
	activeAppendSRV = bufferBSRV.Get();

	// Just buffer in the beginning. Don't move the particles around yet
	//activeConsumeUAV = bufferBUAV.Get();
	//activeAppendUAV = bufferAUAV.Get();
	//activeAppendSRV = bufferASRV.Get();

	particleCountBuffer = renderer->CreateConstantBuffer(nullptr, sizeof(ParticleCount), false, true);

	simulationStatBuffer = renderer->CreateConstantBuffer(nullptr, sizeof(float), true, true);
}

void ParticleSystem::SwitchSimulationBuffers()
{
	if (!simPhaseOne)
	{
		activeConsumeUAV = bufferBUAV.Get();
		activeAppendUAV = bufferAUAV.Get();
		activeAppendSRV = bufferASRV.Get();
		simPhaseOne = !simPhaseOne;
	}
	else
	{
		activeConsumeUAV = bufferAUAV.Get();
		activeAppendUAV = bufferBUAV.Get();
		activeAppendSRV = bufferBSRV.Get();
		simPhaseOne = !simPhaseOne;
	}

}
