#pragma once

struct MessageCherkasov;

class ITransportCherkasov {
public:
    virtual ~ITransportCherkasov() = default;
    virtual void send(const MessageCherkasov& msg) const = 0;  
    virtual bool receive(MessageCherkasov& msg) = 0;
};