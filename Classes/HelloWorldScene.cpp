/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ParallaxNodeExtras.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define SPACE_GAME "SpaceGame.caf"
#define EXPLOSION_LARGE "explosion_large.caf"
#define LASER_SHIP "laser_ship.caf"
#else
#define SPACE_GAME "SpaceGame.wav"
#define EXPLOSION_LARGE "explosion_large.wav"
#define LASER_SHIP "laser_ship.wav"
#endif

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
                                 origin.y + closeItem->getContentSize().height / 2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    Device::setAccelerometerEnabled(true);
    
    
    // 1. Agregar nave espacial
    _batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
    this->addChild(_batchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");
    
    _ship = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    Size winSize = Director::getInstance()->getWinSize();
    _ship->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
    _batchNode->addChild(_ship, 1);
    
    // 2) Crear Nodo Paralax
    _backgroundNode = ParallaxNodeExtras::create(); //1
    this->addChild(_backgroundNode,-1);
    
    // 3) Agrega objetos en nodo Paralax
    _spacedust1 = Sprite::create("bg_front_spacedust.png");
    _spacedust2 = Sprite::create("bg_front_spacedust.png");
    _planetsunrise = Sprite::create("bg_planetsunrise.png");
    _galaxy = Sprite::create("bg_galaxy.png");
    _spacialanomaly = Sprite::create("bg_spacialanomaly.png");
    _spacialanomaly2 = Sprite::create("bg_spacialanomaly2.png");
    
    // 3) Determina Velocidad en que corre el fondo
    Point dustSpeed = Vec2(0.1, 0.1);
    Point bgSpeed = Vec2(0.05, 0.05);
    
    // 4) Agrega hios al nodo Paralax
    _backgroundNode->addChild(_spacedust1, 0, dustSpeed, Vec2(0,winSize.height/2) ); // 2
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed, Vec2( _spacedust1->getContentSize().width,winSize.height/2));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, Vec2(0, winSize.height * 0.7));
    _backgroundNode->addChild(_planetsunrise, -1 , bgSpeed, Vec2(600, winSize.height * 0));
    _backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, Vec2(900, winSize.height * 0.3));
    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, Vec2(1500, winSize.height * 0.9));
    
    
    HelloWorld::addChild(ParticleSystemQuad::create("Stars1.plist"));
    HelloWorld::addChild(ParticleSystemQuad::create("Stars2.plist"));
    HelloWorld::addChild(ParticleSystemQuad::create("Stars3.plist"));
    
    
    #define KNUMASTEROIDS 15
    _asteroids = new Vector<Sprite*>(KNUMASTEROIDS);
    for (int i = 0; i < KNUMASTEROIDS; ++i) {
        auto *asteroid = Sprite::createWithSpriteFrameName("asteroid.png");
        asteroid->setVisible(false);
        _batchNode->addChild(asteroid);
        _asteroids->pushBack(asteroid);
    }
    
    #define KNUMLASERS 5
     _shipLasers = new Vector<Sprite*>(KNUMLASERS);
     for (int i = 0; i < KNUMLASERS; ++i) {
     auto shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");
     shipLaser->setVisible(false);
     _batchNode->addChild(shipLaser);
     _shipLasers->pushBack(shipLaser);
     }
    
    this->scheduleUpdate();
    
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    _lives = 3;
    double curTime = getTimeTick();
    _gameOverTime = curTime + 30000;
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic(SPACE_GAME,true);
    SimpleAudioEngine::getInstance()->preloadEffect(EXPLOSION_LARGE);
    SimpleAudioEngine::getInstance()->preloadEffect(LASER_SHIP);
    
    return true;
}

void HelloWorld::update(float dt) {
    Point dustSpeed = Vec2(0.1, 0.1);
    Point bgSpeed = Vec2(0.05, 0.05);
    
    Array *spaceDusts = Array::createWithCapacity(2);
    spaceDusts->addObject(_spacedust1);
    spaceDusts->addObject(_spacedust2);
    for ( int ii = 0; ii <spaceDusts->count(); ii++ ) {
        Sprite * spaceDust = (Sprite *)(spaceDusts->objectAtIndex(ii));
        float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
        float size = spaceDust->getContentSize().width;
        if ( xPosition < -size/2 ) {
            _backgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2,0),spaceDust);
        }
    }
    
    Array *backGrounds = Array::createWithCapacity(4);
    backGrounds->addObject(_galaxy);
    backGrounds->addObject(_planetsunrise);
    backGrounds->addObject(_spacialanomaly);
    backGrounds->addObject(_spacialanomaly2);
    for ( int ii = 0; ii <backGrounds->count(); ii++ ) {
        Sprite * background = (Sprite *)(backGrounds->objectAtIndex(ii));
        float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x;
        float size = background->getContentSize().width;
        if ( xPosition < -size ) {
            _backgroundNode->incrementOffset(Vec2(2000,0),background);
        }
    }
    
    Point backgroundScrollVert = Vec2(-1000, 0);
    _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));
    
    Size winSize = Director::sharedDirector()->getWinSize();
    float maxY = winSize.height - _ship->getContentSize().height/2;
    float minY = _ship->getContentSize().height/2;
    
    float diff = (_shipPointsPerSecY * dt);
    float newY = _ship->getPosition().y + diff;
    newY = MIN(MAX(newY, minY), maxY);
    _ship->setPosition(Vec2(_ship->getPosition().x, newY));
   
    
    float curTimeMillis = getTimeTick();
    
    if (curTimeMillis > _nextAsteroidSpawn) {
        
        float randMillisecs = randomValueBetween(0.20F, 1.0F) * 1000;
        _nextAsteroidSpawn = randMillisecs + curTimeMillis;
        
        float randY = randomValueBetween(0.0F, winSize.height);
        float randDuration = randomValueBetween(2.0F, 10.0F);
        
        Sprite *asteroid = _asteroids->at(_nextAsteroid);
        _nextAsteroid++;
        
        if (_nextAsteroid >= _asteroids->size())
            _nextAsteroid = 0;
        
        asteroid->stopAllActions();
        asteroid->setPosition(winSize.width + asteroid->getContentSize().width / 2, randY);
        asteroid->setVisible(true);
        asteroid->runAction(
                            Sequence::create(
                                             MoveBy::create(randDuration, Point(-winSize.width - asteroid->getContentSize().width, 0)),
                                             CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                             NULL /* DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)*/)
                            );
    }
    
    // Asteroids
    for (auto asteroid : *_asteroids){
        if (!((Sprite *) asteroid)->isVisible() )
            continue;
        for (auto shipLaser : *_shipLasers){
            if (!((Sprite *) shipLaser)->isVisible())
                continue;
            if (((Sprite *) shipLaser)->getBoundingBox().intersectsRect(((Sprite *)asteroid)->getBoundingBox()) ) {
                SimpleAudioEngine::getInstance()->playEffect(EXPLOSION_LARGE);
                ((Sprite *)shipLaser)->setVisible(false);
                ((Sprite *)asteroid)->setVisible(false);
                continue;
            }
        }
        if (_ship->getBoundingBox().intersectsRect(((Sprite *)asteroid)->getBoundingBox()) ) {
            ((Sprite *)asteroid)->setVisible(false);
            _ship->runAction( Blink::create(1.0, 9));
            _lives--;
        }
    }
    
    if (_lives <= 0) {
        _ship->stopAllActions();
        _ship->setVisible(false);
        this->endScene(KENDREASONLOSE);
    } else if (curTimeMillis >= _gameOverTime) {
        this->endScene(KENDREASONWIN);
    }
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::onAcceleration(Acceleration* pAccelerationValue, Event* event) {
    #define KFILTERINGFACTOR 0.1
    #define KRESTACCELX -0.6
    #define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)
    #define KMAXDIFFX 0.2
    
    double rollingX;
    
    // Cocos2DX inverts X and Y accelerometer depending on device orientation
    // in landscape mode right x=-y and y=x !!! (Strange and confusing choice)
    pAccelerationValue->x = pAccelerationValue->y;
    rollingX = (pAccelerationValue->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
    float accelX = pAccelerationValue->x - rollingX;
    Size winSize = Director::sharedDirector()->getWinSize();
    float accelDiff = accelX - KRESTACCELX;
    float accelFraction = accelDiff / KMAXDIFFX;
    _shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;
}

float HelloWorld::randomValueBetween(float low, float high) {
    return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

float HelloWorld::getTimeTick() {
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float) millisecs;
}

void HelloWorld::setInvisible(Node * node) {
    node->setVisible(false);
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event){
    SimpleAudioEngine::getInstance()->playEffect(LASER_SHIP);
    //SimpleAudioEngine::getInstance()->playEffect("laser_ship.wav");
    auto winSize = Director::getInstance()->getWinSize();
    auto shipLaser = _shipLasers->at(_nextShipLaser++);
    if (_nextShipLaser >= _shipLasers->size())
        _nextShipLaser = 0;
    shipLaser->setPosition(_ship->getPosition() + Point(shipLaser->getContentSize().width / 2, 0));
    shipLaser->setVisible(true);
    shipLaser->stopAllActions();
    shipLaser->runAction(
                         Sequence::create(
                                          MoveBy::create(0.5, Point(winSize.width, 0)),
                                          CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                          NULL));
}


void HelloWorld::restartTapped() {
    Director::sharedDirector()->replaceScene
    (TransitionZoomFlipX::create(0.5, this->getScene()));
    // reschedule
    this->scheduleUpdate();
}

void HelloWorld::endScene( EndReason endReason ) {
    if (_gameOver)
    return;
    _gameOver = true;
    
    auto winSize = Director::getInstance()->getWinSize();
    char message[10] = "You Win";
    if (endReason == KENDREASONLOSE)
        strcpy(message, "You Lose");
    auto label = Label::createWithBMFont("Arial.fnt", message);
    label->setScale(0.1F);
    label->setPosition(winSize.width / 2, winSize.height*0.6F);
    this->addChild(label);
    
    /*strcpy(message, "Restart");
    auto restartLabel = Label::createWithBMFont("Arial.fnt", message);
    auto restartItem = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(HelloWorld::restartTapped, this));
    restartItem->setScale(0.1F);
    restartItem->setPosition(winSize.width / 2, winSize.height*0.4);
    
    auto *menu = Menu::create(restartItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    // clear label and menu
    restartItem->runAction(ScaleTo::create(0.5F, 1.0F));
    label->runAction(ScaleTo::create(0.5F, 1.0F));*/
    
    // Terminate update callback
    this->unscheduleUpdate();
}
/*
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}*/
