#ifndef PlayerSetting_h__
#define PlayerSetting_h__

int const MAX_LEVEL_NUM =64;

struct PlayerSetting
{
	string name;
	int    idxCurLevel;
	bool   levelEnabled[ MAX_LEVEL_NUM ];

	void load( char const* path )
	{







	}
};

#endif // PlayerSetting_h__
