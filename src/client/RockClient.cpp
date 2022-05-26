#include "RockClient.h"

#include "TextureManager.h"
#include "GameObjectRegistry.h"

#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"
#include "NetworkManagerClient.h"

#include "StringUtils.h"

RockClient::RockClient() :
	mTimeLocationBecameOutOfSync( 0.f )
{
	mSpriteComponent.reset( new SpriteComponent( this ) );
	mSpriteComponent->SetTexture( TextureManager::sInstance->GetTexture( "rock" ) );
}

void RockClient::Update()
{
	//for now, we don't simulate any movement on the client side
	//we only move when the server tells us to move
}

void RockClient::Read( InputMemoryBitStream& inInputStream )
{
	bool stateBit;

	uint32_t readState = 0;

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		uint32_t rockId;
		inInputStream.Read( rockId );
		SetRockId( rockId );
		readState |= ECRS_RockId;
	}

	float oldRotation = GetRotation();
	Vector3 oldLocation = GetLocation();

	float replicatedRotation;
	Vector3 replicatedLocation;

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		inInputStream.Read( replicatedLocation.mX );
		inInputStream.Read( replicatedLocation.mY );

		SetLocation( replicatedLocation );

		inInputStream.Read( replicatedRotation );
		SetRotation( replicatedRotation );

		readState |= ECRS_Pose;
	}
}
