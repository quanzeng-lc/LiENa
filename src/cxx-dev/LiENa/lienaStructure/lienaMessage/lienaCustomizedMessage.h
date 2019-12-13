#ifndef LIENACUSTOMIZEDMESSAGE_H
#define LIENACUSTOMIZEDMESSAGE_H

#include "lienaMessage.h"
#include "lienaDatagram.h"
#include "lienaMessageProtocol.h"


class lienaCustomizedMessage : public lienaMessage
{

public:
    void define_body_length(uint32_t length);
    void append_uint8(uint8_t v);
    void append_uint16(uint16_t v);
    void append_uint32(uint32_t v);
    void append_uint64(uint64_t v);

//    void append_bool(bool v);
//    void append_float(float v);
//    void append_double(double v);

    uint8_t* get_message_body();

private:
    uint8_t* message_body;
    uint32_t index;
    lienaMessageProtocol* rule;

public:
    lienaCustomizedMessage(uint64_t messageId, uint32_t targetId, uint32_t originId, uint64_t  timestampes, uint32_t DLC);
};

#endif // LIENACUSTOMIZEDMESSAGE_H
