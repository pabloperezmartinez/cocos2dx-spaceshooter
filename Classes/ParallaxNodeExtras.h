//
//  ParallaxNodeExtras.h
//  SpaceShooter
//
//  Created by Pablo Pérez Martínez on 26/05/2019.
//

#ifndef ParallaxNodeExtras_h
#define ParallaxNodeExtras_h

#include "cocos2d.h"

USING_NS_CC;

class ParallaxNodeExtras : public ParallaxNode {
    
    public :
    
    // just to avoid ugly later cast and also for safety
    static ParallaxNodeExtras * create();
    
    // Facility method (it’s expected to have it soon in COCOS2DX)
    //void updatePosition();
    
    void incrementOffset(CCPoint offset, CCNode* node); 
} ;

#endif /* ParallaxNodeExtras_h */
