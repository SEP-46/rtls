#include "uwb_anchor.h"

UWBAnchor& UWBAnchorList::AddAnchor( NodeId_t id )
{
	assert( mNumAnchors < MAX_ANCHORS );

	UWBAnchor& anchor = mAnchors[mNumAnchors++];
	anchor = UWBAnchor( id );

	return anchor;
}

void UWBAnchorList::RemoveAnchorByIndex( size_t index )
{
	assert( mNumAnchors > 0 );
	assert( index < mNumAnchors );
	mAnchors[index] = mAnchors[mNumAnchors - 1];
	mNumAnchors--;
}

int UWBAnchorList::FindAnchorIndexById( NodeId_t id ) const
{
	for ( size_t i = 0; i < mNumAnchors; i++ )
	{
		if ( mAnchors[i].GetId() == id )
			return (int)i;
	}
	return -1;
}

const UWBAnchor* UWBAnchorList::FindAnchorById( NodeId_t id ) const
{
	for ( size_t i = 0; i < mNumAnchors; i++ )
	{
		if ( mAnchors[i].GetId() == id )
			return &mAnchors[i];
	}
	return nullptr;
}
