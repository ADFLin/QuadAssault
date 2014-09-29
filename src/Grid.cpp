#include "Grid.h"
Grid::Grid()
{
}
Grid::~Grid()
{
	delete[] celije;
}
void Grid::Init(float velicina)
{
	int v=velicina/velicinaCelije;
	celije=new Celija[v*v];
	for(int x=0; x<v; x++)
	for(int y=0; y<v; y++)
	{
		celije[x+y].x=x;
		celije[x+y].y=y;
	}
}