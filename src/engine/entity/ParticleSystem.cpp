#include "ParticleSystem.h"

#include "../nsepch.h"
#include "../servers/RenderServer.h"
#include "../servers/TimeServer.h"

NSE::ParticleSystem::ParticleSystem(size_t capacity, size_t stride, size_t count)
{
    _capacity = capacity;
    _count = count;
    _pariclesMesh = CreateObject<Mesh>(0, 1);
    _particlesDataBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Structured, stride, capacity, true);

    Initialize();
}

NSE::ParticleSystem::~ParticleSystem()
{
    Release();
}

void NSE::ParticleSystem::OnCreated()
{
    OnSetupParticles(_particlesDataBuffer->GetDataPointer(), _count);
    _particlesDataBuffer->UploadForce();
}

void NSE::ParticleSystem::RenderEntity(const NSE_Camera& camera)
{
    S_PID(_ParticlesData);

    if (this->renderingMaterial)
    {
        this->renderingMaterial->SetBuffer(PID__ParticlesData, _particlesDataBuffer);
    }

    RenderServer::Get()->DrawMesh(_pariclesMesh, this->renderingMaterial, this->GetModelMatrix(camera->position), camera);
}

void NSE::ParticleSystem::OnParticlesUpdate()
{
    size_t deltaFrameCount = 1;

    if (_simulationFramerate > 0)
    {
        _simulationTime += TimeServer::Get()->Delta();

        size_t targetFrameCount = floor(_simulationTime * (float)_simulationFramerate);

        deltaFrameCount = targetFrameCount - _simulatedFrameCount;

        if (deltaFrameCount > MAX_SIMULATION_FRAMES_PER_UPDATE)
        {
            deltaFrameCount = 3;
            _simulationTime = (float)((double)targetFrameCount / (double)_simulationFramerate);
        }
    }

    for (int i = 0; i < deltaFrameCount; i++)
    {
        OnProcessParticles(_particlesDataBuffer->GetDataPointer(), _count);
    }

    _particlesDataBuffer->UploadForce();
}

void NSE::ParticleSystem::Release()
{
    if (_particlesDataBuffer)
    {
        _particlesDataBuffer->Release();
    }

    if (_pariclesMesh)
    {
        _pariclesMesh->Release();
    }
}

void NSE::ParticleSystem::SetSeed(int seed)
{
    _seed = seed;
}

void NSE::ParticleSystem::SetCapacity(size_t capacity)
{
    _capacity = capacity;

    _pariclesMesh->Resize(0, (int)capacity * 6);
    _particlesDataBuffer->Resize(capacity);

    Initialize();
}

void NSE::ParticleSystem::SetSimulationFramerate(int framerate)
{

}

void NSE::ParticleSystem::SetRenderType(RenderType type)
{
    _renderType = type;

    Initialize();
}

void NSE::ParticleSystem::Initialize()
{
    // Initialize particles mesh
    auto quadIndices = RenderServer::Get()->GetPrimitiveQuadMesh()->GetIndices();
    uint32_t* indices = nullptr;

    switch (_renderType)
    {
        case RenderType::Billboard:

            if (_pariclesMesh->GetIndexCount() != (int)_capacity * 6)
            {
                _pariclesMesh->Resize(0, (int)_capacity * 6);
            }

            indices = _pariclesMesh->GetIndices();
            int j;

            for (int i = 0; i < _capacity; i++)
            {
                memcpy(indices + i * 6, quadIndices, sizeof(uint32_t) * 6);
                for (j = i * 6; j < (i + 1) * 6; j++)
                {
                    indices[j] += i * 4;
                }
            }

            break;

        case RenderType::Line:

            if (_pariclesMesh->GetIndexCount() != (int)_capacity * 2)
            {
                _pariclesMesh->Resize(0, (int)_capacity * 2);
            }

            indices = _pariclesMesh->GetIndices();

            for (int i = 0; i < _capacity * 2; i++)
            {
                indices[i] = i;
            }

            break;

        case RenderType::Point:

            if (_pariclesMesh->GetIndexCount() != (int)_capacity)
            {
                _pariclesMesh->Resize(0, (int)_capacity);
            }

            indices = _pariclesMesh->GetIndices();

            for (int i = 0; i < _capacity; i++)
            {
                indices[i] = i;
            }

            break;
    }

    _pariclesMesh->Upload();
}

void NSE::ParticleSystem::SetCount(size_t count)
{
    assert(count <= _capacity);
    _count = count;
}
