#include "HelloWorldScene.h"


CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void HelloWorld::genBackground()
{
	_background->removeFromParentAndCleanup(true);
	ccColor4F bgColor = this->randomBrightColor();

	_background = this->spriteWithColor(bgColor, 512, 512);
	_background->setPosition(visibleSize / 2);

	this->addChild(_background);
	this->setScale(0.6);
}

ccColor4F HelloWorld::randomBrightColor()
{
	while (true)
	{
		int requiredBrightness = 192;
		ccColor4B randomColor =
			ccc4(rand() % 255, rand() % 255, rand() % 255, 255);
		if (randomColor.r > requiredBrightness ||
			randomColor.g > requiredBrightness ||
			randomColor.b > requiredBrightness)
		{
			return ccc4FFromccc4B(randomColor);
		}
	}
}

CCSprite* HelloWorld::spriteWithColor(ccColor4F bgColor, float textureWidth, float textureHeight)
{
	// 1: Create CCRenderTexture
	CCRenderTexture *rt = CCRenderTexture::create(textureWidth, textureHeight);

	// 2: Call CCRenderTexture:begin
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
	CC_NODE_DRAW_SETUP();

	// 3: Draw into texture
	float gradiantAlpha = 1.0f;
	CCPoint vertices[4];
	ccColor4F colors[4];
	int nVertices = 0;

	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0,0));
	vertices[nVertices] = CCPointMake(textureWidth, 0);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));
	vertices[nVertices] = CCPointMake(0, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, gradiantAlpha));
	vertices[nVertices] = CCPointMake(textureWidth, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, gradiantAlpha));


	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, FALSE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	// Create Noise
	CCSprite* noise = CCSprite::create("Noise2/Noise.png");
	ccBlendFunc blendFunc;
	blendFunc.dst = GL_ZERO;
	blendFunc.src = GL_DST_COLOR;
	noise->setBlendFunc(blendFunc);
	noise->setPosition(ccp(textureWidth / 2, textureHeight / 2));
	noise->visit();

	// 4: Call CCRenderTexture:end
	rt->end();

	// 5: create new sprite from the texture
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}

void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	this->genBackground();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    visibleSize = CCDirector::sharedDirector()->getVisibleSize();
   
	_background = new CCSprite();
	this->genBackground();
	this->setTouchEnabled(true);
    return true;
}


