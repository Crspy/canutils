
#ifndef SRC_CANBUFFER_QUEUE_H
#define SRC_CANBUFFER_QUEUE_H

#include "CanBufferImpl.h"

#include <vector>

namespace CanSocket
{

class CanBufferRingBuffer : public CanBufferImpl
{
public:
    CanBufferRingBuffer( unsigned int size );
    ~CanBufferRingBuffer();
    
    virtual int read(CANMessage& msg) override;
    virtual int write(const CANMessage& msg) override;
    
    virtual int resize( unsigned int size ) override;
    
    virtual bool hasNext() const override;
    virtual bool isFull()  const override;
    virtual bool isEmpty() const override;
    
    virtual std::string implementation() const override;
    
protected:
    std::vector<CANMessage> _ring_buffer;
    std::size_t _read;
    std::size_t _write;
};

} /* namespace CanSocket */

#endif /* SRC_CANBUFFER_QUEUE_H */