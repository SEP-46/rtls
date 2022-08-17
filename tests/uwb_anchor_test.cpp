#include <gtest/gtest.h>

#include "uwb_tag.h"

TEST( UWBAnchorTest, DefaultIdIsInvalid )
{
    UWBAnchor a;
    EXPECT_EQ( a.GetId(), UWB_ANCHOR_ID_INVALID );
}

TEST( UWBAnchorTest, CanSetId )
{
    UWBAnchor a( 123 );
    EXPECT_EQ( a.GetId(), 123 );
}

TEST( UWBAnchorTest, CanSetPosition )
{
    Vec3 pos = { 1.0f, 2.0f, 3.0f };

    UWBAnchor a;
    a.SetPosition( pos );
    EXPECT_EQ( a.GetPosition(), pos);
}

TEST( UWBAnchorTest, CanSetDistance )
{
    float distance = 123.0f;

    UWBAnchor a;
    a.SetDistance( distance );
    EXPECT_FLOAT_EQ( a.GetDistance(), distance);
}

TEST( UWBAnchorTest, CanSetQualityFactor )
{
    float qf = 0.123f;

    UWBAnchor a;
    a.SetDistanceQualityFactor( qf );
    EXPECT_EQ( a.GetDistanceQualityFactor(), qf );
}

TEST( UWBAnchorTest, CanSetTimestamp )
{
    int64_t timestamp = 1234;

    UWBAnchor a;
    a.SetLastUpdatedTimestamp( timestamp );
    EXPECT_EQ( a.GetLastUpdatedTimestamp(), timestamp );
}

class UWBAnchorListTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        list1.AddAnchor( 1 );
        list2.AddAnchor( 2 );
        list2.AddAnchor( 3 );
    }

    // void TearDown() override {}

    UWBAnchorList list0;
    UWBAnchorList list1;
    UWBAnchorList list2;
};

TEST_F( UWBAnchorListTest, IsEmptyInitially )
{
    EXPECT_EQ( list0.NumAnchors(), 0 );
}

TEST_F( UWBAnchorListTest, AddAnchorWorks )
{
    list0.AddAnchor( 1 );
    EXPECT_EQ( list0.NumAnchors(), 1 );
    EXPECT_EQ( list0.GetAnchorByIndex( 0 ).GetId(), 1 );

    list0.AddAnchor( 2 );
    EXPECT_EQ( list0.NumAnchors(), 2 );
    EXPECT_EQ( list0.GetAnchorByIndex( 1 ).GetId(), 2 );
}

TEST_F( UWBAnchorListTest, RemoveAnchorWorks )
{
    EXPECT_EQ( list2.NumAnchors(), 2 );

    list2.RemoveAnchorByIndex( 1 );
    EXPECT_EQ( list2.NumAnchors(), 1 );
    EXPECT_TRUE( list2.FindAnchorById( 2 ) );
    EXPECT_FALSE( list2.FindAnchorById( 3 ) );

    list2.RemoveAnchorByIndex( 0 );
    EXPECT_EQ( list2.NumAnchors(), 0 );
    EXPECT_FALSE( list2.FindAnchorById( 2 ) );
    EXPECT_FALSE( list2.FindAnchorById( 3 ) );
}

TEST_F( UWBAnchorListTest, FindAnchorWorks )
{
    EXPECT_EQ( list0.FindAnchorById( 1 ), nullptr );

    EXPECT_EQ( list1.FindAnchorById( 1 ), &list1.GetAnchorByIndex( 0 ) );
    EXPECT_EQ( list1.FindAnchorById( 2 ), nullptr );
    
    EXPECT_EQ( list2.FindAnchorById( 1 ), nullptr );
    EXPECT_EQ( list2.FindAnchorById( 2 ), &list2.GetAnchorByIndex( 0 ) );
    EXPECT_EQ( list2.FindAnchorById( 3 ), &list2.GetAnchorByIndex( 1 ) );
}

TEST_F( UWBAnchorListTest, FindAnchorIndexWorks )
{
    EXPECT_EQ( list0.FindAnchorIndexById( 1 ), -1 );

    EXPECT_EQ( list1.FindAnchorIndexById( 1 ), 0 );
    EXPECT_EQ( list1.FindAnchorIndexById( 2 ), -1 );

    EXPECT_EQ( list2.FindAnchorIndexById( 1 ), -1 );
    EXPECT_EQ( list2.FindAnchorIndexById( 2 ), 0 );
    EXPECT_EQ( list2.FindAnchorIndexById( 3 ), 1 );
}