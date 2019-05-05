#include "TDpoint.h"
TDpoint* TDpoint::createPoint(int x, int y)
{
	TDpoint* td = new TDpoint();
	td->autorelease();
	td->px = x;
	td->py = y;
	return td;
}