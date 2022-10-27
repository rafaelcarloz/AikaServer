#pragma once

#include <windows.h>

#include <stdio.h>
#include <string>

struct HttpResponse
{
    HttpResponse() : statusCode(0) {}
    void Reset()
    {
        text = "";
        header = L"";
        statusCode = 0;
        error = L"";
    }

    std::string text;
    std::wstring header;
    DWORD statusCode;
    std::wstring error;
};

class HttpRequest
{
public:
    HttpRequest(
        const std::wstring& domain,
        int port,
        bool secure,
        const std::wstring& user_agent = L"WinHttpClient",
        const std::wstring& proxy_username = L"",
        const std::wstring& proxy_password = L"",
        const std::wstring& server_username = L"",
        const std::wstring& server_password = L"");

    bool Get(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        HttpResponse& response);

    bool Post(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        const std::string& body,
        HttpResponse& response);

    bool Put(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        const std::string& body,
        HttpResponse& response);

    bool Delete(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        const std::string& body,
        HttpResponse& response);
};
