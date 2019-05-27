//
//  ParallaxNodeExtras.cpp
//  SpaceShooter
//
//  Created by Pablo Pérez Martínez on 26/05/2019.
//

#include <stdio.h>
#include "cocos2d.h"
#include "ParallaxNodeExtras.h"

class PointObject : public Ref
{
public:
    static PointObject * create(Point ratio, Point offset)
    {
        PointObject *ret = new PointObject();
        ret->initWithPoint(ratio, offset);
        ret->autorelease();
        return ret;
    }
    
    bool initWithPoint(Point ratio, Point offset)
    {
        _ratio = ratio;
        _offset = offset;
        _child = nullptr;
        return true;
    }
    
    inline const Point& getRatio() const { return _ratio; };
    inline void setRatio(const Point& ratio) { _ratio = ratio; };
    
    inline const Point& getOffset() const { return _offset; };
    inline void setOffset(const Point& offset) { _offset = offset; };
    
    inline Node* getChild() const { return _child; };
    inline void setChild(Node* child) { _child = child; };
    
private:
    Point _ratio;
    Point _offset;
    Node *_child; // weak ref
};


ParallaxNodeExtras* ParallaxNodeExtras::create()
{
    // Create an instance of InfiniteParallaxNode
    ParallaxNodeExtras* node = new ParallaxNodeExtras();
    if(node) {
        // Add it to autorelease pool
        node->autorelease();
    } else {
        // Otherwise delete
        delete node;
        node = 0;
    }
    return node;
}

void ParallaxNodeExtras::incrementOffset(Point offset,Node* node){
    for( unsigned int i = 0; i < this->getParallaxArray()->num; i++) {
        PointObject * point = (PointObject *)this->getParallaxArray()->arr[i];
        Node * curNode = point->getChild();
        if( curNode == node ) {
            point->setOffset( ccpAdd(point->getOffset(), offset) );
            break;
        }
    }
}
