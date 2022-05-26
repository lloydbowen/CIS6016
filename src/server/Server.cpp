
#include "Server.h"
#include "GameObjectRegistry.h"
#include "StringUtils.h"
#include "Colors.h"
#include "PlayerServer.h"
#include "RockServer.h"

//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction( 'PLYR', PlayerServer::StaticCreate );
//	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseServer::StaticCreate );
//	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction('ROCK', RockServer::StaticCreate);

	InitNetworkManager();

	// Setup latency
	float latency = 0.0f;
	string latencyString = StringUtils::GetCommandLineArg( 2 );
	if( !latencyString.empty() )
	{
		latency = stof( latencyString );
	}
	NetworkManagerServer::sInstance->SetSimulatedLatency( latency );
}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg( 1 );
	uint16_t port = stoi( portString );

	return NetworkManagerServer::StaticInit( port );
}


void Server::SetupWorld()
{
	// Static game objects and NPCs here.
	
	RockPtr rock0 = std::static_pointer_cast<Rock>(GameObjectRegistry::sInstance->CreateGameObject('ROCK'));
	rock0->SetLocation(Vector3(0.0f, 1.0f, 0.0f));
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnPlayers();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();
}

void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{

	int playerId = inClientProxy->GetPlayerId();

	//ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName() );
	SpawnPlayer( playerId );
}

void Server::SpawnPlayer( int inPlayerId )
{
	PlayerPtr player = std::static_pointer_cast< Player >( GameObjectRegistry::sInstance->CreateGameObject( 'PLYR' ) );
	player->SetColor( Colors::Red );//ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
	player->SetPlayerId( inPlayerId );
	//gotta pick a better spawn location than this...
	player->SetLocation( Vector3( 1.f - static_cast< float >( inPlayerId ), 0.f, 0.f ) );

}

void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	//kill client's player
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	//ScoreBoardManager::sInstance->RemoveEntry( playerId );
	PlayerPtr player = GetPlayer( playerId );
	if( player )
	{
		player->SetDoesWantToDie( true );
	}
}

PlayerPtr Server::GetPlayer( int inPlayerId )
{
	//run through the objects till we find the Player...
	//it would be nice if we kept a pointer to the Player on the clientproxy
	//but then we'd have to clean it up when the Player died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for( int i = 0, c = gameObjects.size(); i < c; ++i )
	{
		GameObjectPtr go = gameObjects[ i ];

		/* Original code did this in a weird way, used a method in the base (GameObject) which
		returned a player object if a game object is the player and null otherwise */

		uint32_t type = go->GetClassId();

		//Player* player = dynamic_cast<Player*>(*go);
		PlayerPtr player = nullptr;
		if(type == 'PLYR')
		{
			player = std::static_pointer_cast< Player >(go);
		}

		if(player && player->GetPlayerId() == inPlayerId )
		{
			return player;
		}
	}

	return nullptr;

}
