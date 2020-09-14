#include "TextureManager.h"

TextureManager& TextureManager::RemoveTextureSRV(std::string id)
{
    srvRepository.erase(id);
    return *this;
}

TextureManager& TextureManager::AddTextureSRV(std::string id, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
    srvRepository.insert({ id, srv });
    return *this;
}

bool TextureManager::DoesExist(std::string id)
{
    if (srvRepository.find(id) != srvRepository.end())
    {
        return true;
    }
    return false;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::GetTextureSRV(std::string id)
{
    return srvRepository[id];
}
