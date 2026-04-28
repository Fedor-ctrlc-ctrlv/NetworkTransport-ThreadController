#include <string>
#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>
#include <map>
#include <iostream>
#include <condition_variable>
#include "ServerTransportCherkasov.h"
#include "LocalTransportCherkasov.h"

/*
int main() {
    std::locale::global(std::locale("rus_rus.866"));
    std::wcin.imbue(std::locale());
    std::wcout.imbue(std::locale());

    if (!ServerTransportCherkasov::initServer(5555)) {
        std::wcout << L"[SERVER] Ошибка: сервер не запущен" << std::endl;
        return 1;
    }

    std::wcout << L"[SERVER] Запущен на порту 5555" << std::endl;

    std::vector<int> threadIds;  // Храним ID созданных потоков
    ServerTransportCherkasov serverListener(-2);

    while (true) {
        MessageCherkasov msg = MessageCherkasov::receiveMessage(serverListener);

        switch (msg.header.to) {
        case -2:
            switch (msg.header.messageType) {
            case MessageTypeCherkasov::MT_START:
            {
                int newThreadId = LocalTransportCherkasov::CreateSession();
                threadIds.push_back(newThreadId);
                int threadCount = LocalTransportCherkasov::GetThreadCount();

                std::wcout << L"[SERVER] Поток " << newThreadId + 1
                    << L" создан. Всего потоков: " << threadCount << std::endl;

                ServerTransportCherkasov transport(msg.header.from);
                MessageCherkasov::sendMessage(transport,
                    MessageTypeCherkasov::MT_INFO,
                    std::to_wstring(threadCount),
                    msg.header.from, msg.header.from);
            }
            break;

            case MessageTypeCherkasov::MT_STOP:
            {
                if (!threadIds.empty()) {
                    int stoppedId = threadIds.back();
                    threadIds.pop_back();

                    LocalTransportCherkasov::StopSession(stoppedId);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                    int threadCount = LocalTransportCherkasov::GetThreadCount();
                    std::wcout << L"[SERVER] Поток " << stoppedId + 1
                        << L" остановлен. Всего потоков: " << threadCount << std::endl;
                }
                else {
                    std::wcout << L"[SERVER] Нет потоков для остановки" << std::endl;
                }

                int threadCount = LocalTransportCherkasov::GetThreadCount();
                ServerTransportCherkasov transport(msg.header.from);
                MessageCherkasov::sendMessage(transport,
                    MessageTypeCherkasov::MT_INFO,
                    std::to_wstring(threadCount),
                    msg.header.from, msg.header.from);
            }
            break;

            case MessageTypeCherkasov::MT_DATA:
            {
                std::lock_guard<std::mutex> lg(LocalTransportCherkasov::consoleMutex_);
                std::wcout << L"[CLIENT " << msg.header.from << L"] " << msg.data << std::endl;
            }
            break;

            case MessageTypeCherkasov::MT_INFO:
            {
                int threadCount = LocalTransportCherkasov::GetThreadCount();
                ServerTransportCherkasov transport(msg.header.from);
                MessageCherkasov::sendMessage(transport,
                    MessageTypeCherkasov::MT_INFO,
                    std::to_wstring(threadCount),
                    msg.header.from, msg.header.from);
            }
            break;
            }
            break;

        case -1:
        {
            std::lock_guard<std::mutex> lg(LocalTransportCherkasov::consoleMutex_);
            std::wcout << L"[CLIENT " << msg.header.from << L" -> ALL] " << msg.data << std::endl;
        }
        for (int threadId : threadIds) {
            msg.send(LocalTransportCherkasov(threadId));
        }
        break;

        default:
            // Проверяем, существует ли такой поток
            bool threadExists = false;
            for (int tid : threadIds) {
                if (tid == msg.header.to) {
                    threadExists = true;
                    break;
                }
            }

            if (threadExists) {
                {
                    std::lock_guard<std::mutex> lg(LocalTransportCherkasov::consoleMutex_);
                    std::wcout << L"[CLIENT " << msg.header.from << L" -> THREAD "
                        << msg.header.to + 1 << L"] " << msg.data << std::endl;
                }
                msg.send(LocalTransportCherkasov(msg.header.to));
            }
            else {
                std::wcout << L"[SERVER] Поток " << msg.header.to + 1 << L" не существует!" << std::endl;
            }
            break;
        }
    }

    ServerTransportCherkasov::closeServer();
    LocalTransportCherkasov::WaitForAllThreads();

    return 0;
}*/

int main() {
    std::locale::global(std::locale("rus_rus.866"));
    std::wcin.imbue(std::locale());
    std::wcout.imbue(std::locale());

    if (!ServerTransportCherkasov::initServer(5555)) {
        std::wcout << L"[SERVER] Ошибка: сервер не запущен" << std::endl;
        return 1;
    }

    std::wcout << L"[SERVER] Запущен на порту 5555" << std::endl;

    std::vector<int> threadIds;  
    ServerTransportCherkasov serverListener(-2);

    while (true) {
        MessageCherkasov msg = MessageCherkasov::receiveMessage(serverListener);

        switch (msg.header.to) {
        case -2:
            switch (msg.header.messageType) {
            case MessageTypeCherkasov::MT_START:
            {
                int newThreadId = LocalTransportCherkasov::CreateSession();
                threadIds.push_back(newThreadId);
                int threadCount = threadIds.size();  

                std::wcout << L"[SERVER] Поток " << threadCount
                    << L" создан (ID=" << newThreadId << L"). Всего потоков: " << threadCount << std::endl;

                ServerTransportCherkasov transport(msg.header.from);
                MessageCherkasov::sendMessage(transport,
                    MessageTypeCherkasov::MT_INFO,
                    std::to_wstring(threadCount),
                    msg.header.from, msg.header.from);
            }
            break;

            case MessageTypeCherkasov::MT_STOP:
            {
                if (!threadIds.empty()) {
                    int stoppedId = threadIds.back();
                    threadIds.pop_back();

                    LocalTransportCherkasov::StopSession(stoppedId);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                    int threadCount = threadIds.size();
                    std::wcout << L"[SERVER] Поток " << threadCount + 1
                        << L" (ID=" << stoppedId << L") остановлен. Всего потоков: " << threadCount << std::endl;
                }
                else {
                    std::wcout << L"[SERVER] Нет потоков для остановки" << std::endl;
                }

                int threadCount = threadIds.size();
                ServerTransportCherkasov transport(msg.header.from);
                MessageCherkasov::sendMessage(transport,
                    MessageTypeCherkasov::MT_INFO,
                    std::to_wstring(threadCount),
                    msg.header.from, msg.header.from);
            }
            break;

            case MessageTypeCherkasov::MT_DATA:
            {
                std::lock_guard<std::mutex> lg(LocalTransportCherkasov::consoleMutex_);
                std::wcout << L"[CLIENT " << msg.header.from << L"] " << msg.data << std::endl;
            }
            break;

            case MessageTypeCherkasov::MT_INFO:
            {
                int threadCount = threadIds.size();
                ServerTransportCherkasov transport(msg.header.from);
                MessageCherkasov::sendMessage(transport,
                    MessageTypeCherkasov::MT_INFO,
                    std::to_wstring(threadCount),
                    msg.header.from, msg.header.from);
            }
            break;
            }
            break;

        case -1:  
        {
            std::lock_guard<std::mutex> lg(LocalTransportCherkasov::consoleMutex_);
            std::wcout << L"[CLIENT " << msg.header.from << L" -> ALL] " << msg.data << std::endl;
        }
        
        for (int threadId : threadIds) {
            msg.send(LocalTransportCherkasov(threadId));
        }
        break;

        default:  
        {
            
            int position = msg.header.to;

            if (position >= 0 && position < (int)threadIds.size()) {
                int realThreadId = threadIds[position];
                {
                    std::lock_guard<std::mutex> lg(LocalTransportCherkasov::consoleMutex_);
                    std::wcout << L"[CLIENT " << msg.header.from << L" -> THREAD "
                        << position + 1 << L"] " << msg.data << std::endl;
                }
                msg.send(LocalTransportCherkasov(realThreadId));
            }
            else {
                std::wcout << L"[SERVER] Поток с позицией " << position + 1 << L" не существует!" << std::endl;
            }
            break;
        }
        }
    }

    ServerTransportCherkasov::closeServer();
    LocalTransportCherkasov::WaitForAllThreads();

    return 0;
}