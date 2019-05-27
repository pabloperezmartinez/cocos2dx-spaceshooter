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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"

USING_NS_CC;

typedef enum {
    KENDREASONWIN,
    KENDREASONLOSE
} EndReason;

class HelloWorld : public cocos2d::Scene
{
private:
    cocos2d::SpriteBatchNode * _batchNode;
    cocos2d::Sprite * _ship;
    ParallaxNodeExtras *_backgroundNode;
    Sprite *_spacedust1;
    Sprite *_spacedust2;
    Sprite *_planetsunrise;
    Sprite *_galaxy;
    Sprite *_spacialanomaly;
    Sprite *_spacialanomaly2;
    float _shipPointsPerSecY;
    Vector<Sprite*> *_asteroids;
    int _nextAsteroid;
    float _nextAsteroidSpawn;
    Vector<Sprite*> *_shipLasers;
    int _nextShipLaser;
    int _lives;
    double _gameOverTime;
    bool _gameOver;
    
public:
    static cocos2d::Scene* createScene();
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual bool init();
    
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);
    
    void update(float dt);
    
    virtual void onAcceleration(Acceleration* pAccelerationValue, Event* event);
    
    float randomValueBetween(float low, float high);
    void setInvisible(Node * node);
    float getTimeTick();
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *event);
    void endScene(EndReason endReason);
    void restartTapped();
    //virtual void onTouchBegan(Touch* touch, Event* event);
    
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
