//
//
//
//
//
//

#if defined(WIN32)

#include <os/WindowWindows.h>
#include <thread>
#include <chrono>

namespace te {
	namespace os {

        WindowWindows::WindowWindows() :
            m_hwnd(NULL),
            m_pDirect2dFactory(NULL),
            m_pRenderTarget(NULL),
            m_pLightSlateGrayBrush(NULL),
            m_pCornflowerBlueBrush(NULL)
        {
        }


        WindowWindows::~WindowWindows()
        {
            if (m_pDirect2dFactory) delete m_pDirect2dFactory;
            if (m_pRenderTarget) delete m_pRenderTarget;
            if (m_pLightSlateGrayBrush) delete m_pLightSlateGrayBrush;
            if (m_pCornflowerBlueBrush) delete m_pCornflowerBlueBrush;

        }

        void WindowWindows::RunMessageLoop()
        {
            MSG msg;

            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        HRESULT WindowWindows::Initialize(int _bufferWidth, int _bufferHeight)
        {
            bufferWidth_ = _bufferWidth;
            bufferHeight_ = _bufferHeight;
            buffer_.initialize(bufferWidth_, bufferHeight_);
            t0 = std::chrono::high_resolution_clock::now();
            HRESULT hr;

            // Initialize device-indpendent resources, such
            // as the Direct2D factory.
            hr = CreateDeviceIndependentResources();

            if (SUCCEEDED(hr))
            {
                // Register the window class.
                WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
                wcex.style = CS_HREDRAW | CS_VREDRAW;
                wcex.lpfnWndProc = WindowWindows::WndProc;
                wcex.cbClsExtra = 0;
                wcex.cbWndExtra = sizeof(LONG_PTR);
                wcex.hInstance = HINST_THISCOMPONENT;
                wcex.hbrBackground = NULL;
                wcex.lpszMenuName = NULL;
                wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
                wcex.lpszClassName = "D2DWindowWindows";

                RegisterClassEx(&wcex);


                // Because the CreateWindow function takes its size in pixels,
                // obtain the system DPI and use it to scale the window size.
                FLOAT dpiX, dpiY;

                // The factory returns the current system DPI. This is also the value it will use
                // to create its own windows.
                m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);


                // Create the window.
                m_hwnd = CreateWindow(
                    "D2DWindowWindows",
                    "Direct2D Demo App",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
                    static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
                    NULL,
                    NULL,
                    HINST_THISCOMPONENT,
                    this
                );
                hr = m_hwnd ? S_OK : E_FAIL;
                if (SUCCEEDED(hr))
                {
                    SetTimer(m_hwnd,             // handle to main window 
                        1,            // timer identifier 
                        10,                 // 10-second interval 
                        (TIMERPROC)NULL);     // no timer callback 
                    ShowWindow(m_hwnd, SW_SHOWNORMAL);
                    UpdateWindow(m_hwnd);
                }
            }

            return hr;
        }

        HRESULT WindowWindows::CreateDeviceIndependentResources()
        {
            HRESULT hr = S_OK;

            // Create a Direct2D factory.
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

            return hr;
        }

        HRESULT WindowWindows::CreateDeviceResources()
        {
            HRESULT hr = S_OK;

            if (!m_pRenderTarget)
            {
                RECT rc;
                GetClientRect(m_hwnd, &rc);

                D2D1_SIZE_U size = D2D1::SizeU(
                    rc.right - rc.left,
                    rc.bottom - rc.top
                );

                // Create a Direct2D render target.
                hr = m_pDirect2dFactory->CreateHwndRenderTarget(
                    D2D1::RenderTargetProperties(),
                    D2D1::HwndRenderTargetProperties(m_hwnd, size),
                    &m_pRenderTarget
                );


                if (SUCCEEDED(hr))
                {
                    // Create a gray brush.
                    hr = m_pRenderTarget->CreateSolidColorBrush(
                        D2D1::ColorF(D2D1::ColorF::LightSlateGray),
                        &m_pLightSlateGrayBrush
                    );
                }
                if (SUCCEEDED(hr))
                {
                    // Create a blue brush.
                    hr = m_pRenderTarget->CreateSolidColorBrush(
                        D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
                        &m_pCornflowerBlueBrush
                    );
                }

                if (SUCCEEDED(hr))
                {
                    // Create a DirectWrite factory.
                    hr = DWriteCreateFactory(
                        DWRITE_FACTORY_TYPE_SHARED,
                        __uuidof(m_pDWriteFactory),
                        reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
                    );
                }
                if (SUCCEEDED(hr))
                {
                    // Create a DirectWrite text format object.
                    hr = m_pDWriteFactory->CreateTextFormat(
                        L"Verdana",
                        NULL,
                        DWRITE_FONT_WEIGHT_NORMAL,
                        DWRITE_FONT_STYLE_NORMAL,
                        DWRITE_FONT_STRETCH_NORMAL,
                        10,
                        L"", //locale
                        &m_pTextFormat
                    );
                }
                if (SUCCEEDED(hr))
                {
                    // Center the text horizontally and vertically.
                    m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

                    m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                }

                if (SUCCEEDED(hr)) {
                    hr = m_pRenderTarget->CreateSolidColorBrush(
                        D2D1::ColorF(D2D1::ColorF::Black),
                        &m_pBlackBrush
                    );
                }
            }

            return hr;
        }


        void WindowWindows::DiscardDeviceResources()
        {
            if (m_pRenderTarget) delete m_pRenderTarget;
            if (m_pLightSlateGrayBrush) delete m_pLightSlateGrayBrush;
            if (m_pCornflowerBlueBrush) delete m_pCornflowerBlueBrush;
        }

        LRESULT CALLBACK WindowWindows::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            LRESULT result = 0;

            if (message == WM_CREATE)
            {
                LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
                WindowWindows* pWindowWindows = (WindowWindows*)pcs->lpCreateParams;

                ::SetWindowLongPtrW(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<LONG_PTR>(pWindowWindows)
                );

                result = 1;
            }
            else
            {
                WindowWindows* pWindowWindows = reinterpret_cast<WindowWindows*>(static_cast<LONG_PTR>(
                    ::GetWindowLongPtrW(
                        hwnd,
                        GWLP_USERDATA
                    )));

                bool wasHandled = false;

                if (pWindowWindows)
                {
                    switch (message)
                    {
                    case WM_SIZE:
                    {
                        UINT width = LOWORD(lParam);
                        UINT height = HIWORD(lParam);
                        pWindowWindows->OnResize(width, height);
                    }
                    result = 0;
                    wasHandled = true;
                    break;

                    case WM_DISPLAYCHANGE:
                    {
                        InvalidateRect(hwnd, NULL, FALSE);
                    }
                    result = 0;
                    wasHandled = true;
                    break;
                    case WM_PAINT:
                    {
                        break;
                    }
                    case WM_TIMER:
                    {
                        pWindowWindows->OnRender();
                        ValidateRect(hwnd, NULL);
                    }
                    result = 0;
                    wasHandled = true;
                    break;

                    case WM_DESTROY:
                    {
                        PostQuitMessage(0);
                    }
                    result = 1;
                    wasHandled = true;
                    break;
                    }
                }

                if (!wasHandled)
                {
                    result = DefWindowProc(hwnd, message, wParam, lParam);
                }
            }

            return result;
        }

        HRESULT WindowWindows::OnRender()
        {
            HRESULT hr = S_OK;

            hr = CreateDeviceResources();
            if (SUCCEEDED(hr))
            {
                m_pRenderTarget->BeginDraw();

                m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

                m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
                D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
                // Draw a grid background.
                unsigned width = static_cast<unsigned>(rtSize.width);
                unsigned height = static_cast<unsigned>(rtSize.height);

                D2D1_BITMAP_PROPERTIES bmpProps = {
                    {DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE},
                    96,
                    96,
                };

                ID2D1Bitmap* bmp;
                buffer_.lock();
                hr = m_pRenderTarget->CreateBitmap(
                    D2D1::SizeU(bufferWidth_, bufferHeight_),
                    buffer_.frontBuffer(), // <<--- Wrong, see (a) below
                    bufferWidth_ * 4, // <<--- Close but wrong, see (b) below
                    bmpProps, // <<--- Wrong, see (c) below
                    &bmp);
                buffer_.unlock();

                if (SUCCEEDED(hr)) {
                    m_pRenderTarget->DrawBitmap(  bmp,
                        D2D1::RectF(0, 0, width, height), 
                        1.0f,
                        D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
                        D2D1::RectF(0, 0, bufferWidth_, bufferHeight_));
                }
                bmp->Release();

                auto t1 = std::chrono::high_resolution_clock::now();
                float fps = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
                t0 = t1;
                fps = 1 / fps * 1e6;
                std::string sFps = ("FPS: " + std::to_string(fps));
                wchar_t* cpFps = new wchar_t[sFps.size()];
                mbstowcs(cpFps, sFps.c_str(), sFps.size());

                m_pRenderTarget->DrawText(
                    cpFps,
                    sFps.size(),
                    m_pTextFormat,
                    D2D1::RectF(0, 0, 100, 100),
                    m_pBlackBrush
                );

                hr = m_pRenderTarget->EndDraw();
            }
            if (hr == D2DERR_RECREATE_TARGET)
            {
                hr = S_OK;
                DiscardDeviceResources();
            }
            return hr;
        }

        void WindowWindows::OnResize(UINT width, UINT height)
        {
            if (m_pRenderTarget)
            {
                // Note: This method can fail, but it's okay to ignore the
                // error here, because the error will be returned again
                // the next time EndDraw is called.
                m_pRenderTarget->Resize(D2D1::SizeU(width, height));
            }
        }
	}
}

#endif