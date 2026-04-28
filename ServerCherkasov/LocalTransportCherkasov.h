#pragma once
#include "../cherkasovdll/ITransportCherkasov.h"
#include "SessionCherkasov.h"
#include <map>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>

class LocalTransportCherkasov : public ITransportCherkasov {
public:
    int targetId;

    LocalTransportCherkasov(int id = -1) : targetId(id) {}

    virtual void send(const MessageCherkasov& msg) const override;
    virtual bool receive(MessageCherkasov& msg) override;

    static int CreateSession();
    static void StopSession(int sessionId);
    static void StopAllSessions();
    static void WaitForAllThreads();
    static int GetThreadCount();
    static void PrintToConsole(const std::wstring& text);
    static std::mutex consoleMutex_;

private:
    static void WorkerThread(int sessionId);

    static std::vector<std::thread> threads_;
    static std::map<int, std::shared_ptr<SessionCherkasov>> sessions_;
    static std::mutex mutex_;
    static int nextSessionId_;
};