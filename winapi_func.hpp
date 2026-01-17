#ifndef IKUN_WINAPI_FUNC_HPP
#define IKUN_WINAPI_FUNC_HPP

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define NOMINMAX

#include <windows.h>
#include <iostream>
#include <string>

namespace winapi
{
    // 显示消息窗口
    void msgbox(std::wstring title/*标题*/, std::wstring text/*内容*/)
    {
        MessageBox(NULL, text.c_str(), title.c_str(), MB_OK);
    }

    // 获取当前进程PID
    int get_pid()
    {
        return static_cast<int>(GetCurrentProcessId());
    }
}
#endif