#include <Windows.h>
#include "utils.cpp"


bool gameRunning = true;
struct Render_State {
    int height, width;
    void* memory;

    BITMAPINFO bitmap_info;
};

Render_State render_state;
#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"
#include "movement.cpp"

#define process_button(b, vk)\
case vk: {\
inputs.buttons[b].changed = is_down!= inputs.buttons[b].is_down;\
inputs.buttons[b].is_down = is_down;\
} break;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            gameRunning = false;
        } break;

        case WM_SIZE: {
            RECT rectangle;
            GetClientRect(hwnd, &rectangle);
            render_state.height = rectangle.bottom - rectangle.top;
            render_state.width = rectangle.right - rectangle.left;

            int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);
            if (render_state.memory) {
                VirtualFree(render_state.memory, 0, MEM_RELEASE);
            }
            render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;


            typedef struct tagBITMAPINFOHEADER {
                DWORD biSize;
                LONG biWidth;
                LONG biHeight;
                WORD biPlanes;
                WORD biBitCount;
                DWORD biCompression;
                DWORD biSizeImage;
                LONG biXPelsPerMeter;
                LONG biYPelsPerMeter;
                DWORD biClrUsed;
                DWORD biClrImportant;
            } BITMAPINFOHEADER, * PBIMAPINFOHEADER;

        } break;
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Game Window Class";
    window_class.lpfnWndProc = window_callback;

    RegisterClass(&window_class);
    ShowCursor(false);
    HWND window = CreateWindow(
        window_class.lpszClassName,
        L"Pong",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720,
        0,
        0,
        hInstance,
        0);
    {
        //Fullscreen
        SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
        SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }

    HDC hdc = GetDC(window);

    Input inputs = {};
    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }

    while (gameRunning) {
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) {
            inputs.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (static_cast<long long>(1) << 31)) == 0);

                    switch (vk_code) {
                        process_button(BUTTON_UP, VK_UP);
                        process_button(BUTTON_DOWN, VK_DOWN); 
                        process_button(BUTTON_LEFT, VK_LEFT);
                        process_button(BUTTON_RIGHT, VK_RIGHT); 
                        process_button(BUTTON_ENTER, VK_RETURN);
                        process_button(BUTTON_W, 'W');
                        process_button(BUTTON_S, 'S');
                        process_button(BUTTON_P, 'P'); 
                        process_button(BUTTON_R, 'R');
                        process_button(BUTTON_Q, 'Q');
                    }
                } break;
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            } 
        }
        simulate_game(&inputs, delta_time);
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
        LARGE_INTEGER  frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;
    }

};
