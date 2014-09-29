#ifndef Grid_h__
#define Grid_h__

class Celija
{
public:

	int x;
	int y;
};

class Grid
{
public:
	Grid();
	~Grid();
	void Init(float velicina);

	float   velicinaCelije;
	Celija* celije;
};

#endif // Grid_h__