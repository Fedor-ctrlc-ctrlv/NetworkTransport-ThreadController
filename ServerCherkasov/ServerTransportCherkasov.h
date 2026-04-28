#pragma once
#include "../cherkasovdll/ITransportCherkasov.h"
#include "../cherkasovdll/MessageCherkasov.h"
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <map>
#include <thread>

using boost::asio::ip::tcp;

class ServerTransportCherkasov : public ITransportCherkasov {
public:
    int id;

    ServerTransportCherkasov(int id = -2);
    ~ServerTransportCherkasov();

    static bool initServer(int port);
    static void closeServer();

    virtual void send(const MessageCherkasov& msg) const override;
    virtual bool receive(MessageCherkasov& msg);  

private:
    static void acceptLoop();
    static void handleClient(std::shared_ptr<tcp::socket> sock, int clientId);
    static void sendToSocket(tcp::socket& sock, const MessageCherkasov& msg);
    static MessageCherkasov recvFromSocket(tcp::socket& sock);

    static std::mutex serverMutex_;
    static std::condition_variable serverCv_;
    static std::queue<MessageCherkasov> serverQueue_;
    static std::map<int, std::shared_ptr<tcp::socket>> clients_;
    static int nextClientId_;
    static std::shared_ptr<tcp::acceptor> acceptor_;
    static boost::asio::io_context ioContext_;
    static std::unique_ptr<std::thread> ioThread_;
    static std::unique_ptr<std::thread> acceptThread_;
    static bool running_;
};