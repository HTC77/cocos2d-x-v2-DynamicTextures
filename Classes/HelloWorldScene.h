#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	CCSize visibleSize;

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	CCSprite* _background;
	void genBackground();
	ccColor4F randomBrightColor();
	CCSprite* spriteWithColor(ccColor4F bgColor, float textureWidth, float textureHeight);
	CCSprite* spriteWithColor(ccColor4F bgColor, ccColor4F bgColor2, float textureWidth, float textureHeight,int nStripes);
	void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) override;
	void update(float delta) override;
	
	//convert to gl
	CCPoint ctg(CCPoint point);
};

#endif // __HELLOWORLD_SCENE_H__
