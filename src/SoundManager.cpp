#include "SoundManager.h"
#include "Base.h"
#include "DataPath.h"

#include <iostream>

static int SoundPlayMaxNum = 16;

SoundManager::SoundManager()
{

}
SoundManager::~SoundManager()
{
	mDataStorage.clear();
}


SoundData* SoundManager::getData(char const* ime)
{
	for(int i=0; i<mDataStorage.size(); i++)
	{
		if(mDataStorage[i].name==ime)
		{
			return &mDataStorage[i];
		}
	}

	if ( !loadSound(ime) )
		return NULL;

	return &mDataStorage.back();
}

bool SoundManager::loadSound(char const* name)
{
	sf::SoundBuffer buffer;

	String path = SOUND_DIR;
	path += name;
	if ( !buffer.loadFromFile( path.c_str() ) )
	{
		MessageBox(NULL,TEXT("Greska kod ucitavanja zvukova."),TEXT("Error."),MB_OK);
		return false;
	}
	SoundData z;
	z.name   = name;
	z.buffer = buffer;	
	mDataStorage.push_back(z);
	std::cout << "Sound loaded : " << name << std::endl;
	return true;
}
void SoundManager::cleanup()
{
	for( SoundList::iterator iter = mSounds.begin() , itEnd = mSounds.end();
		 iter != itEnd ; ++iter )	
	{
		Sound* sound = *iter;
		if ( sound->isPlaying() )
			sound->stop();
		delete sound;
	}
	mSounds.clear();

	mDataStorage.clear();
}

void SoundManager::update( float dt )
{
	for( SoundList::iterator iter = mSounds.begin();
		iter != mSounds.end(); )
	{
		Sound* sound = *iter;
		if( !sound->isPlaying() )
		{
			delete sound;
			iter = mSounds.erase( iter );				
		}
		else
		{
			++iter;
		}
	}
}

Sound* SoundManager::addSound( char const* name , bool canRepeat )
{
	if ( mSounds.size() >= SoundPlayMaxNum )
		return NULL;

	SoundData* data = getData( name );

	if ( data == NULL )
		return NULL;

	if ( !canRepeat )
	{
		for(int i=0; i<mSounds.size(); i++)
		{
			Sound* sound = mSounds[i];
			if( sound->getData() != data )
				continue;

			sound->stop();
			return sound;
		}
	}

	Sound* sound = new Sound( data );
	mSounds.push_back(sound);		
	return sound;
}
