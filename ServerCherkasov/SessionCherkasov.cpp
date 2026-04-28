#include <string>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "SessionCherkasov.h"

SessionCherkasov::SessionCherkasov(int sessionID) : sessionID(sessionID) {}
SessionCherkasov::~SessionCherkasov() {}

void SessionCherkasov::addMessage(const MessageCherkasov& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    messages_.push(msg);
}

bool SessionCherkasov::getMessage(MessageCherkasov& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (messages_.empty()) return false;
    msg = messages_.front();
    messages_.pop();
    return true;
}