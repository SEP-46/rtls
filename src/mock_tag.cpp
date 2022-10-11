#include "mock_tag.h"

#include "util.h"

MockTag::MockTag()
{
	// Add some fixed anchors
	UWBAnchor& anchor1 = mAnchorList.AddAnchor( 1 );
	anchor1.SetPosition( { 0.0f, 0.0f, 0.0f } );

	UWBAnchor& anchor2 = mAnchorList.AddAnchor( 2 );
	anchor2.SetPosition( { 1.0f, 0.0f, 0.0f } );

	UWBAnchor& anchor3 = mAnchorList.AddAnchor( 3 );
	anchor3.SetPosition( { 0.0f, 1.0f, 0.0f } );
}

bool MockTag::ReadDistanceData()
{
	// To match real kit behaviour, only update at rate of 10 Hz
	// 10 Hz = 0.1s period = 100 ms
	Timestamp_t time = Util_GetCurrentTime();
	if ( time - mLastUpdate < 100 )
		return false;

	mLastUpdate = time;

	mTagPosition.x = 0.5f + 0.5f * cosf( time / 1000.0f );
	mTagPosition.y = 0.5f + 0.5f * sinf( time / 1000.0f );

	for ( size_t i = 0; i < mAnchorList.NumAnchors(); i++ )
	{
		UWBAnchor& anchor = mAnchorList.GetAnchorByIndex( i );
		anchor.SetDistance( Vec3::Distance( anchor.GetPosition(), mTagPosition ) );
		anchor.SetLastUpdatedTimestamp( mLastUpdate );
	}

	return true;
}

std::vector<AnchorDistanceMeasurement> MockTag::CollectAnchorDistances() const
{
	std::vector<AnchorDistanceMeasurement> measurements;
	for ( size_t i = 0; i < mAnchorList.NumAnchors(); i++ )
	{
		const UWBAnchor& anchor = mAnchorList.GetAnchorByIndex( i );
		AnchorDistanceMeasurement measurement;
		measurement.id = anchor.GetId();
		measurement.distance = anchor.GetDistance();
		measurements.push_back( measurement );
	}
	return measurements;
}

Vec3 MockTag::GetBuiltinPosition() const
{
	return mTagPosition;
}

