#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    if (!InitializeDirectX(hwnd, width, height))
    {
        return false;
    }

    if (!InitalizeShaders())
        return false;

    if (!InitializeScene())
        return false;

    return true;
}

void Graphics::RenderFrame()
{
    float bgcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);


    this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
    this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
    this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    this->deviceContext->Draw(3, 0);
    this->swapchain->Present(1, NULL);
}


// Initialize Directx 11
bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
    
    if (adapters.size() < 1)
    {
        ErrorLogger::Log("NO DXGI Adapter found.");
        return false;
    }
    
    
    // dxgi swap chain description
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Numerator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;




    HRESULT hr = D3D11CreateDeviceAndSwapChain(
                        adapters[0].pAdapter, // IDXGI Adapter
                        D3D_DRIVER_TYPE_UNKNOWN, //
                        NULL, // for software driver type
                        NULL, // flags for run time layers
                        NULL, // features level array,
                        0, //# of feature level in array
                        D3D11_SDK_VERSION,
                        &scd, // swapchain description
                        this->swapchain.GetAddressOf(), // swapchain address
                        this->device.GetAddressOf(), // Device Address
                        NULL, // Supported feature levle
                        this->deviceContext.GetAddressOf()); // device context address
    
                                                             
   // if failed to create device and swap chain
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to Create device and swapchain");
        return false;
    }
    
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backbuffer;

    hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backbuffer.GetAddressOf()));
    
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "GetBuffer failed.");
        return false;
    }


    // create render target view
    hr = this->device->CreateRenderTargetView(
        backbuffer.Get(),
        NULL,
        this->renderTargetView.GetAddressOf()
    );

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create render target view");
        return false;
    }

    this->deviceContext->OMSetRenderTargets(
          1,
          this->renderTargetView.GetAddressOf(),
          NULL    
    );


    // setup view port
    D3D11_VIEWPORT viewPort;
    ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.Width = width;
    viewPort.Height = height;

    // Set the ViewPort
    this->deviceContext->RSSetViewports(1, &viewPort);
    
    return true;
}

bool Graphics::InitalizeShaders()
{
    std::wstring shaderfolder = L"";

#pragma region DetermineShaderPath
    if (IsDebuggerPresent() == TRUE)
    {
#ifdef _DEBUG

    #ifdef _WIN64 //x64
            shaderfolder = L"..\\x64\\Debug\\";
    #else //x86 WIN32
            shaderfolder = L"..\\Debug\\";
    #endif

    #else  //Release Mode
    #ifdef _WIN64 //64
        shaderfolder = L"..\\x64\\Release\\";
    #else //x86 (Win32)
        shaderfolder = L"..\\Release\\"

    #endif


#endif



    }


    


    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {
            "POSITION", // The HLSL semantic associated with this element in shader input signature
            0, 
            DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 
            0, 
            0, 
            D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    UINT numOfElements = ARRAYSIZE(layout);

    // initialize vertexshader
    if (!vertexShader.Initialize(this->device, shaderfolder + L"vertexshader.cso", layout, numOfElements))
        return false;

    // initialize pixelshader
    if (!pixelShader.Initialize(this->device, shaderfolder + L"pixelshader.cso"))
        return false;
        

    
    return true;
}

bool Graphics::InitializeScene()
{
    Vertex v[] = {
       // Vertex( 0.0f,  0.0f),  // center Point
        Vertex(-0.1f,  0.0f),  // Left   Point
        Vertex( 0.0f,  0.1f),  // Right  Point
        Vertex( 0.1f,  0.0f)   // Top    Point     x......x.......x
    };

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = v;

    HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer");
        return false;
    }
    return true;
}
