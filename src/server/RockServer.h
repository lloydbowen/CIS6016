#ifndef ROCK_SERVER_H
#define ROCK_SERVER_H

#include "Rock.h"
#include "NetworkManagerServer.h"


class RockServer : public Rock
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new RockServer() ); }

	virtual void Update() override;

protected:
	RockServer();
};

#endif // ROCK_SERVER_H
