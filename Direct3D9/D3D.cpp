#include "D3D.h"

int D3D::init(HWND hWnd, INT width, INT height, bool isWindowed)
{
    // 1. get Direct3D 9 interface
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (pD3D == nullptr) return 20;

    // 2. check for fixed-function pipeline capability
    D3DCAPS9 d3dCaps = {};
    UINT adapter = D3DADAPTER_DEFAULT;
    D3DDEVTYPE devType = D3DDEVTYPE_HAL;
    HRESULT hr = pD3D->GetDeviceCaps(adapter, devType, &d3dCaps);
    if (FAILED(hr)) return 22;

    DWORD vertexProcessingType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    if (d3dCaps.VertexProcessingCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vertexProcessingType = D3DCREATE_HARDWARE_VERTEXPROCESSING; // graphic card is supporting fixed-funxtion pipeline;

    // 3. set up presentation parameters
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Windowed = isWindowed;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    // 4. create Direct3D 9 device
    hr = pD3D->CreateDevice(
        adapter, // graphic card to use (here primary adapter)
        devType, // hardware or software renderer
        hWnd, // handle to window instance
        vertexProcessingType, // optional flags
        &d3dpp, // presentation parameters
        &_pD3DDevice
    );
    if (FAILED(hr)) return 24;

    pD3D->Release();
    pD3D = nullptr;

    return 0;
}

void D3D::beginScene(D3DCOLOR backgroundColor)
{
    // clear back buffer with solid color
    _pD3DDevice->Clear(
        0, nullptr, // regions to clear, 0 for whole back buffer
        D3DCLEAR_TARGET, // which buffer to clear, target == back buffer
        backgroundColor, 1.0f, 0 // clear values for back buffer, z-buffer & stencil buffer
    );

    _pD3DDevice->BeginScene();
}

void D3D::endScene()
{
    _pD3DDevice->EndScene();

    // swap back with front buffer
    _pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void D3D::deInit()
{
    if (_pD3DDevice != nullptr)
    {
        _pD3DDevice->Release();
        _pD3DDevice = nullptr;
    }
}
