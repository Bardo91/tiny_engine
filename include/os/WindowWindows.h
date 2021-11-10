//
//
//
//
//
//


#if defined(WIN32)
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")

#include <chrono>
#include <string>

#include <core/SwapBuffer.h>

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


namespace te {
    namespace os {
        class WindowWindows {
        public:
            WindowWindows(const te::core::SwapBuffer& _buffer);
            ~WindowWindows();

            // Register the window class and call methods for instantiating drawing resources
            HRESULT Initialize();

            // Process and dispatch messages
            void RunMessageLoop();

        private:
            // Initialize device-independent resources.
            HRESULT CreateDeviceIndependentResources();

            // Initialize device-dependent resources.
            HRESULT CreateDeviceResources();

            // Release device-dependent resource.
            void DiscardDeviceResources();

            // Draw content.
            HRESULT OnRender();

            // Resize the render target.
            void OnResize(
                UINT width,
                UINT height
            );

            // The windows procedure.
            static LRESULT CALLBACK WndProc(
                HWND hWnd,
                UINT message,
                WPARAM wParam,
                LPARAM lParam
            );
        private:
            HWND m_hwnd;
            ID2D1Factory* m_pDirect2dFactory;
            ID2D1HwndRenderTarget* m_pRenderTarget;
            ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
            ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
            std::chrono::high_resolution_clock::time_point t0;

            IDWriteFactory* m_pDWriteFactory;
            IDWriteTextFormat* m_pTextFormat;
            ID2D1SolidColorBrush* m_pBlackBrush;


            const te::core::SwapBuffer& buffer_;
        };
    }
}

#endif