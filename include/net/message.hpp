#pragma once
#include "net/common.hpp"

namespace net
{
template <typename T>
struct message_header
{
        T id{};
        uint32_t body_size = 0;
};

template <typename T>
struct message
{
        message_header<T> header{};
        std::vector<uint8_t> body;

        size_t size() const
        {
                return sizeof(message_header<T>) + body.size();
        }

        friend std::ostream& operator <<(std::ostream& os, const message<T>& msg)
        {
                os << "ID: " << int(msg.header.id) << "Size: " << msg.header.size;
                return os;
        }

        template <typename DataType>
        friend message<T>& operator <<(message<T>& msg, const DataType& data)
        {
                static_assert(std::is_standard_layout<DataType>::value, "Data type pushed into message must be trivial to handle!");
                size_t i = msg.body.size();
                msg.body.resize(msg.body.size() + sizeof(DataType));
                memcpy(msg.body.data() + i, &data, sizeof(DataType));
                msg.header.body_size = msg.body.size();
                return msg;
        }

        template <typename DataType>
        friend message<T>& operator >>(message<T>& msg, DataType& data)
        {
                static_assert(std::is_standard_layout<DataType>::value, "Data type pulled from message must be trivial to handle!");
                size_t i = msg.body.size() - sizeof(DataType);
                memcpy(&data, msg.body.data() + i, sizeof(DataType));
                msg.body.resize(i);
                msg.header.body_size = msg.body.size();
                return msg;
        }
};

//forward declaration
template <typename T>
class connection;

template <typename T>
struct owned_message
{
        std::shared_ptr<connection<T> > remote = nullptr;
        message<T> msg;


        friend std::ostream& operator <<(std::ostream& os, const owned_message<T>& msg)
        {
                os << msg.msg;
                return os;
        }
};
}
