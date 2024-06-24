// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>

extern "C" {
    #include "BlackJack.h"
}


// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    //bool show_demo_window = true;
    //bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    const char* suit_names[] = {
    "Paus",
    "Diamantes",
    "Copas",
    "Espadas" };

    const char* rank_names[] = {
        "As",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "Valete",
        "Rainha",
        "Rei" };


    bool gameStarted = false;
    bool stand = false;
    bool playerWin = false;
    bool playerLose = false;
    bool playerTie = false;
    bool gameEnded = false;

    int32_t playerCardsQtd = 2;
    int32_t bankerCardsQtd = 1;
    int32_t banker_card_values, player_card_values;

    BlackJack__USER player1 = 1;
    BlackJack__USER banker = 0;
    BlackJack__SUIT player_suit[12];
    BlackJack__RANK player_rank[12];
    BlackJack__SUIT banker_suit[12];
    BlackJack__RANK banker_rank[12];
    BlackJack__USER winners[5];
    BlackJack__USER losers[5];

    int32_t winners_counter;
    int32_t losers_counter;


    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (gameStarted == false) {
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGui::SetNextWindowPos(ImVec2(0, 0)); // Ensure window starts at (0,0)

            {
                ImGui::Begin("Blackjack Game", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

                ImGui::Dummy(ImVec2(10.0f, 10.0f));
                ImGui::Text("Click em Jogar para iniciar o BlackJack");
                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                // Center the button
                ImVec2 window_size = ImGui::GetWindowSize();
                ImVec2 button_size = ImVec2(300, 40); // Larger button size

                // Style the button
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.5f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.3f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 10)); // Padding
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 20)); // Spacing

                ImGui::SetCursorPosX((window_size.x - button_size.x) / 2);
                ImGui::SetCursorPosY((window_size.y - button_size.y) / 2);
                if (ImGui::Button("Jogar", button_size)) {
                    gameStarted = true;
                    printf("iniciando jogo\n");

                    BlackJack__INITIALISATION();

                    BlackJack__add_player(player1);
                    BlackJack__initialize_table_with_banker(banker);

                    BlackJack__add_balance(player1, 100);

                    BlackJack__enter_table(player1);

                    BlackJack__bet(player1, 10);

                    BlackJack__shuffle_deck();

                    BlackJack__deal_initial_banker_cards();

                    // 2 é o indice do Jogador 1 na mesa
                    BlackJack__deal_initial_player_cards(2);

                }

                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar(3);

                ImGui::End();
            }
        }

        if (gameStarted) {
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGui::SetNextWindowPos(ImVec2(0, 0)); // Ensure window starts at (0,0)

            {
                ImGui::Begin("Blackjack Game", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);



                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                ImGui::Text("Voce esta jogando contra a mesa. Puxe as cartas e pare a qualquer momento");
                ImGui::Dummy(ImVec2(0.0f, 30.0f));
                ImGui::Text("Cartas da mesa:");
                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                BlackJack__show_banker_cards(banker_rank, banker_suit);

                for (int i = 0; i < bankerCardsQtd; ++i) {
                    ImGui::Text("%s de %s\n", rank_names[banker_rank[i]], suit_names[banker_suit[i]]);
                }

                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                BlackJack__show_banker_cards_values(&banker_card_values, banker);
                ImGui::Text("Valor das cartas da mesa: %d", banker_card_values);

                ImGui::Dummy(ImVec2(0.0f, 30.0f));
                ImGui::Text("Suas cartas:");
                ImGui::Dummy(ImVec2(0.0f, 30.0f));

                BlackJack__show_player_cards(player_rank, player_suit, player1);

                for (int i = 0; i < playerCardsQtd; ++i) {
                    ImGui::Text("%s de %s\n", rank_names[player_rank[i]], suit_names[player_suit[i]]);
                }

                ImGui::Dummy(ImVec2(0.0f, 10.0f));

                BlackJack__show_player_cards_values(&player_card_values, player1);
                ImGui::Text("Valor das suas cartas: %d", player_card_values);

                ImGui::Dummy(ImVec2(0.0f, 30.0f));


                // Center the buttons
                ImVec2 window_size = ImGui::GetWindowSize();
                ImVec2 button_size = ImVec2(300, 40); // Larger button size
                ImVec2 button_pos = ImVec2((window_size.x - button_size.x) / 2, ImGui::GetCursorPosY());

                // Style the buttons
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.5f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.3f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 10)); // Padding
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 20)); // Spacing

                if (!gameEnded && !stand) {
                    ImGui::SetCursorPos(button_pos);
                    if (ImGui::Button("Puxar", button_size)) {
                        // Ação para o botão "Puxar"
                        BlackJack__hit_player(player1);
                        BlackJack__show_player_cards(player_rank, player_suit, player1);
                        playerCardsQtd = playerCardsQtd + 1;

                        BlackJack__show_banker_cards_values(&banker_card_values, banker);
                        BlackJack__show_player_cards_values(&player_card_values, player1);
                        printf("Valor player: %d\n", player_card_values);
                        printf("Valor banker: %d\n", banker_card_values);

                        if(player_card_values > 21){
                            playerLose = true;
                            BlackJack__check_player_result(2, banker);

                            gameEnded = true;
                        }
                    }

                    button_pos.y += button_size.y + 20; // Adjust position for next button
                    ImGui::SetCursorPos(button_pos);
                    if (ImGui::Button("Parar", button_size)) {
                        stand = true;
                        BlackJack__stand_player(player1);

                        BlackJack__show_banker_cards_values(&banker_card_values, banker);
                        while (banker_card_values < 17) {
                            BlackJack__hit_banker(banker);
                            BlackJack__show_banker_cards(banker_rank, banker_suit);
                            BlackJack__show_banker_cards_values(&banker_card_values, banker);
                            bankerCardsQtd = bankerCardsQtd + 1;
                        }
                        
                        BlackJack__stand_banker(banker);

                        BlackJack__show_banker_cards_values(&banker_card_values, banker);
                        ImGui::Text("Valor banker após parar: %d", banker_card_values);

                        BlackJack__check_player_result(2, banker);

                   
                        BlackJack__show_winners(winners);
                        BlackJack__show_losers(losers);

                       
                        BlackJack__show_winners_counter(&winners_counter);

                        printf("Winners counter: %d\n", winners_counter);

                       
                        BlackJack__show_losers_counter(&losers_counter);

                        printf("Losers counter : % d\n", losers_counter);

                       
                        if (losers_counter == 1) {
                            playerLose = true;
                        }
                        else if (winners_counter == 1) {
                            playerWin = true;
                        }
                        else {
                            playerTie = true;
                        }

                        gameEnded = true;
                    }
                }

                if (playerWin && gameEnded) {
                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    ImGui::Text("Voce ganhou! :)");
                }

                if (playerLose && gameEnded) {


                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    ImGui::Text("Voce perdeu! :(");
                }

                if (playerTie && gameEnded) {
                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    ImGui::Text("Empate! ");
                }


                if (gameEnded) {
                    button_pos.y += button_size.y + 50;
                    ImGui::SetCursorPos(button_pos);
                    if (ImGui::Button("Jogar novamente", button_size)) {
                        stand = false;

                        BlackJack__end_game(banker);

                        gameEnded = false;
                        playerWin = false;
                        playerLose = false;
                        playerTie = false;
                        playerCardsQtd = 2;
                        bankerCardsQtd = 1;
                        banker_card_values = 0;
                        player_card_values = 0;

                        printf("iniciando novamente\n");
                        BlackJack__INITIALISATION();

                        BlackJack__add_player(player1);
                        BlackJack__initialize_table_with_banker(banker);

                        BlackJack__add_balance(player1, 100);

                        BlackJack__enter_table(player1);

                        BlackJack__bet(player1, 10);

                        BlackJack__shuffle_deck();

                        BlackJack__deal_initial_banker_cards();

                        // 2 é o indice do Jogador 1 na mesa
                        BlackJack__deal_initial_player_cards(2);
                    }
                }

             

                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar(3);

                ImGui::End();
            }
        }

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(20, 20);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.6f, 0.4f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.7f, 0.5f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.5f, 0.3f, 1.0f);
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
