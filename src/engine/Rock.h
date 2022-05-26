#ifndef ROCK_H_
#define ROCK_H_

#include "GameObject.h"
#include "World.h"

/* We'll later create client and server versions of this class */

class Rock : public GameObject
{
public:
	CLASS_IDENTIFICATION( 'ROCK', GameObject )

	enum EPlayerReplicationState
	{
		ECRS_Pose = 1 << 0,
		ECRS_RockId = 1 << 2,

		ECRS_AllState = ECRS_Pose | ECRS_RockId
	};

	static	GameObject*	StaticCreate()			{ return new Rock(); }

	//Note - the code in the book doesn't provide this until the client.
	//This however limits testing.
	static	GameObjectPtr	StaticCreatePtr()			{ return GameObjectPtr(new Rock()); }
	
	void		SetRockId( uint32_t inRockId )			{ mRockId = inRockId; }
	uint32_t	GetRockId()						const 	{ return mRockId; }

	virtual uint32_t GetAllStateMask()	const override	{ return ECRS_AllState; }

	virtual void Update() override;

//	virtual void	Read( InputMemoryBitStream& inInputStream ) override;

	uint32_t Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const override;

	Rock();
protected:
	
private:
	uint32_t			mRockId;
};

typedef shared_ptr< Rock >	RockPtr;

#endif // ROCK_H_
