#pragma once
#ifndef RENDERER
#define RENDERER

#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <string>

//Forward Declarations
struct nk_colorf;
struct nk_context;
class NuklearWindowManager;
class DataManager;

class Renderer
{
#pragma region Attributes
private:
	nk_context* nuklearContext_;
	nk_colorf* backgroundColour_;

	WNDCLASSW windowClass_{};
	RECT windowRect_{};
	DWORD style_ = WS_OVERLAPPEDWINDOW;
	DWORD extendedStyle_ = WS_EX_APPWINDOW;
	HWND hwnd_;
	DXGI_SWAP_CHAIN_DESC swapChainDesc_{};
	D3D_FEATURE_LEVEL featureLevel_;

	NuklearWindowManager* nuklearWindowManager_;
public:

#pragma endregion

#pragma region Behaviours
private:
	static void SetSwapChainSize(int width, int height);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	void InitNuklear(int windowWidth, int windowHeight, const std::string& fontName = "", int fontSize = 12);
	void InitD3D11(int windowWidth, int windowHeight);
	void InitWindow(const std::string& windowName, int windowWidth, int windowHeight);
	int ProcessUserInput();
	void DrawGUI();

public:
	Renderer(const std::string& windowName, const int windowWidth, const int windowHeight, shared_ptr<DataManager> dataManager, const std::string& fontName = "", const int fontSize = 12);
	~Renderer();

	int Update();
#pragma endregion
};
#endif