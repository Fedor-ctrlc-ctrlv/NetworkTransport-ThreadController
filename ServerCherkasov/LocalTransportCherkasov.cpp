#include "pch.h"
#include "LocalTransportCherkasov.h"
#include <fstream>
#include <iostream>
#include <filesystem>

std::vector<std::thread> LocalTransportCherkasov::threads_;
std::map<int, std::shared_ptr<SessionCherkasov>> LocalTransportCherkasov::sessions_;
std::mutex LocalTransportCherkasov::mutex_;
int LocalTransportCherkasov::nextSessionId_ = 0;
std::mutex LocalTransportCherkasov::consoleMutex_;

void LocalTransportCherkasov::PrintToConsole(const std::wstring& text) {
    std::lock_guard<std::mutex> lock(consoleMutex_);
    std::wcout << text << std::endl;
}

void LocalTransportCherkasov::send(const MessageCherkasov& msg) const {
    std::lock_guard<std::mutex> lock(mutex_);

    if (targetId == -1) {
        for (auto& [id, session] : sessions_) {
            session->addMessage(msg);
        }
    }
    else if (targetId >= 0) {
        auto it = sessions_.find(targetId);
        if (it != sessions_.end()) {
            it->second->addMessage(msg);
        }
    }
}

bool LocalTransportCherkasov::receive(MessageCherkasov& msg) {
    auto it = sessions_.find(targetId);
    if (it != sessions_.end()) {
        return it->second->getMessage(msg);
    }
    return false;
}

void LocalTransportCherkasov::WorkerThread(int sessionId) {
    auto session = std::make_shared<SessionCherkasov>(sessionId);

    {
        std::lock_guard<std::mutex> lock(mutex_);
        sessions_[sessionId] = session;
    }

    while (true) {
        MessageCherkasov msg;
        if (session->getMessage(msg)) {
            switch (msg.header.messageType) {
            case MessageTypeCherkasov::MT_STOP:
            {
                std::lock_guard<std::mutex> lock(mutex_);
                sessions_.erase(sessionId);
                return;
            }
            case MessageTypeCherkasov::MT_DATA:
            {
                std::lock_guard<std::mutex> lock(mutex_);
                std::filesystem::create_directory(L"./msgs");
                std::wstring filename = L"./msgs/" + std::to_wstring(sessionId + 1) + L".txt";
                std::wofstream out(filename, std::ios::app);
                out.imbue(std::locale(""));
                if (out.is_open()) {
                    out << msg.data << std::endl;
                }
                break;
            }
            default:
                break;
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

int LocalTransportCherkasov::CreateSession() {
    std::lock_guard<std::mutex> lock(mutex_);
    int id = nextSessionId_++;
    threads_.emplace_back(WorkerThread, id);

    for (int i = 0; i < 50; ++i) {
        if (sessions_.find(id) != sessions_.end()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return id;
}
/*
void LocalTransportCherkasov::StopSession(int sessionId) {
    MessageCherkasov stopMsg(sessionId, MessageTypeCherkasov::MT_STOP, L"");
    LocalTransportCherkasov(sessionId).send(stopMsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}*/

void LocalTransportCherkasov::StopSession(int sessionId) {
    std::wcout << L"[LOCAL] Останавливаем сессию с ID=" << sessionId << std::endl;

    MessageCherkasov stopMsg(sessionId, MessageTypeCherkasov::MT_STOP, L"");
    LocalTransportCherkasov(sessionId).send(stopMsg);

    for (int i = 0; i < 50; ++i) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (sessions_.find(sessionId) == sessions_.end()) {
            std::wcout << L"[LOCAL] Сессия " << sessionId << L" успешно удалена" << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}


void LocalTransportCherkasov::StopAllSessions() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [id, session] : sessions_) {
        MessageCherkasov stopMsg(id, MessageTypeCherkasov::MT_STOP, L"");
        LocalTransportCherkasov(id).send(stopMsg);
    }
}

void LocalTransportCherkasov::WaitForAllThreads() {
    for (auto& t : threads_) {
        if (t.joinable()) t.join();
    }
    threads_.clear();
    sessions_.clear();
    nextSessionId_ = 0;
}

int LocalTransportCherkasov::GetThreadCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<int>(sessions_.size());
}