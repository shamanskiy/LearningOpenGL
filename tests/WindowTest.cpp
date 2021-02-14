#include "gtest/gtest.h"
#include "WIndow.h"
TEST(WindowTest, canInitialize)
{
    Window mainWindow(800, 600);
    auto out = mainWindow.initialize();
    ASSERT_TRUE(out.ok());
}


