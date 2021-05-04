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
TEST(TimeKeeperTest, setTimeTwice_getTimeStep_returnOne)
{
    TimeTracker timeKeeper;
    timeKeeper.setTime(1.);
    timeKeeper.setTime(2.);
    ASSERT_FLOAT_EQ(timeKeeper.timeStep(), 1.);
}

TEST(CursorTrackerTest, init_getPositionChange_returnOne)
{
    CursorTracker cursorTracker;
    ASSERT_FLOAT_EQ(cursorTracker.positionChangeX(), 0.);
    ASSERT_FLOAT_EQ(cursorTracker.positionChangeY(), 0.);
}

TEST(CursorTrackerTest, setPositionOnce_getPositonChange_returnZero)
{
    CursorTracker cursorTracker;
    cursorTracker.setPosition(1., 1.);
    ASSERT_FLOAT_EQ(cursorTracker.positionChangeX(), 0.);
    ASSERT_FLOAT_EQ(cursorTracker.positionChangeY(), 0.);
}

TEST(CursorTrackerTest, setPositionTwice_getPositonChange_returnOneOne)
{
    CursorTracker cursorTracker;
    cursorTracker.setPosition(1., 1.);
    cursorTracker.setPosition(2., 0.);
    ASSERT_FLOAT_EQ(cursorTracker.positionChangeX(), 1.);
    ASSERT_FLOAT_EQ(cursorTracker.positionChangeY(), 1.);
}

TEST(StickyButtonTest, false_push_true)
{
    StickyButton button(false);

    button.push();

    ASSERT_TRUE(button.state());
}

TEST(StickyButtonTest, false_pushPush_true)
{
    StickyButton button(false);

    button.push();
    button.push();

    ASSERT_TRUE(button.state());
}

TEST(StickyButtonTest, false_pushReleasePush_false)
{
    StickyButton button(false);

    button.push();
    button.release();
    button.push();

    ASSERT_FALSE(button.state());
}

TEST(StickyButtonTest, false_pushReleasePushPush_false)
{
    StickyButton button(false);

    button.push();
    button.release();
    button.push();
    button.push();

    ASSERT_FALSE(button.state());
}

TEST(StickyButtonTest, false_pushReleasePushReleasePush_true)
{
    StickyButton button(false);

    button.push();
    button.release();
    button.push();
    button.release();
    button.push();

    ASSERT_TRUE(button.state());
}


