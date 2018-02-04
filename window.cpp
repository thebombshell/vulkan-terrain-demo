
// /window.cpp

#include "window.hpp"

using namespace vk_terrain_demo;

LRESULT CALLBACK window_proc(HWND t_handle, UINT t_message, WPARAM t_w_param, LPARAM t_l_param) {
	
	switch (t_message) {
		
		case WM_CLOSE:
		
			PostQuitMessage(0);
		break;
	}
	return DefWindowProc(t_handle, t_message, t_w_param, t_l_param);
}

//
// window
//

window::window(LPCSTR t_title, int t_width, int t_height) {
	
	static TCHAR window_class_name[] = _T("win32app");
	
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = window_proc;
	window_class.hInstance = GetModuleHandle(0);
	window_class.hIcon = LoadIcon(window_class.hInstance, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = window_class_name;
	RegisterClass(&window_class);
	
	m_handle = CreateWindowEx
		( 0, window_class_name, "vulkan terrain demo", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720
		, NULL, NULL, window_class.hInstance, NULL);
	
	ShowWindow(m_handle, SW_SHOW);
}

window::~window() {
	
	DestroyWindow(m_handle);
}

void window::run() {
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND window::get_handle() {
	
	return m_handle;
}