#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <windows.h>
#include <memory>
#include <iostream>

#include "Renderer.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
	// 해상도 지정
	const int width = 1280, height = 720;
	
	// 윈도우 클래스 정보
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"Simple_RayTracer", // lpszClassName : 윈도우 클래스 이름
		NULL };

	RegisterClassEx(&wc);

	// 실제로 그려지는 해상도를 설정하기 위해
	RECT wr = { 0, 0, width, height };				   // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE); // adjust the size

	// 윈도우 핸들
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"Simple_RayTracer",
		WS_OVERLAPPEDWINDOW,
		100,				// 윈도우 좌측 상단의 x 좌표
		100,				// 윈도우 좌측 상단의 y 좌표
		wr.right - wr.left, // 윈도우 가로 방향 해상도
		wr.bottom - wr.top, // 윈도우 세로 방향 해상도
		NULL,
		NULL,
		wc.hInstance,
		NULL);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	auto renderer = std::make_unique<Renderer>(hwnd, width, height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.DisplaySize = ImVec2(width, height);
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplDX11_Init(renderer->device, renderer->deviceContext);
	ImGui_ImplWin32_Init(hwnd);

	// Main message loop
	// 매 프레임마다 반복되는 부분
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			// 광원 위치 조절
			ImGui::Begin("Light");
			ImGui::SliderFloat3("pos", &renderer->rayTracer.light.pos.x, -10.0f, 10.0f);
			ImGui::End();
			ImGui::Render();

			renderer->Update();
			renderer->Render();

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			// switch the back buffer and the front buffer
			renderer->swapChain->Present(1, 0);
		}
	}

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	renderer->Clean();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Windows procedure
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		// Reset and resize swapchain
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_MOUSEMOVE:
		// std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
		break;
	case WM_LBUTTONUP:
		// std::cout << "WM_LBUTTONUP Left mouse button" << std::endl;
		break;
	case WM_RBUTTONUP:
		// std::cout << "WM_RBUTTONUP Right mouse button" << std::endl;
		break;
	case WM_KEYDOWN:
		// std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
