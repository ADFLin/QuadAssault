#ifndef SoundManager_h__
#define SoundManager_h__

#include "Dependence.h"

struct SoundData
{
	char const* name;
	sf::SoundBuffer buffer;			
};

class Sound
{
public:
	Sound( SoundData* data )
		:mData( data )
	{
		mSoundImpl.setBuffer( mData->buffer );
	}

	void play(){ mSoundImpl.play(); }
	void stop(){ mSoundImpl.stop(); }
	bool isPlaying(){ return mSoundImpl.getStatus() == sf::Sound::Playing; }

	SoundData* getData(){ return mData; }

private:
	sf::Sound  mSoundImpl;
	SoundData* mData;
};

class SoundManager
{
public:	
	SoundManager();
	~SoundManager();

	SoundData* getData(char const* name );
	Sound*     addSound( char const* name , bool canRepeat = false );
	void       update( float dt );
	bool       loadSound(char const* name );
	void       cleanup();

private:
	typedef std::vector< Sound* > SoundList;

	std::vector<SoundData> mDataStorage;
	SoundList mSounds;
};


#endif // SoundManager_h__
