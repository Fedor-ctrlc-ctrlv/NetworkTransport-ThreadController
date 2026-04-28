#pragma once

#ifdef NETWORKLIBCHERKASOV_EXPORTS
#define NETWORKLIB_API __declspec(dllexport)
#else
#define NETWORKLIB_API __declspec(dllimport)
#endif

#include <string>

struct MsgStructCherkasov {
    int to;
    int msgType;
    wchar_t* data;
};

extern "C" {
    NETWORKLIB_API bool __stdcall ConnectToServer(const char* host, int port);
    NETWORKLIB_API void __stdcall DisconnectFromServer();
    NETWORKLIB_API void __stdcall SendMsgFromSharp(MsgStructCherkasov msg);
    NETWORKLIB_API bool __stdcall IsConnected();
    NETWORKLIB_API bool __stdcall ReceiveMessageFromServer(MsgStructCherkasov* outMsg);
}