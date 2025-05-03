#include "Emitter.h"
#include "Graphics.h"


using namespace DirectX;

Emitter::Emitter(
	int maxParticles,
	float maxParticleLifetime,
	int particlesPerEmit,
	float secondsPerEmit,
	std::shared_ptr<SimpleVertexShader> particleVS,
	std::shared_ptr<SimplePixelShader> particlePS) :
	maxParticles(maxParticles),
	maxParticleLifetime(maxParticleLifetime),
	particlesPerEmit(particlesPerEmit),
	secondsPerEmit(secondsPerEmit),
	particleVS(particleVS),
	particlePS(particlePS)
{
	// Set transform 
	transform = std::make_shared<Transform>();
	transform->SetPosition(0.0f, 0.0f, 0.0f);

	// Set up emitter properties
	indexFirstAlive = 0;
	indexFirstDead = 0;
	numLivingParticles = indexFirstDead - indexFirstAlive;
	timeSinceLastEmit = 0;

	// Delete existing resources
	//if (particles) delete[] particles;
	particleIndexBuffer.Reset();
	particleDataBuffer.Reset();
	particleDataSRV.Reset();

	// Initialize particle array
	particles = new Particle[maxParticles];
	ZeroMemory(particles, sizeof(Particle) * maxParticles);

	// Set up buffers
	CreateBuffers();
}

Emitter::~Emitter() 
{ 
	// Clear the particle array
	delete[] particles;
}

std::shared_ptr<Transform> Emitter::GetTransform() { return transform; }
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Emitter::GetParticleTexture() { return texture; }
int Emitter::GetIndexFirstAlive() { return indexFirstAlive; }
int Emitter::GetIndexFirstDead() { return indexFirstDead; }
int Emitter::GetNumLivingParticles() { return numLivingParticles; }
float Emitter::GetTimeSinceLastEmit() { return timeSinceLastEmit; }

void Emitter::SetParticleTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { this->texture = texture; }
void Emitter::SetSampler(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler) { this->sampler = sampler; }

void Emitter::CreateBuffers()
{
	// Make a dynamic buffer to hold all particle data on GPU
	// Note: We'll be overwriting this every frame with new lifetime data
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(Particle);
	desc.ByteWidth = sizeof(Particle) * maxParticles;
	Graphics::Device->CreateBuffer(&desc, 0, particleDataBuffer.GetAddressOf());

	// Create an SRV that points to a structured buffer of particles
	// so we can grab this data in a vertex shader
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = maxParticles;
	Graphics::Device->CreateShaderResourceView(particleDataBuffer.Get(), &srvDesc, particleDataSRV.GetAddressOf());

	// Set up data for index buffer
	unsigned int* indices = new unsigned int[maxParticles * 6];
	int indexCount = 0;
	for (int i = 0; i < maxParticles * 4; i += 4)
	{
		indices[indexCount++] = i;
		indices[indexCount++] = i + 1;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i + 3;
	}
	numIndices = indexCount;

	// Create the index buffer
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * (UINT)numIndices; // Number of indices
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData = {};
	initialIndexData.pSysMem = indices;
	Graphics::Device->CreateBuffer(&ibd, &initialIndexData, particleIndexBuffer.GetAddressOf());
}


void Emitter::EmitParticle(float currentTime)
{	
	// Check if we've spawned all the particles
	if (numLivingParticles == maxParticles)
		return;
	
	// Set initial particle values
	particles[indexFirstDead].EmitTime = currentTime;
	particles[indexFirstDead].StartPos = transform->GetPosition();

	// Increment particle counters
	indexFirstDead = (indexFirstDead + 1) % maxParticles;
	numLivingParticles += 1;
}

void Emitter::Update(float dt, float currentTime) 
{
	// Check particle ages
	for (int i = 0; i < numLivingParticles; i++)
	{
		int index = (indexFirstAlive + i) % maxParticles;
		if ((currentTime - particles[index].EmitTime) > maxParticleLifetime)
		{
			indexFirstAlive = (indexFirstAlive + 1) % maxParticles;
			numLivingParticles -= 1;
		}
	}
	
	timeSinceLastEmit += dt;
	while (timeSinceLastEmit > secondsPerEmit)
	{
		EmitParticle(currentTime);
		timeSinceLastEmit -= secondsPerEmit;
	}
}

void Emitter::Draw(float currentTime, std::shared_ptr<FPSCamera> camera)
{
	CopyResourcesToGPU();

	// Set index buffer, unbind vertex buffer
	UINT stride = 0;
	UINT offset = 0;
	ID3D11Buffer* nullBuffer = 0;
	Graphics::Context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	Graphics::Context->IASetIndexBuffer(particleIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Turn on shaders
	particleVS->SetShader();
	particlePS->SetShader();

	// Send data to the vertex shader
	particleVS->SetMatrix4x4("view", camera->GetView());
	particleVS->SetMatrix4x4("projection", camera->GetProjection());
	particleVS->SetFloat("currentTime", currentTime);
	particleVS->CopyAllBufferData();

	particleVS->SetShaderResourceView("ParticleData", particleDataSRV);

	// Send data to the pixel shader
	particlePS->SetShaderResourceView("ParticleTexture", texture);
	particlePS->SetSamplerState("BasicSampler", sampler);
	particlePS->CopyAllBufferData();

	// Draw this emitter
	Graphics::Context->DrawIndexed(numIndices, 0, 0);
}

void Emitter::CopyResourcesToGPU()
{
	// Map the buffer, locking it on the GPU so we can write to it
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	Graphics::Context->Map(particleDataBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	// How are living particles arranged in the buffer?
	if (indexFirstAlive < indexFirstDead)
	{
		// Only copy from FirstAlive -> FirstDead
		memcpy(
			mapped.pData, // Destination = start of particle buffer
			particles + indexFirstAlive, // Source = particle array, offset to first living particle
			sizeof(Particle) * numLivingParticles); // Amount = number of particles (measured in BYTES!)
	}
	else
	{
		// Copy from 0 -> FirstDead
		memcpy(
			mapped.pData, // Destination = start of particle buffer
			particles, // Source = start of particle array
			sizeof(Particle) * indexFirstDead); // Amount = particles up to first dead (measured in BYTES!)
		// ALSO copy from FirstAlive -> End
		memcpy(
			(void*)((Particle*)mapped.pData + indexFirstDead), // Destination = particle buffer, AFTER the data we copied in previous memcpy()
			particles + indexFirstAlive, // Source = particle array, offset to first living particle
			sizeof(Particle) * (maxParticles - indexFirstAlive)); // Amount = number of living particles at end of array (measured in BYTES!)
	}

	// Unmap (unlock) now that we're done with it
	Graphics::Context->Unmap(particleDataBuffer.Get(), 0);
}
