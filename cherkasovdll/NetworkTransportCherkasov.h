#pragma once
#include "ITransportCherkasov.h"
#include "MessageCherkasov.h"
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <thread>

using boost::asio::ip::tcp;


class ClientTransportCherkasov : public ITransportCherkasov {
public:
    ClientTransportCherkasov();
    ~ClientTransportCherkasov();

    bool connect(const std::string& host, int port);
    void disconnect();
    bool is_connected() const;

    virtual void send(const MessageCherkasov& msg) const override;
    virtual bool receive(MessageCherkasov& msg) override;

private:
    std::shared_ptr<tcp::socket> socket_;
    boost::asio::io_context ioContext_;
    std::unique_ptr<std::thread> ioThread_;
    mutable std::mutex sendMutex_;

    void sendToSocket(tcp::socket& sock, const MessageCherkasov& msg) const;
    MessageCherkasov recvFromSocket(tcp::socket& sock);
};