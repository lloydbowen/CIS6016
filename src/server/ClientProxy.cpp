#include "ClientProxy.h"
#include "Timing.h"
#include "Engine.h"
#include "Server.h"


namespace
{
	const float kRespawnDelay = 3.f;
}

ClientProxy::ClientProxy( const SocketAddress& inSocketAddress, const string& inName, int inPlayerId ) :
mSocketAddress( inSocketAddress ),
mName( inName ),
mPlayerId( inPlayerId ),
mIsLastMoveTimestampDirty( false ),
mTimeToRespawn( 0.f )
{
	UpdateLastPacketTime();
}


void ClientProxy::UpdateLastPacketTime()
{
	mLastPacketFromClientTime = Timing::sInstance.GetTimef();
}

void	ClientProxy::HandlePlayerDied()
{
	mTimeToRespawn = Timing::sInstance.GetFrameStartTime() + kRespawnDelay;
}

void	ClientProxy::RespawnPlayerIfNecessary()
{
	if( mTimeToRespawn != 0.f && Timing::sInstance.GetFrameStartTime() > mTimeToRespawn )
	{
		static_cast< Server* > ( Engine::sInstance.get() )->SpawnPlayer( mPlayerId );
		mTimeToRespawn = 0.f;
	}
}
