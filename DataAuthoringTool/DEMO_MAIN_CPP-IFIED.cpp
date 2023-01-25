/* nuklear - 1.32.0 - public domain */
#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

#define MAX_VERTEX_BUFFER (512 * 1024)
#define MAX_INDEX_BUFFER (128 * 1024)

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION

#include "Nuklear/nuklear.h"
#include "Nuklear/nuklear_d3d11.h"

static IDXGISwapChain* swap_chain;
static ID3D11Device* device;
static ID3D11DeviceContext* context;
static ID3D11RenderTargetView* rt_view;

static void set_swap_chain_size(int width, int height)
{
	ID3D11Texture2D* back_buffer;
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	HRESULT hresult;

	if (rt_view) rt_view->Release();

	context->OMSetRenderTargets(0, NULL, NULL);

	hresult = swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	if (hresult == DXGI_ERROR_DEVICE_REMOVED || hresult == DXGI_ERROR_DEVICE_RESET || hresult == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
	{
		MessageBoxW(NULL, L"DXGI device is removed or reset!", L"Error", 0);
		exit(0);
	}
	assert(SUCCEEDED(hresult));

	memset(&desc, 0, sizeof(desc));
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hresult = swap_chain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&back_buffer);
	assert(SUCCEEDED(hresult));

	hresult = device->CreateRenderTargetView((ID3D11Resource*)back_buffer, &desc, &rt_view);
	assert(SUCCEEDED(hresult));

	back_buffer->Release();
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (swap_chain)
		{
			int width = LOWORD(lparam);
			int height = HIWORD(lparam);
			set_swap_chain_size(width, height);
			nk_d3d11_resize(context, width, height);
		}
		break;
	}

	if (nk_d3d11_handle_event(hwnd, msg, wparam, lparam))
		return 0;

	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

enum Templates
{
	NONE,
	TEMPLATE_A,
	TEMPLATE_B,
	TEMPLATE_C
};

int main(void)
{
	static int active = NONE;

	struct nk_context* ctx;
	struct nk_colorf bg;

	WNDCLASSW wc;
	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD exstyle = WS_EX_APPWINDOW;
	HWND hwnd;
	int running = 1;
	HRESULT hresult;
	D3D_FEATURE_LEVEL feature_level;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;

	/* Win32 */
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleW(0);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"NuklearWindowClass";
	RegisterClassW(&wc);

	AdjustWindowRectEx(&rect, style, FALSE, exstyle);

	hwnd = CreateWindowExW(exstyle, wc.lpszClassName, L"Tool Test Demo",
		style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, wc.hInstance, NULL);

	/* D3D11 setup */
	memset(&swap_chain_desc, 0, sizeof(swap_chain_desc));
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		&swap_chain, &device, &feature_level, &context)))
	{
		/* if hardware device fails, then try WARP high-performance
		   software rasterizer, this is useful for RDP sessions */
		hresult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP,
			NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swap_chain_desc,
			&swap_chain, &device, &feature_level, &context);
		assert(SUCCEEDED(hresult));
	}
	set_swap_chain_size(WINDOW_WIDTH, WINDOW_HEIGHT);

	/* GUI */
	ctx = nk_d3d11_init(device, WINDOW_WIDTH, WINDOW_HEIGHT, MAX_VERTEX_BUFFER, MAX_INDEX_BUFFER);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	/* Load Cursor: if you uncomment cursor loading please hide the cursor */
	{
		struct nk_font_atlas* atlas;
		nk_d3d11_font_stash_begin(&atlas);
		struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../extra_font/DroidSans.ttf", 14, 0);
		/*struct nk_font *robot = nk_font_atlas_add_from_file(atlas, "../../extra_font/Roboto-Regular.ttf", 14, 0);*/
		/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
		/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyClean.ttf", 12, 0);*/
		/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyTiny.ttf", 10, 0);*/
		/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../extra_font/Cousine-Regular.ttf", 13, 0);*/
		nk_d3d11_font_stash_end();
		/*nk_style_load_all_cursors(ctx, atlas->cursors);*/
		nk_style_set_font(ctx, &droid->handle);
	}

	/* style.c */
#ifdef INCLUDE_STYLE
/* ease regression testing during Nuklear release process; not needed for anything else */
#ifdef STYLE_WHITE
	set_style(ctx, THEME_WHITE);
#elif defined(STYLE_RED)
	set_style(ctx, THEME_RED);
#elif defined(STYLE_BLUE)
	set_style(ctx, THEME_BLUE);
#elif defined(STYLE_DARK)
	set_style(ctx, THEME_DARK);
#endif
#endif

	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
	while (running)
	{
		/* Input */
		MSG msg;
		nk_input_begin(ctx);
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = 0;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		nk_input_end(ctx);

		/* GUI */
		if (nk_begin(ctx, "Demo Member Window", nk_rect(25, 50, 850, 500),
		    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		    NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			//Template Selection Dropdown
			if(nk_tree_push(ctx, NK_TREE_TAB, "Template", NK_MAXIMIZED))
			{
				nk_layout_row_dynamic(ctx, 20, 4);
				active = nk_option_label(ctx, "None", active == 0) ? 0 : active;
				active = nk_option_label(ctx, "Template A", active == 1) ? 1 : active;
				active = nk_option_label(ctx, "Template B", active == 2) ? 2 : active;
				active = nk_option_label(ctx, "Template C", active == 3) ? 3 : active;
				nk_tree_pop(ctx);
			}

			//Fields Fill-Out Section
			if(nk_tree_push(ctx, NK_TREE_TAB, "Fields", NK_MAXIMIZED))
			{
				//Hard-coded values for sake of demonstration
				static char text[5][64];
				static int text_length[5];
				static int occupied = nk_false;
				struct nk_style_button inactiveValidateButton;

				//Most fields need label, input, and validate button
				nk_layout_row_dynamic(ctx, 24, 3);
				switch(active)
				{
					//Example Template A
					case 1:
						//Title
						nk_label(ctx, "Title - String (text)", NK_TEXT_LEFT);
						nk_edit_string(ctx, NK_EDIT_SIMPLE, text[0], &text_length[0], 64, nk_filter_default);
						//Validate - Presence Check
						if(nk_button_label(ctx, "Validate"))
						{
							if(text_length[0] != 0)
							{
								fprintf(stdout, "Title validated successfully.\n");
							}
							else
							{
								fprintf(stdout, "WARNING: Title field should be non-empty.\n");
							}
						}

						//Blurb
						nk_label(ctx, "Blurb - String (text)", NK_TEXT_LEFT);
						nk_edit_string(ctx, NK_EDIT_SIMPLE, text[1], &text_length[1], 64, nk_filter_default);
						//Validate - Length Check
						if(nk_button_label(ctx, "Validate"))
						{
							if((text_length[1] == 0) || (text_length[1] > 19 && text_length[1] < 41))
							{
								fprintf(stdout, "Blurb validated successfully.\n");
							}
							else
							{
								fprintf(stdout, "WARNING: Blurb field should contain between 20 and 40 characters.\n");
							}
						}
						break;

					//Example Template B
					case 2:
						//X-Coordinate
						nk_label(ctx, "X-Coordinate - Integer (whole number)", NK_TEXT_LEFT);
						nk_edit_string(ctx, NK_EDIT_SIMPLE, text[2], &text_length[2], 64, nk_filter_decimal);
						//Validate - Presence and Non-Negative Checks
						if(nk_button_label(ctx, "Validate"))
						{
							if(text_length[2] > 0)
							{
								fprintf(stdout, "X-Coordinate validated successfully.\n");
							}
							else if(text_length[2] == 0)
							{
								fprintf(stdout, "WARNING: X-Coordinate field should be non-empty.\n");
							}
							else if(atoi(text[2]) < 0)
							{
								fprintf(stdout, "WARNING: X-Coordinate field should be non-negative.");
							}
						}

						//Y-Coordinate
						nk_label(ctx, "Y-Coordinate - Integer (whole number)", NK_TEXT_LEFT);
						nk_edit_string(ctx, NK_EDIT_SIMPLE, text[3], &text_length[3], 64, nk_filter_decimal);
						//Validate - Presence and Non-Negative Checks
						if(nk_button_label(ctx, "Validate"))
						{
							if(text_length[3] > 0)
							{
								fprintf(stdout, "Y-Coordinate validated successfully.\n");
							}
							else if(text_length[3] == 0)
							{
								fprintf(stdout, "WARNING: Y-Coordinate field should be non-empty.\n");
							}
							else if(atoi(text[3]) < 0)
							{
								fprintf(stdout, "WARNING: Y-Coordinate field should be non-negative.");
							}
						}

						//Occupied Status
						nk_label(ctx, "Occupied - Boolean (true/false)", NK_TEXT_LEFT);
						nk_checkbox_label(ctx, "", &occupied);
						//No validation, as it is a simple check-box
						inactiveValidateButton = ctx->style.button;
		                ctx->style.button.normal = nk_style_item_color(nk_rgb(40,40,40));
		                ctx->style.button.hover = nk_style_item_color(nk_rgb(40,40,40));
		                ctx->style.button.active = nk_style_item_color(nk_rgb(40,40,40));
		                ctx->style.button.border_color = nk_rgb(60,60,60);
		                ctx->style.button.text_background = nk_rgb(60,60,60);
		                ctx->style.button.text_normal = nk_rgb(60,60,60);
		                ctx->style.button.text_hover = nk_rgb(60,60,60);
		                ctx->style.button.text_active = nk_rgb(60,60,60);
	                    nk_button_label(ctx, "Validate");
						ctx->style.button = inactiveValidateButton;
						break;

					case 3:
						nk_label(ctx, "No Validation - String (text)", NK_TEXT_LEFT);
						nk_edit_string(ctx, NK_EDIT_SIMPLE, text[4], &text_length[4], 64, nk_filter_default);
	                    inactiveValidateButton = ctx->style.button;
		                ctx->style.button.normal = nk_style_item_color(nk_rgb(40,40,40));
		                ctx->style.button.hover = nk_style_item_color(nk_rgb(40,40,40));
		                ctx->style.button.active = nk_style_item_color(nk_rgb(40,40,40));
		                ctx->style.button.border_color = nk_rgb(60,60,60);
		                ctx->style.button.text_background = nk_rgb(60,60,60);
		                ctx->style.button.text_normal = nk_rgb(60,60,60);
		                ctx->style.button.text_hover = nk_rgb(60,60,60);
		                ctx->style.button.text_active = nk_rgb(60,60,60);
	                    nk_button_label(ctx, "Validate");
						ctx->style.button = inactiveValidateButton;
						
						break;
				}
				nk_tree_pop(ctx);
			}

			if(nk_tree_push(ctx, NK_TREE_TAB, "Footer", NK_MAXIMIZED))
			{
				nk_layout_row_dynamic(ctx, 20, 1);
				nk_button_label(ctx, "Save");
				nk_button_label(ctx, "Load");
				nk_button_label(ctx, "Export");
				nk_button_label(ctx, "Delete Member");
				nk_tree_pop(ctx);
			}
		}//End nk_begin

		nk_end(ctx);


		/* Draw */
		context->ClearRenderTargetView(rt_view, &bg.r);
		context->OMSetRenderTargets(1, &rt_view, NULL);
		nk_d3d11_render(context, NK_ANTI_ALIASING_ON);
		hresult = swap_chain->Present(1, 0);
		if (hresult == DXGI_ERROR_DEVICE_RESET || hresult == DXGI_ERROR_DEVICE_REMOVED) {
			/* to recover from this, you'll need to recreate device and all the resources */
			MessageBoxW(NULL, L"D3D11 device is lost or removed!", L"Error", 0);
		}
		else if (hresult == DXGI_STATUS_OCCLUDED) {
			/* window is not visible, so vsync won't work. Let's sleep a bit to reduce CPU usage */
			Sleep(10);
		}
		assert(SUCCEEDED(hresult));
	}

	context->ClearState();
	nk_d3d11_shutdown();
	rt_view->Release();
	context->Release();
	device->Release();
	swap_chain->Release();
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
	return 0;
}