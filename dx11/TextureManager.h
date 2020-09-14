#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include <wrl/client.h>

class TextureManager
{
public:
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	};

	TextureManager& RemoveTextureSRV(std::string id);
	TextureManager& AddTextureSRV(std::string id, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

	bool DoesExist(std::string id);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureSRV(std::string id);

private:
	TextureManager() { };

	TextureManager(TextureManager const& in) = delete;
	void operator=(TextureManager const& rhs) = delete;

private:
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> srvRepository;
};

