#include "uwb_tag.h"

#include "lmh_serial.h"
#include "util.h"
#include <dwm_api.h>
#include <cassert>
#include <chrono>

UWBTag::UWBTag()
{
	LMH_SetImpl( LMH_Serial_GetImpl() );
	dwm_init();
}

UWBTag::~UWBTag()
{
	dwm_deinit();
}

bool UWBTag::ReadDistanceData()
{
	dwm_status_t status;
	if ( dwm_status_get( &status ) != RV_OK || !status.loc_data )
		return false;

	dwm_loc_data_t loc;
	dwm_pos_t pos;
	loc.p_pos = &pos;
	int ret = dwm_loc_get( &loc );
	assert( ret == RV_OK );
	assert( loc.anchors.dist.cnt == loc.anchors.an_pos.cnt );

	Timestamp_t timestamp = Util_GetCurrentTime();

	bool anchorConnected[MAX_ANCHORS] = {};

	for ( uint8_t i = 0; i < loc.anchors.dist.cnt; i++ )
	{
		UWBAnchorId_t id = loc.anchors.dist.addr[i];
		int anchorIndex = mAnchorList.FindAnchorIndexById( id );

		UWBAnchor* anchor;
		if ( anchorIndex == -1 )
		{
			anchorIndex = (int)mAnchorList.NumAnchors();
			anchor = &mAnchorList.AddAnchor( id );
		}
		else
		{
			anchor = &mAnchorList.GetAnchorByIndex( anchorIndex );
		}

		anchorConnected[anchorIndex] = true;

		// Sometimes we get a measurement of 0, which isn't correct
		// In this case, the anchor is still connected, but we should just use the last known distance measurement
		if ( loc.anchors.dist.dist[i] == 0 )
			continue;

		// DWM API gives us measurements in millimetre integers
		// We deal in metres, so need to do conversion by dividing by 1000
		float anchorDistMm = (float)loc.anchors.dist.dist[i];

		anchor->SetLastUpdatedTimestamp( timestamp );

		anchor->SetDistance( anchorDistMm / 1000.0f );

		Vec3 anchorPosMm = {
			(float)loc.anchors.an_pos.pos[i].x,
			(float)loc.anchors.an_pos.pos[i].y,
			(float)loc.anchors.an_pos.pos[i].z
		};
		anchor->SetPosition( anchorPosMm / 1000.0f );

		float anchorDistQf = (float)loc.anchors.dist.qf[i];
		anchor->SetDistanceQualityFactor( anchorDistQf );
	}

	// Remove anchors that we are no longer connected to
	for ( int i = (int)mAnchorList.NumAnchors() - 1; i >= 0; i-- )
	{
		if ( !anchorConnected[i] )
			mAnchorList.RemoveAnchorByIndex( ( size_t )i );
	}

	return true;
}

size_t UWBTag::CollectAnchorPositionsAndDistances( Vec3* anchorPositions, float* anchorDistances ) const
{
	for ( size_t i = 0; i < mAnchorList.NumAnchors(); i++ )
	{
		const UWBAnchor& anchor = mAnchorList.GetAnchorByIndex( i );
		anchorPositions[i] = anchor.GetPosition();
		anchorDistances[i] = anchor.GetDistance();
	}
	return mAnchorList.NumAnchors();
}

