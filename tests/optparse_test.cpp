#include <gtest/gtest.h>

#include "optparse.h"
#include <cmath>

TEST( OptParseTest, Empty )
{
	OptParse args;
	args.Process( 0, nullptr );

	EXPECT_EQ( args.GetArgC(), 0 );
	EXPECT_EQ( args.Exists( "nonexistent" ), false );
	EXPECT_EQ( args["nonexistent"], nullptr );
}

TEST( OptParseTest, LongNameArgWorks )
{
	OptParse args;
	args.AddArgOption( "longname", "defaultvalue" );

	{
		const char* argv[] = { "rtls.exe", "--hi" };
		args.Process( 2, argv );
		EXPECT_STREQ( args["longname"], nullptr );
		EXPECT_EQ( args.GetArgC(), 0 );
	}

	{
		const char* argv[] = { "rtls.exe", "--longname", "nondefaultvalue" };
		args.Process( 2, argv );
		EXPECT_STREQ( args["longname"], "defaultvalue" );
		EXPECT_EQ( args.GetArgC(), 0 );
		args.Process( 3, argv );
		EXPECT_STREQ( args["longname"], "nondefaultvalue" );
		EXPECT_EQ( args.GetArgC(), 0 );
	}
}

TEST( OptParseTest, ShortNameArgWorks )
{
	OptParse args;
	args.AddArgOption( "longname", 'l', "defaultvalue" );

	{
		const char* argv[] = { "rtls.exe", "-l", "nondefaultvalue" };
		args.Process( 3, argv );
		EXPECT_STREQ( args["longname"], "nondefaultvalue" );
		EXPECT_EQ( args.GetArgC(), 0 );
	}
}

TEST( OptParseTest, FlagWorks )
{
	OptParse args;
	args.AddFlagOption( "first", 'a' );
	args.AddFlagOption( "second", 'b' );
	args.AddFlagOption( "third", 'c' );

	{
		const char* argv[] = { "rtls.exe", "-a", "-c" };
		args.Process( 3, argv );
		EXPECT_TRUE( args["first"] );
		EXPECT_TRUE( args["third"] );
		EXPECT_EQ( args.GetArgC(), 0 );
	}
}

TEST( OptParseTest, StrayWorks )
{
	OptParse args;
	args.AddFlagOption( "first", 'a' );
	args.AddFlagOption( "second", 'b' );
	args.AddFlagOption( "third", 'c' );

	{
		const char* argv[] = { "rtls.exe", "-a", "stray", "-c" };
		args.Process( 4, argv );
		EXPECT_EQ( args.GetArgC(), 1 );
		EXPECT_EQ( args.GetArg( 0 ), "stray" );
	}
}