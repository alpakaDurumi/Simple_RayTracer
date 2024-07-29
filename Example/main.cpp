#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <windows.h>
#include <memory>
#include <iostream>

#include "Renderer.h"
#include "Texture.h"
#include "Object.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
	// �ػ� ����
	const int width = 1280, height = 720;
	
	// ������ Ŭ���� ����
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
		L"Simple_RayTracer", // lpszClassName : ������ Ŭ���� �̸�
		NULL };

	RegisterClassEx(&wc);

	// ������ �׷����� �ػ󵵸� �����ϱ� ����
	RECT wr = { 0, 0, width, height };				   // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE); // adjust the size

	// ������ �ڵ�
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"Simple_RayTracer",
		WS_OVERLAPPEDWINDOW,
		100,				// ������ ���� ����� x ��ǥ
		100,				// ������ ���� ����� y ��ǥ
		wr.right - wr.left, // ������ ���� ���� �ػ�
		wr.bottom - wr.top, // ������ ���� ���� �ػ�
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
	// �� �����Ӹ��� �ݺ��Ǵ� �κ�
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
			// ���� ��ġ ����
			ImGui::Begin("Light");
			ImGui::SliderFloat3("pos", &renderer->rayTracer.light.pos.x, -10.0f, 10.0f);
			ImGui::End();

			// �� object�� ���� �ؽ�ó ���ø� ����
			for (auto& obj : renderer->rayTracer.objects) {
				if (!obj->material->ambTexture) continue;

				ImGui::Begin("Texture Sampling Settings");

				const char* textureAddressModes[] = { "Clamp", "Wrap", "Mirror", "Border"};
				static const char* current_addressMode = "Clamp";
				int selectedIndex1 = 0; // ù ��° ��Ӵٿ� �޴��� �ʱ� ���� �ε���

				ImGui::SetNextItemWidth(150.0f); // 150�ȼ� �� ����
				if (ImGui::BeginCombo("texture addressing mode", current_addressMode)) {
					for (int n = 0; n < IM_ARRAYSIZE(textureAddressModes); n++) {
						bool is_selected = (current_addressMode == textureAddressModes[n]);
						if (ImGui::Selectable(textureAddressModes[n], is_selected))
							current_addressMode = textureAddressModes[n];
						
						if (current_addressMode == "Clamp")
							obj->material->ambTexture->SetAddressMode(TextureAddressMode::Clamp);
						else if (current_addressMode == "Wrap")
							obj->material->ambTexture->SetAddressMode(TextureAddressMode::Wrap);
						else if (current_addressMode == "Mirror")
							obj->material->ambTexture->SetAddressMode(TextureAddressMode::Mirror);
						else if (current_addressMode == "Border")
							obj->material->ambTexture->SetAddressMode(TextureAddressMode::Border);
							
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				const char* textureFilterModes[] = { "Nearest", "Bilinear" };
				static const char* current_filterMode = "Nearest";
				int selectedIndex2 = 0; // �� ��° ��Ӵٿ� �޴��� �ʱ� ���� �ε���

				ImGui::SetNextItemWidth(150.0f); // 150�ȼ� �� ����
				if (ImGui::BeginCombo("texture filtering mode", current_filterMode)) {
					for (int n = 0; n < IM_ARRAYSIZE(textureFilterModes); n++) {
						bool is_selected = (current_filterMode == textureFilterModes[n]);
						if (ImGui::Selectable(textureFilterModes[n], is_selected))
							current_filterMode = textureFilterModes[n];

						if (current_filterMode == "Nearest")
							obj->material->ambTexture->SetFilterMode(TextureFilterMode::Point);
						else if (current_filterMode == "Bilinear")
							obj->material->ambTexture->SetFilterMode(TextureFilterMode::Bilinear);

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::End();
			}

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
