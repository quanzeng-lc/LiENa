#ifndef LIENANETWORKQUALITYMESSAGE_H
#define LIENANETWORKQUALITYMESSAGE_H

#include "lienaMessage.h"


class lienaNetworkQualityMessage : public lienaMessage
{

private:
    int index;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
    uint64_t t4;

public:
    int get_index();
    uint64_t get_t1();
    uint64_t get_t2();
    uint64_t get_t3();
    uint64_t get_t4();

    void set_t1(uint64_t t);
    void set_t2(uint64_t t2);
    void set_t3(uint64_t t3);
    void set_t4(uint64_t t4);
    void set_index(int index);

public:
    lienaNetworkQualityMessage(uint64_t messageId,
                               uint32_t targetId,
                               uint32_t originId,
                               uint64_t timestampes,
                               uint32_t DLC);
};

#endif // LIENANETWORKQUALITYMESSAGE_H
