#include "Rock.h"
#include "Maths.h"
#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"

Rock::Rock() :
	GameObject(),
	mRockId( 0 )
{
	SetCollisionRadius( 0.5f );
}

void Rock::Update()
{

}

uint32_t Rock::Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const
{
	uint32_t writtenState = 0;

	if( inDirtyState & ECRS_RockId )
	{
		inOutputStream.Write( (bool)true );
		inOutputStream.Write( GetRockId() );

		writtenState |= ECRS_RockId;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}


	if( inDirtyState & ECRS_Pose )
	{
		inOutputStream.Write( (bool)true );

		Vector3 location = GetLocation();
		inOutputStream.Write( location.mX );
		inOutputStream.Write( location.mY );

		inOutputStream.Write( GetRotation() );

		writtenState |= ECRS_Pose;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}

	return writtenState;
}