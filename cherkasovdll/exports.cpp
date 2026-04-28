#include "pch.h"
#include "exports.h"
#include "NetworkTransportCherkasov.h"
#include <objbase.h>


static std::unique_ptr<ClientTransportCherkasov> g_client = nullptr;

bool __stdcall ConnectToServer(const char* host, int port) {
    g_client = std::make_unique<ClientTransportCherkasov>();
    return g_client->connect(host, port);
}

void __stdcall DisconnectFromServer() {
    if (g_client) {
        g_client->disconnect();
        g_client = nullptr;
    }
}

void __stdcall SendMsgFromSharp(MsgStructCherkasov msg) {
    if (g_client) {
        MessageCherkasov m(msg.to, static_cast<MessageTypeCherkasov>(msg.msgType),
            std::wstring(msg.data ? msg.data : L""));
        g_client->send(m);
    }
}

bool __stdcall IsConnected() {
    return g_client && g_client->is_connected();
}

bool __stdcall ReceiveMessageFromServer(MsgStructCherkasov* outMsg) {
    if (!g_client || !outMsg) return false;
    try {
        MessageCherkasov msg;
        if (g_client->receive(msg)) {
            outMsg->to = msg.header.to;
            outMsg->msgType = static_cast<int>(msg.header.messageType);
            int length = static_cast<int>(msg.data.length()) + 1;
            outMsg->data = (wchar_t*)CoTaskMemAlloc(length * sizeof(wchar_t));
            wcscpy_s(outMsg->data, length, msg.data.c_str());
            return true;
        }
    }
    catch (...) {}
    return false;
}