#pragma once

#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"


class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);

	void RenderFrame();

private:
	bool InitializeDirectX(HWND hwnd, int width, int height);

	bool InitalizeShaders();

	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	
	//Microsoft::WRL::ComPtr<ID3D10Blob> vertex_shader_buffer;

	VertexShader vertexShader;
	PixelShader  pixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};

