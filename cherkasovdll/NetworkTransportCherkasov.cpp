#include "pch.h"
#include "NetworkTransportCherkasov.h"
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

ClientTransportCherkasov::ClientTransportCherkasov() {
    ioThread_ = std::make_unique<std::thread>([this]() { ioContext_.run(); });
}

ClientTransportCherkasov::~ClientTransportCherkasov() {
    disconnect();
    ioContext_.stop();
    if (ioThread_ && ioThread_->joinable())
        ioThread_->join();
}

bool ClientTransportCherkasov::connect(const std::string& host, int port) {
    try {
        socket_ = std::make_shared<tcp::socket>(ioContext_);
        tcp::resolver resolver(ioContext_);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(*socket_, endpoints);
        return true;
    }
    catch (...) {
        return false;
    }
}

void ClientTransportCherkasov::disconnect() {
    if (socket_ && socket_->is_open()) {
        try { socket_->close(); }
        catch (...) {}
    }
    socket_ = nullptr;
}

bool ClientTransportCherkasov::is_connected() const {
    return socket_ && socket_->is_open();
}

void ClientTransportCherkasov::sendToSocket(tcp::socket& sock, const MessageCherkasov& msg) const {
    auto data = msg.serialize();
    uint32_t size = htonl(static_cast<uint32_t>(data.size()));
    boost::asio::write(sock, boost::asio::buffer(&size, sizeof(size)));
    boost::asio::write(sock, boost::asio::buffer(data.data(), data.size()));
}

MessageCherkasov ClientTransportCherkasov::recvFromSocket(tcp::socket& sock) {
    uint32_t size = 0;
    boost::asio::read(sock, boost::asio::buffer(&size, sizeof(size)));
    size = ntohl(size);
    if (size == 0 || size > 1024 * 1024)
        throw std::runtime_error("Invalid message size");
    std::vector<char> buffer(size);
    boost::asio::read(sock, boost::asio::buffer(buffer.data(), size));
    return MessageCherkasov::deserialize(buffer.data(), size);
}

void ClientTransportCherkasov::send(const MessageCherkasov& msg) const {
    if (!socket_ || !socket_->is_open())
        return;
    std::lock_guard<std::mutex> lock(sendMutex_); 
    sendToSocket(*socket_, msg);
}

bool ClientTransportCherkasov::receive(MessageCherkasov& msg) {
    if (!socket_ || !socket_->is_open())
        return false;
    try {
        msg = recvFromSocket(*socket_);
        return true;
    }
    catch (...) {
        return false;
    }
}