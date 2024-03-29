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
	ccColor4F bgcolor2 = this->randomBrightColor();

	int stripes = (rand() % 4 + 1) * 2;
	//_background = this->spriteWithColor(bgColor, 512, 512);
	_background = this->spriteWithColor(bgColor, bgcolor2, 512, 512,stripes);
	_background->setPosition(visibleSize / 2);

	cocos2d::ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	_background->getTexture()->setTexParameters(&tp);
	//_background->setRotation(90);
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
	float gradiantAlpha = 0.7f;
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
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, colors);
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

CCSprite* HelloWorld::spriteWithColor(ccColor4F bgColor, ccColor4F color2,
	float textureWidth, float textureHeight, int nStripes)
{
	// 1. create render texture
	CCRenderTexture* rt = CCRenderTexture::create(textureWidth, textureHeight);

	// 2. begin render texture
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	// 3. draw

	// Layer 1: Noise
	CCSprite *noise = CCSprite::create("Noise2/Noise.png");
	ccBlendFunc blendFunc;
	blendFunc.dst = GL_ZERO;
	blendFunc.src = GL_DST_COLOR;
	noise->setBlendFunc(blendFunc);
	noise->setPosition(ccp(textureWidth / 2, textureHeight / 2));
	noise->visit();

	// Layer 2: Stripes
	CCPoint vertices[48];
	ccColor4F colors[48];
	int nVertices = 0;

	float x1 = -textureHeight;
	float x2;
	float y1 = textureHeight;
	float y2 = 0;
	float dx = textureWidth / nStripes * 2;
	float stripeWidth = dx / 2;
	for (int i = 0; i < nStripes; i++) {
		x2 = x1 + textureHeight;

		vertices[nVertices] = CCPointMake(x1, y1);
		colors[nVertices++] = bgColor2;

		vertices[nVertices] = CCPointMake(x1 + stripeWidth, y1);
		colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));

		vertices[nVertices] = CCPointMake(x2, y2);
		colors[nVertices++] = bgColor2;

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = bgColor2;

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = bgColor2;

		vertices[nVertices] = CCPointMake(x2 + stripeWidth, y2);
		colors[nVertices++] = bgColor2;

		x1 += dx;
	}

	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));

	CC_NODE_DRAW_SETUP();
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)nVertices);

	// layer 3: gradiant alpha
	float gradiantAlpha = 0.7f;
	nVertices = 0;

	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));
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


	// layer 4: highlight
	float borderHeight = textureHeight / 16;
	float borderAlpha = 0.3f;
	nVertices = 0;

	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = ccColor4F(ccc4f(1, 1, 1, borderAlpha));

	vertices[nVertices] = CCPointMake(textureWidth, 0);
	colors[nVertices++] = ccColor4F(ccc4f(1, 1, 1, borderAlpha));

	vertices[nVertices] = CCPointMake(0, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));

	vertices[nVertices] = CCPointMake(textureWidth, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	// 4. end render texture
	rt->end();
	
	// 5. create new sprite from the texture
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}

void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	this->genBackground();
}

void HelloWorld::update(float delta)
{
	float PIXELS_PER_SECOND = 100;
	static float offset = 0;
	offset += PIXELS_PER_SECOND * delta;
	CCSize textureSize = _background->getTextureRect().size;
	_background->setTextureRect(CCRectMake(offset, 0, textureSize.width, textureSize.height));
}

CCPoint HelloWorld::ctg(CCPoint point)
{
	return CCDirector::sharedDirector()->convertToGL(point);
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
	this->scheduleUpdate();
    return true;
}


