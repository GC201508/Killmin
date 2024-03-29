#include "System.h"

LPDIRECT3D9             g_pD3D         = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice   = NULL;
Effect*			        g_effect       = NULL;
XInputMode*				XInput         = NULL;

#if debuGmodE
DebugFont*				Dfont          = NULL;
CStopwatch*				sw             = NULL;
#endif //__debuGmodE


extern void Init();
extern void Render();
extern void Update();
extern void Terminate();
//-----------------------------------------------------------------------------
// DirectXを初期化。
//-----------------------------------------------------------------------------
void InitD3D(HWND hWnd)
{
	
	g_effect = new Effect;
	XInput   = new XInputMode;

#if debuGmodE
	Dfont    = new DebugFont;
	sw       = new CStopwatch;
#endif //__debuGmodE

	//D3Dオブジェクトを作成する。
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// D3Dデバイスを作成するためのパラメータを設定する。
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed               = TRUE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat       = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//D3Dデバイスを作成する。
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice);

}
//-----------------------------------------------------------------------------
// メッセージプロシージャ。
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		if (g_pd3dDevice != NULL)
			g_pd3dDevice->Release();

		if (g_pD3D != NULL)
			g_pD3D->Release();
		Terminate();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//-----------------------------------------------------------------------------
// ウィンメイン。
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// ウィンドウクラスを登録。
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Killmin", NULL
	};
	RegisterClassEx(&wc);

	// ウィンドウを作成。
	HWND hWnd = CreateWindow(
		"Killmin"          , // 登録されているクラス名,
		"キルミンベイベー" , // ウィンドウ名,
		WS_OVERLAPPEDWINDOW, // ウィンドウスタイル,
		300                , // ウィンドウの横方向の位置,	
		100                , // ウィンドウの縦方向の位置,
		WindowSizeX        , // ウィンドウの幅,
		WindowSizeY        , // ウィンドウの高さ,
		NULL, NULL, wc.hInstance, NULL);
	// Direct3Dを初期化。
	InitD3D(hWnd);


	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	//ここからゲーム関係の初期化。
	Init();
	XInput->Init(0);

	// ゲームループ
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			XInput->Update();
			Update();
			Render();
		}
	}
	UnregisterClass("Shader Tutorial", wc.hInstance);
	return 0;
}
