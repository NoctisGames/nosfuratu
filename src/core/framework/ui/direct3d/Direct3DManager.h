//
//  Direct3DManager.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/17/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DManager__
#define __gowengamedev__Direct3DManager__

#define VERTICES_PER_RECTANGLE 4
#define INDICES_PER_RECTANGLE 6

#define D3DManager (Direct3DManager::getInstance())

#include "Direct3DProgram.h"
#include "DeviceResources.h"

#include <vector>
#include <memory>

class Direct3DTextureGpuProgramWrapper;
class Direct3DGeometryGpuProgramWrapper;
class Direct3DFrameBufferToScreenGpuProgramWrapper;

class Direct3DManager
{
public:
	static Direct3DManager * getInstance();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_offscreenRenderTarget; // the offscreen render target texture
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_offscreenRenderTargetView; // the offscreen render target interface
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_offscreenShaderResourceView; // this is needed for the screen pixel shader
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState; // the blend state interface
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_screenBlendState; // the blend state interface, but for rendering to the screen
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixConstantbuffer; // the matrix constant buffer interface
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_offsetConstantBuffer; // the offset constant buffer interface
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexbuffer; // the index buffer interface

	// Used in SpriteBatcher
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sbSamplerState; // the sampler state interfaces
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_sbVertexShader; // the vertex shader interface
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_sbPixelShader; // the pixel shader interface
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_sbSinWavePixelShader; // the pixel shader interface
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_sbInputLayout; // the input layout interface
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_sbVertexBuffer; // the vertex buffer interface
	std::vector<TEXTURE_VERTEX> m_textureVertices;

	// Also used in SpriteBatcher, but only for rendering the offscreen render target texture to the screen
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_fbVertexShader; // the vertex shader interface
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_fbPixelShader; // the pixel shader interface
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_fbInputLayout; // the input layout interface

	// Used in RectangleBatcher, LineBatcher, and CircleBatcher (Geometry)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_gbVertexShader; // the vertex shader interface
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_gbPixelShader; // the pixel shader interface
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_gbInputLayout; // the input layout interface
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_gbVertexBuffer; // the vertex buffer interface
	std::vector<COLOR_VERTEX> m_colorVertices;

	// All above rendering takes place inside this matrix
	DirectX::XMMATRIX m_matFinal;

	std::unique_ptr<Direct3DTextureGpuProgramWrapper> m_textureProgram;
	std::unique_ptr<Direct3DGeometryGpuProgramWrapper> m_colorProgram;
	std::unique_ptr<Direct3DFrameBufferToScreenGpuProgramWrapper> m_fbToScreenProgram;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	int m_iMaxBatchSize;
	int m_iNumShadersLoaded;
	bool m_areSpriteBatcherResourcesCreated;
	bool m_areGeometryBatcherResourcesCreated;

	void init(const std::shared_ptr<DX::DeviceResources>& deviceResources, int maxBatchSize);
	void createDeviceDependentResources();
	void createWindowSizeDependentResources();
	void releaseDeviceDependentResources();

	void createMatrix(float left, float right, float bottom, float top);

	// Called by Batchers
	void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v); 
	void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a);

	bool isLoaded();

private:
	void createBlendState();
	void createSamplerState();
	void createInputLayoutForSpriteBatcher();
	void createInputLayoutForGeometryBatcher();
	void createIndexBuffer();
	void createConstantBuffer();
	void createOffsetBuffer();

	std::vector<short> createIndexValues(int maxBatchSize);

	// ctor, copy ctor, and assignment should be private in a Singleton
	Direct3DManager();
	Direct3DManager(const Direct3DManager&);
	Direct3DManager& operator=(const Direct3DManager&);
};

#endif /* defined(__gowengamedev__Direct3DManager__) */