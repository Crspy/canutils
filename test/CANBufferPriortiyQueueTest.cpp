/*
 * CANBufferPriortiyQueueTest.cpp
 *
 *  Created on: 26.02.2018
 *      Author: rauser
 */

#include "CANBufferPriortiyQueueTest.h"

#include "SocketCanFactory.h"
#include <thread>

namespace CanUtils
{
namespace Test
{

void CANUtilsPriortiyQueueTest::SetUp()
{  
}

void CANUtilsPriortiyQueueTest::TearDown()
{
}

TEST_F( CANUtilsPriortiyQueueTest, initialization )
{
	SocketCanFactory factory;
	CanBufferPtr buffer1 = factory.createCanBuffer("PriorityQueue");
	
	EXPECT_EQ   ( DEFAULT_BUFFER_SIZE, buffer1->size()           ); 
	EXPECT_EQ   ( "PriorityQueue",             buffer1->implementation() );
	EXPECT_TRUE ( buffer1->isEmpty() );
	EXPECT_FALSE( buffer1->isFull()  );
	EXPECT_FALSE( buffer1->hasNext() );
	
	CanBufferPtr buffer2 = factory.createCanBuffer("PriorityQueue", 42);
	
	EXPECT_EQ   ( 42,      buffer2->size()           ); 
	EXPECT_EQ   ( "PriorityQueue", buffer2->implementation() );
	EXPECT_TRUE ( buffer2->isEmpty() );
	EXPECT_FALSE( buffer2->isFull()  );
	EXPECT_FALSE( buffer2->hasNext() );
}

TEST_F( CANUtilsPriortiyQueueTest, isFull )
{
	SocketCanFactory factory;
	CanBufferPtr buffer = factory.createCanBuffer("PriorityQueue", 16);
	
	/* buffer should be not full */
	EXPECT_FALSE( buffer->isFull() );
	
	/* fill buffer */
	for( unsigned int i = 0; i < buffer->size() ; ++i )
	{
		CANMessage message( i, CANMessage::CANFrameType::Standard, 1, i );
		EXPECT_EQ( 0, buffer->write( message ) );
	}
	
	/* buffer is now full */
	EXPECT_TRUE ( buffer->isFull() );
	
	/* read a single message -> buffer is not full */
	CANMessage msg;
	EXPECT_EQ( 0, buffer->read( msg ) );
	
	EXPECT_FALSE( buffer->isFull() );
}


TEST_F( CANUtilsPriortiyQueueTest, isEmpty )
{
	SocketCanFactory factory;
	CanBufferPtr buffer = factory.createCanBuffer("PriorityQueue", 16);
	
	/* buffer should be empty */
	EXPECT_TRUE( buffer->isEmpty() );
	
	/* add a single message */
	CANMessage message( 0x123, CANMessage::CANFrameType::Standard, 1, 0x01 );
	EXPECT_EQ( 0, buffer->write( message ) );

	/* buffer is not empty */
	EXPECT_FALSE( buffer->isEmpty() );
	
	/* read the last message -> buffer is empty */
	CANMessage msg;
	EXPECT_EQ( 0, buffer->read( msg ) );
	
	EXPECT_TRUE( buffer->isEmpty() );
}

TEST_F( CANUtilsPriortiyQueueTest, resize )
{
	SocketCanFactory factory;
    const unsigned int initial_size = 2; 
	CanBufferPtr buffer = factory.createCanBuffer("PriorityQueue", initial_size);
	
	/* add a single message */
	CANMessage message( 0x123, CANMessage::CANFrameType::Standard, 1, 0x01 );
	EXPECT_EQ( 0, buffer->write( message ) );
	EXPECT_EQ( 0, buffer->write( message ) );
	
	/* buffer should be full */
	EXPECT_TRUE( buffer->isFull() );
	
	/* double buffer size */
	EXPECT_EQ( 0, buffer->resize( 2 * initial_size ) );  
	EXPECT_EQ( 2 * initial_size, buffer->size()      );
    
	/* buffer is not full */
	EXPECT_FALSE( buffer->isFull() );
	
	/* add another two messages */
	EXPECT_EQ( 0, buffer->write( message ) );
	EXPECT_EQ( 0, buffer->write( message ) );
	
	/* buffer should be full */
	EXPECT_TRUE( buffer->isFull() );
    
	/* resize to a smaller buffer */
	EXPECT_EQ( 0, buffer->resize( initial_size ) );  
	EXPECT_EQ( initial_size, buffer->size()      );
    
	/* buffer is full, but no messages are lost */
	EXPECT_TRUE( buffer->isFull() );
    
    /* resize to null should throw an exception */
    EXPECT_THROW( buffer->resize( 0 ), std::invalid_argument ); 
}

TEST_F( CANUtilsPriortiyQueueTest, resize_not_reordered )
{
    SocketCanFactory factory;
    const unsigned int initial_size = 2;
	CanBufferPtr buffer = factory.createCanBuffer("PriorityQueue", initial_size);
    
    /* add a single message */
	CANMessage message1( 0x123, CANMessage::CANFrameType::Standard, 1, 0x01 );
    CANMessage message2( 0x123, CANMessage::CANFrameType::Standard, 1, 0x02 );
    
	EXPECT_EQ( 0, buffer->write( message1 ) );
    
    /* double buffer size */
	EXPECT_EQ( 0, buffer->resize( 2 * initial_size ) );  
	EXPECT_EQ( 2 * initial_size, buffer->size()      );
    
    EXPECT_EQ( 0, buffer->write( message2 ) );
    
	/* resize to a smaller buffer */
	EXPECT_EQ( 0, buffer->resize( initial_size ) );  
	EXPECT_EQ( initial_size, buffer->size()      );
	
    CANMessage msg;
	EXPECT_EQ( 0, buffer->read( msg ) );
	EXPECT_EQ( message1, msg );
	
	EXPECT_EQ( 0, buffer->read( msg ) );
	EXPECT_EQ( message2, msg );
}

TEST_F( CANUtilsPriortiyQueueTest, clear )
{
	SocketCanFactory factory;
	CanBufferPtr buffer = factory.createCanBuffer("PriorityQueue", 16);
	
	/* buffer should be empty */
	EXPECT_TRUE( buffer->isEmpty() );
	
	/* add a single message */
	CANMessage message( 0x123, CANMessage::CANFrameType::Standard, 1, 0x01 );
	EXPECT_EQ( 0, buffer->write( message ) );
	
	/* buffer is not empty */
	EXPECT_FALSE( buffer->isEmpty() );
	
	/* clear the buffer */
	buffer->clear();
	
	EXPECT_TRUE( buffer->isEmpty() );
}

TEST_F( CANUtilsPriortiyQueueTest, read_write_single )
{
    SocketCanFactory factory;
    CanBufferPtr buffer = factory.createCanBuffer("PriorityQueue");
    
    CANMessage message1( 0x001, CANMessage::CANFrameType::Standard, 1, 0x01 );
    CANMessage message2( 0x002, CANMessage::CANFrameType::Standard, 1, 0x02 );
    CANMessage message3( 0x003, CANMessage::CANFrameType::Standard, 1, 0x03 );
    
    /* write some test messages */
    EXPECT_EQ( 0, buffer->write( message1 ) );
    EXPECT_EQ( 0, buffer->write( message2 ) );
    EXPECT_EQ( 0, buffer->write( message3 ) );
    
    /* read the messages */
    CANMessage msg1, msg2, msg3;
    EXPECT_EQ( 0, buffer->read( msg1 ) );
    EXPECT_EQ( 0, buffer->read( msg2 ) );
    EXPECT_EQ( 0, buffer->read( msg3 ) );
    
    /* compare, no reorder */
    EXPECT_EQ( message1, msg1 );
    EXPECT_EQ( message2, msg2 );
    EXPECT_EQ( message3, msg3 );
}

TEST_F( CANUtilsPriortiyQueueTest, read_write_reorder )
{
    SocketCanFactory factory;
    CanBufferPtr buffer = factory.createCanBuffer("PriorityQueue");
    
    CANMessage message_id_1( 0x001, CANMessage::CANFrameType::Standard, 1, 0x01 );
    CANMessage message_id_2( 0x002, CANMessage::CANFrameType::Standard, 1, 0x02 );
    CANMessage message_id_3( 0x003, CANMessage::CANFrameType::Standard, 1, 0x03 );
    
    /* write some test messages */
    EXPECT_EQ( 0, buffer->write( message_id_3 ) );
    EXPECT_EQ( 0, buffer->write( message_id_1 ) );
    EXPECT_EQ( 0, buffer->write( message_id_2 ) );
    
    /* read the messages */
    CANMessage msg1, msg2, msg3;
    EXPECT_EQ( 0, buffer->read( msg1 ) );
    EXPECT_EQ( 0, buffer->read( msg2 ) );
    EXPECT_EQ( 0, buffer->read( msg3 ) );
    
    /* compare, no reorder */
    /* ID 0x001 is the highest can message priority, */ 
    /* so we should receive this message at first */
    EXPECT_EQ( message_id_1, msg1 );
    EXPECT_EQ( message_id_2, msg2 );
    EXPECT_EQ( message_id_3, msg3 );
}

TEST_F( CANUtilsPriortiyQueueTest, read_write_async )
{
    SocketCanFactory factory;
    CanBufferPtr buffer = factory.createCanBuffer("Queue");
    const unsigned int retries = 100;
    
    std::thread worker = std::thread
    (
        [&](){
            for( unsigned int i = 0; i < retries; ++i )
            {
                CANMessage message_tx( 0x001, CANMessage::CANFrameType::Standard, 1, i );
                EXPECT_EQ( 0, buffer->write( message_tx ) );
            }
        }
    );
        
    /* read the messages */
    /* all of the message index have the same id, so here should be no reorder */
    for( unsigned int i = 0; i< retries; ++i )
    {
        CANMessage message( 0x001, CANMessage::CANFrameType::Standard, 1, i );
        CANMessage message_rx;
        /* order should be the same, if we inserted it */
        EXPECT_EQ( 0, buffer->read( message_rx ) );
        EXPECT_EQ( message, message_rx );
    }
    
    /* wait for thread */
    if( worker.joinable() )
    {
        worker.join();
    }
}

TEST_F( CANUtilsPriortiyQueueTest, priority_mixed )
{
	EXPECT_TRUE(false);
    CANMessage message_sff_high(     0x001, CANMessage::CANFrameType::Standard, 8, 0x01 );
    CANMessage message_sff_low (     0x7FF, CANMessage::CANFrameType::Standard, 8, 0x01 );
    CANMessage message_ext_high( 0x0000002, CANMessage::CANFrameType::Extended, 8, 0x02 );
    CANMessage message_ext_low ( 0x1FFFFFF, CANMessage::CANFrameType::Extended, 8, 0x02 );
    CANMessage message_err     ( 0x003,     CANMessage::CANFrameType::Error,    8, 0x03 );
    CANMessage message_rtr     ( 0x001,     CANMessage::CANFrameType::Remote,   8, 0x01 );
    /* TODO define the priortiy order and write the test */
}

} /* namespace Test */
} /* namespace CanUtils */
