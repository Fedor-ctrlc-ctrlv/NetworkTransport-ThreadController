#pragma once
#include "../cherkasovdll/MessageCherkasov.h"
#include <queue>
#include <mutex>

class SessionCherkasov {
private:
    std::queue<MessageCherkasov> messages_;
    std::mutex mutex_;

public:
    int sessionID;

    SessionCherkasov(int sessionID);
    ~SessionCherkasov();

    void addMessage(const MessageCherkasov& msg);
    bool getMessage(MessageCherkasov& msg);
};