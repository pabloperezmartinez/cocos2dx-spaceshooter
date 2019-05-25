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

USING_NS_CC;

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

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    
    // 1. Agregar nave espacial
    _batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
    this->addChild(_batchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");
    
    _ship = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    Size winSize = Director::getInstance()->getWinSize();
    _ship->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
    _batchNode->addChild(_ship, 1);
    
    // 2) Crear Nodo Paralax
    _backgroundNode = ParallaxNode::create(); //1
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
    
    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt) {
    Point backgroundScrollVert = Vec2(-1000, 0);
    _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
