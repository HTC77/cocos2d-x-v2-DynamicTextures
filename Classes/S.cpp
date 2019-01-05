#include "HUDLayer.h"

HUDLayer::HUDLayer()
{

}
HUDLayer::~HUDLayer()
{

}

HUDLayer* HUDLayer::create()
{
	HUDLayer* obj = new HUDLayer();
	if (obj && obj->init())
	{
		obj->autorelease();
		return obj;
	}

	CC_SAFE_DELETE(obj);
	return NULL;
}

bool HUDLayer::init()
{

	return true;
}
