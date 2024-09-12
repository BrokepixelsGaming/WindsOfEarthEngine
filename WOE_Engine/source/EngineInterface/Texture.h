#pragma once
///////////////////////////////////////////////////////////////
// Texture Template for interface within the engine. 
///////////////////////////////////////////////////////////////

template <class InternalType>
class _Texture
{
	InternalType m_texture;

public:
	_Texture() = default;
	_Texture(const _Texture&) = delete;
	_Texture(_Texture&&) = default;
	_Texture& operator=(const _Texture&) = delete;
	_Texture& operator=(_Texture&&) = default;
	~_Texture() = default;

	bool LoadFromFile(const char* filename) { return m_texture.LoadFromFile(filename); }
	bool LoadFromMemory(const std::byte* pRawData, size_t size) { return m_texture.LoadFromMemory(pRawData, size); }
	void Destroy() { m_texture.Destroy(); }

	void SetActive(unsigned long stage = 0) const { m_texture.SetActive(stage); }
	bool SetAlpha(unsigned char newVal, TextureAlphaMethod alphaMethod) { return m_texture.SetAlpha(newVal, alphaMethod); }

	unsigned int GetWidth() const { return m_texture.GetWidth(); }
	unsigned int GetHeight() const { return m_texture.GetHeight(); }
	unsigned long GetMemoryUsage() const { return m_texture.GetMemoryUsage(); }

	void* GetDeviceTexture() const { return m_texture.GetDeviceTexture(); }
};