#include "gtest/gtest.h"
#include "Model.h"

TEST(VertexDataTest, position)
{
    VertexData data = VertexData::POSITION;

    ASSERT_TRUE(data.has(VertexData::POSITION));
    ASSERT_FALSE(data.has(VertexData::UV));
    ASSERT_FALSE(data.has(VertexData::NORMAL));

    ASSERT_EQ(data.sizePerVertex(), 3);
    ASSERT_EQ(data.positionOffset(), 0);
    ASSERT_EQ(data.uvOffset(), 3);
    ASSERT_EQ(data.normalOffset(), 3);
}

TEST(VertexDataTest, positionAndNormal)
{
    VertexData data = VertexData::POSITION | VertexData::NORMAL;
    
    ASSERT_TRUE(data.has(VertexData::POSITION));
    ASSERT_FALSE(data.has(VertexData::UV));
    ASSERT_TRUE(data.has(VertexData::NORMAL));

    ASSERT_EQ(data.sizePerVertex(), 6);
    ASSERT_EQ(data.positionOffset(), 0);
    ASSERT_EQ(data.uvOffset(), 3);
    ASSERT_EQ(data.normalOffset(), 3);
}

TEST(VertexDataTest, positionAndNormalAndUV)
{
    VertexData data = VertexData::POSITION | VertexData::NORMAL | VertexData::UV;

    ASSERT_TRUE(data.has(VertexData::POSITION));
    ASSERT_TRUE(data.has(VertexData::UV));
    ASSERT_TRUE(data.has(VertexData::NORMAL));

    ASSERT_EQ(data.sizePerVertex(), 8);
    ASSERT_EQ(data.positionOffset(), 0);
    ASSERT_EQ(data.uvOffset(), 3);
    ASSERT_EQ(data.normalOffset(), 5);
}

