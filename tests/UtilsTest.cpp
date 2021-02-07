#include "gtest/gtest.h"
#include "Utils.h"

TEST(TimeKeeperTest, init_getTimeStep_returnZero)
{
    TimeTracker timeKeeper;
    ASSERT_FLOAT_EQ(timeKeeper.timeStep(), 0.);
}

TEST(TimeKeeperTest, setTimeOnce_getTimeStep_returnZero)
{
    TimeTracker timeKeeper;
    timeKeeper.setTime(1.);
    ASSERT_FLOAT_EQ(timeKeeper.timeStep(), 0.);
}
TEST(TimeKeeperTest, setTimeTwice_getTimeStep_returnDiff)
{
    TimeTracker timeKeeper;
    timeKeeper.setTime(1.);
    timeKeeper.setTime(2.);
    ASSERT_FLOAT_EQ(timeKeeper.timeStep(), 1.);
}


