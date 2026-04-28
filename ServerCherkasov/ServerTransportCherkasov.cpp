#include "pch.h"
#include "ServerTransportCherkasov.h"
#include "LocalTransportCherkasov.h"
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <iostream> 

std::mutex ServerTransportCherkasov::serverMutex_;
std::condition_variable ServerTransportCherkasov::serverCv_;
std::queue<MessageCherkasov> ServerTransportCherkasov::serverQueue_;
std::map<int, std::shared_ptr<tcp::socket>> ServerTransportCherkasov::clients_;
int ServerTransportCherkasov::nextClientId_ = 0;
std::shared_ptr<tcp::acceptor> ServerTransportCherkasov::acceptor_ = nullptr;
boost::asio::io_context ServerTransportCherkasov::ioContext_;
std::unique_ptr<std::thread> ServerTransportCherkasov::ioThread_ = nullptr;
std::unique_ptr<std::thread> ServerTransportCherkasov::acceptThread_ = nullptr;
bool ServerTransportCherkasov::running_ = false;

ServerTransportCherkasov::ServerTransportCherkasov(int id) : id(id) {}
ServerTransportCherkasov::~ServerTransportCherkasov() {}

void ServerTransportCherkasov::sendToSocket(tcp::socket& sock, const MessageCherkasov& msg) {
    auto data = msg.serialize();
    uint32_t size = htonl(static_cast<uint32_t>(data.size()));
    boost::asio::write(sock, boost::asio::buffer(&size, sizeof(size)));
    boost::asio::write(sock, boost::asio::buffer(data.data(), data.size()));
}

MessageCherkasov ServerTransportCherkasov::recvFromSocket(tcp::socket& sock) {
    uint32_t size = 0;
    boost::asio::read(sock, boost::asio::buffer(&size, sizeof(size)));
    size = ntohl(size);
    if (size == 0 || size > 1024 * 1024)
        throw std::runtime_error("Invalid message size");
    std::vector<char> buffer(size);
    boost::asio::read(sock, boost::asio::buffer(buffer.data(), size));
    return MessageCherkasov::deserialize(buffer.data(), size);
}

void ServerTransportCherkasov::send(const MessageCherkasov& msg) const {
    if (id == -2) {
        return;
    }
    else if (id >= 0) {
        std::lock_guard<std::mutex> lock(serverMutex_);
        auto it = clients_.find(msg.header.to);
        if (it != clients_.end() && it->second && it->second->is_open()) {
            sendToSocket(*it->second, msg);
        }
    }
}

bool ServerTransportCherkasov::receive(MessageCherkasov& msg) {
    if (id == -2) {
        std::unique_lock<std::mutex> lock(serverMutex_);
        serverCv_.wait(lock, [] { return !serverQueue_.empty(); });
        msg = serverQueue_.front();
        serverQueue_.pop();
        return true;
    }
    return false;
}
/*новая реализация ниже потому что при такой реализации сервак не сам отправляет пользователю данные о потоках а он запрашивает
void ServerTransportCherkasov::handleClient(std::shared_ptr<tcp::socket> socket, int clientId) {
    {
        std::lock_guard<std::mutex> lock(serverMutex_);
        clients_[clientId] = socket;
        std::wcout << L"[SERVER] Клиент " << clientId << L" подключился. Всего клиентов: " << clients_.size() << std::endl;
    }

    try {
        while (running_) {
            MessageCherkasov msg = recvFromSocket(*socket);
            msg.header.from = clientId;
            {
                std::lock_guard<std::mutex> lock(serverMutex_);
                serverQueue_.push(msg);
                serverCv_.notify_one();
            }
        }
    }
    catch (const std::exception& e) {
        std::wcout << L"[SERVER] Клиент " << clientId << L" отключился (ошибка: " << e.what() << L")" << std::endl;
    }
    catch (...) {
        std::wcout << L"[SERVER] Клиент " << clientId << L" отключился" << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(serverMutex_);
        clients_.erase(clientId);
        std::wcout << L"[SERVER] Клиент " << clientId << L" удалён. Всего клиентов: " << clients_.size() << std::endl;
    }
}*/

void ServerTransportCherkasov::handleClient(std::shared_ptr<tcp::socket> socket, int clientId) {
    {
        std::lock_guard<std::mutex> lock(serverMutex_);
        clients_[clientId] = socket;
        std::wcout << L"[SERVER] Клиент " << clientId << L" подключился. Всего клиентов: " << clients_.size() << std::endl;
    }
    int threadCount = LocalTransportCherkasov::GetThreadCount();


    MessageCherkasov infoMsg(clientId, -2, MessageTypeCherkasov::MT_INFO, std::to_wstring(threadCount));

    try {

        sendToSocket(*socket, infoMsg);
        std::wcout << L"[SERVER] Отправлено клиенту " << clientId
            << L" количество потоков: " << threadCount << std::endl;
    }
    catch (const std::exception& e) {
        std::wcout << L"[SERVER] Ошибка отправки количества потоков клиенту "
            << clientId << L": " << e.what() << std::endl;
    }

    try {
        while (running_) {
            MessageCherkasov msg = recvFromSocket(*socket);
            msg.header.from = clientId; 


            {
                std::lock_guard<std::mutex> lock(serverMutex_);
                serverQueue_.push(msg);
                serverCv_.notify_one();  
            }
        }
    }
    catch (const std::exception& e) {
        std::wcout << L"[SERVER] Клиент " << clientId << L" отключился (ошибка: " << e.what() << L")" << std::endl;
    }
    catch (...) {
        std::wcout << L"[SERVER] Клиент " << clientId << L" отключился" << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(serverMutex_);
        clients_.erase(clientId);
        std::wcout << L"[SERVER] Клиент " << clientId << L" удалён. Всего клиентов: " << clients_.size() << std::endl;
    }
}

void ServerTransportCherkasov::acceptLoop() {
    while (running_) {
        try {
            auto socket = std::make_shared<tcp::socket>(ioContext_);
            acceptor_->accept(*socket);
            int clientId = nextClientId_++;
            std::thread([socket, clientId]() {
                handleClient(socket, clientId);
                }).detach();
        }
        catch (...) {}
    }
}

bool ServerTransportCherkasov::initServer(int port) {
    if (running_) return true;
    try {
        acceptor_ = std::make_shared<tcp::acceptor>(ioContext_, tcp::endpoint(tcp::v4(), port));
        running_ = true;

        acceptThread_ = std::make_unique<std::thread>([]() {
            acceptLoop();
            });

        ioThread_ = std::make_unique<std::thread>([]() {
            ioContext_.run();
            });

        return true;
    }
    catch (...) {
        return false;
    }
}

void ServerTransportCherkasov::closeServer() {
    running_ = false;
    ioContext_.stop();

    if (acceptThread_ && acceptThread_->joinable())
        acceptThread_->join();
    if (ioThread_ && ioThread_->joinable())
        ioThread_->join();

    std::lock_guard<std::mutex> lock(serverMutex_);
    for (auto& p : clients_) {
        if (p.second && p.second->is_open())
            p.second->close();
    }
    clients_.clear();
    acceptor_ = nullptr;
}