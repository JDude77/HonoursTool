#include "Renderer.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION
#include "../Nuklear/nuklear.h"
#include "../Nuklear/nuklear_d3d11.h"

constexpr auto MAX_VERTEX_BUFFER = 512 * 1024;
constexpr auto MAX_INDEX_BUFFER = 128 * 1024;

static IDXGISwapChain* swapChain;
static ID3D11Device* device;
static ID3D11DeviceContext* context;
static ID3D11RenderTargetView* renderTargetView;

Renderer::Renderer(const std::string& windowName, const int windowWidth, const int windowHeight, const std::string& fontName, const int fontSize)
{
	InitWindow(windowName, windowWidth, windowHeight);
	
	InitD3D11(windowWidth, windowHeight);
	
	InitNuklear(windowWidth, windowHeight, fontName, fontSize);
}//End Renderer Constructor

Renderer::~Renderer()
{
	context->ClearState();
	nk_d3d11_shutdown();
	renderTargetView->Release();
	context->Release();
	device->Release();
	swapChain->Release();
	UnregisterClassW(windowClass_.lpszClassName, windowClass_.hInstance);
}//End Renderer Destructor

int Renderer::Update()
{
	int running = ProcessUserInput();

	/////////////////////////////////
	//Process things to render here//
	/////////////////////////////////
	if(nk_begin(nuklearContext_, "TEST", nk_rect(25, 50, 100, 100),
		    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		    NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		
	}//End if

	nk_end(nuklearContext_);
	
	DrawGUI();

	return running;
}//End Update

void Renderer::InitNuklear(const int windowWidth, const int windowHeight, const std::string& fontName, const int fontSize)
{
	nuklearContext_ = nk_d3d11_init(device, windowWidth, windowHeight, MAX_VERTEX_BUFFER, MAX_INDEX_BUFFER);
	
	nk_font_atlas* fontAtlas;
	const nk_font *font = nullptr;
	nk_d3d11_font_stash_begin(&fontAtlas);
	if(!fontName.empty())
	{
		const std::string fontPathTemp = "../../extra_font/" + fontName;
		const char* fontPath = fontPathTemp.c_str();
		font = nk_font_atlas_add_from_file(fontAtlas, fontPath, static_cast<float>(fontSize), nullptr);
	}//End if
	nk_d3d11_font_stash_end();
	if(font != nullptr)
	{
		nk_style_set_font(nuklearContext_, &font->handle);
	}//End if
	

	backgroundColour_ = new nk_colorf;
	backgroundColour_->r = 0.1f;
	backgroundColour_->g = 0.18f;
	backgroundColour_->b = 0.24f;
	backgroundColour_->a = 1.0f;
}//End InitNuklear

void Renderer::InitD3D11(int windowWidth, int windowHeight)
{
	memset(&swapChainDesc_, 0, sizeof(swapChainDesc_));
	swapChainDesc_.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc_.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc_.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc_.SampleDesc.Count = 1;
	swapChainDesc_.SampleDesc.Quality = 0;
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc_.BufferCount = 1;
	swapChainDesc_.OutputWindow = hwnd_;
	swapChainDesc_.Windowed = TRUE;
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc_.Flags = 0;

	if(FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION,
		&swapChainDesc_, &swapChain, &device, &featureLevel, &context)))
	{
		const HRESULT hresult = D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
			&swapChainDesc_, &swapChain, &device, &featureLevel, &context);
		assert(SUCCEEDED(hresult));
	}//End if
	SetSwapChainSize(windowWidth, windowHeight);
}//End InitD3D11

void Renderer::InitWindow(const std::string& windowName, const int windowWidth, const int windowHeight)
{
	windowRect_ = { 0, 0, windowWidth, windowHeight };

	memset(&windowClass_, 0, sizeof(windowClass_));
	windowClass_.style = CS_DBLCLKS;
	windowClass_.lpfnWndProc = WindowProc;
	windowClass_.hInstance = GetModuleHandleW(nullptr);
	windowClass_.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass_.lpszClassName = L"NuklearWindowClass";
	RegisterClassW(&windowClass_);

	AdjustWindowRectEx(&windowRect_, style_, FALSE, extendedStyle_);

	const std::wstring windowNameTemp = std::wstring(windowName.begin(), windowName.end());
	hwnd_ = CreateWindowExW(extendedStyle_, windowClass_.lpszClassName, windowNameTemp.c_str(),
	                       style_ | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
	                       windowRect_.right - windowRect_.left, windowRect_.bottom - windowRect_.top,
	                       nullptr, nullptr, windowClass_.hInstance, nullptr);
}//End InitWindow

int Renderer::ProcessUserInput()
{
	int stillRunning = 1;
	MSG message;
	nk_input_begin(nuklearContext_);
	while(PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
	{
		if(message.message == WM_QUIT)
		{
			stillRunning = 0;
		}//End if
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}//End while

	nk_input_end(nuklearContext_);

	return stillRunning;
}//End ProcessUserInput

void Renderer::DrawGUI()
{
	context->ClearRenderTargetView(renderTargetView, &backgroundColour_->r);
	context->OMSetRenderTargets(1, &renderTargetView, nullptr);
	nk_d3d11_render(context, NK_ANTI_ALIASING_ON);
	HRESULT hresult = swapChain->Present(1, 0);
	if(hresult == DXGI_ERROR_DEVICE_RESET || hresult == DXGI_ERROR_DEVICE_REMOVED)
	{
		MessageBoxW(nullptr, L"D3D11 device is lost or removed!", L"Error", 0);
	}//End if
	else if(hresult == DXGI_STATUS_OCCLUDED)
	{
		Sleep(10);
	}//End else if
	assert(SUCCEEDED(hresult));
}//End DrawGUI

void Renderer::SetSwapChainSize(const int width, const int height)
{
	ID3D11Texture2D* backBuffer;
	D3D11_RENDER_TARGET_VIEW_DESC desc;

	if(renderTargetView) renderTargetView->Release();

	context->OMSetRenderTargets(0, nullptr, nullptr);

	HRESULT hresult = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	if(hresult == DXGI_ERROR_DEVICE_REMOVED || hresult == DXGI_ERROR_DEVICE_RESET || hresult == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
	{
		MessageBoxW(nullptr, L"DXGI device is removed or reset!", L"Error", 0);
		exit(0);
	}//End if
	assert(SUCCEEDED(hresult));

	memset(&desc, 0, sizeof(desc));
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hresult = swapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)(&backBuffer));
	assert(SUCCEEDED(hresult));

	hresult = device->CreateRenderTargetView((ID3D11Resource*)backBuffer, &desc, &renderTargetView);
	assert(SUCCEEDED(hresult));

	backBuffer->Release();
}//End SetSwapChainSize

LRESULT Renderer::WindowProc(const HWND hwnd, const UINT message, const WPARAM wparam, const LPARAM lparam)
{
	switch(message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
			if(swapChain)
			{
				const int width = LOWORD(lparam);
				const int height = HIWORD(lparam);
				SetSwapChainSize(width, height);
				nk_d3d11_resize(context, width, height);
			}//End if
			break;

		default:
			break;
	}//End switch

	if(nk_d3d11_handle_event(hwnd, message, wparam, lparam)) return 0;

	return DefWindowProcW(hwnd, message, wparam, lparam);
}//End WindowProc