/*
 * CANFilter.h
 *
 *  Created on: 24.09.2017
 *      Author: rauser
 */

#ifndef TEST_CANFILTERTEST_H_
#define TEST_CANFILTERTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace CanSocket
{
namespace Test
{

class CANFilter : public ::testing::Test
{
protected:
	virtual void SetUp();
	virtual void TearDown();
};

} /* namespace Test */
} /* namespace CanSocket */

#endif /* TEST_CANFILTERTEST_H_ */
