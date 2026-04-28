#pragma once
#include <string>
#include <vector>
#include <cstring>
#include "ITransportCherkasov.h"

enum class MessageTypeCherkasov : int {
    MT_START = 0,
    MT_STOP = 1,
    MT_DATA = 2,
    MT_INFO = 3
};

struct HeaderCherkasov {
    int to;
    int from;
    MessageTypeCherkasov messageType;
    int size;

    HeaderCherkasov() : to(-2), from(-2), messageType(MessageTypeCherkasov::MT_DATA), size(0) {}
    HeaderCherkasov(int t, int f, MessageTypeCherkasov type, int sz)
        : to(t), from(f), messageType(type), size(sz) {
    }
};

struct MessageCherkasov {
    HeaderCherkasov header;
    std::wstring data;

    MessageCherkasov() : header() {}

    MessageCherkasov(int to, MessageTypeCherkasov type, const std::wstring& msgData)
        : data(msgData) {
        header.to = to;
        header.from = -2;
        header.messageType = type;
        header.size = static_cast<int>(msgData.size() * sizeof(wchar_t));
    }

    MessageCherkasov(int to, int from, MessageTypeCherkasov type, const std::wstring& msgData)
        : data(msgData) {
        header.to = to;
        header.from = from;
        header.messageType = type;
        header.size = static_cast<int>(msgData.size() * sizeof(wchar_t));
    }

    std::vector<char> serialize() const {
        std::vector<char> buffer(sizeof(HeaderCherkasov) + header.size);
        memcpy(buffer.data(), &header, sizeof(HeaderCherkasov));
        if (header.size > 0) {
            memcpy(buffer.data() + sizeof(HeaderCherkasov), data.c_str(), header.size);
        }
        return buffer;
    }

    static MessageCherkasov deserialize(const char* buffer, int size) {
        MessageCherkasov msg;
        memcpy(&msg.header, buffer, sizeof(HeaderCherkasov));
        if (msg.header.size > 0) {
            int charCount = msg.header.size / sizeof(wchar_t);
            msg.data.assign(reinterpret_cast<const wchar_t*>(buffer + sizeof(HeaderCherkasov)), charCount);
        }
        return msg;
    }

    static void sendMessage(ITransportCherkasov& transport,
        MessageTypeCherkasov type,
        const std::wstring& data,
        int to = -2,
        int from = -2) {
        MessageCherkasov msg(to, from, type, data);
        transport.send(msg);
    }

    static MessageCherkasov receiveMessage(ITransportCherkasov& transport) {
        MessageCherkasov msg;
        transport.receive(msg);
        return msg;
    }

    void send(const ITransportCherkasov& transport) const {
        transport.send(*this);
    }

    void receive(ITransportCherkasov& transport) {
        transport.receive(*this);
    }
};