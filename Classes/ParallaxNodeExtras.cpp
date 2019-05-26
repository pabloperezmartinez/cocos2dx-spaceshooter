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
    inline void setRation(Point ratio) {_ratio = ratio;}
    inline void setOffset(Point offset) {_offset = offset;}
    inline void setChild(Node *var) {_child = var;}
    inline Point getOffset() const {return _offset;}
    inline Node* getChild() const {return _child;}
private:
    Point _ratio;
    Point _offset;
    Node* _child;
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

void ParallaxNodeExtras::updatePosition()
{
    int safeOffset = -10;
    // Get visible size
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 1. For each child of an parallax node
    for(int i = 0; i < _children.size(); i++)
    {
        auto node = _children.at(i);
        // 2. We check whether it is out of the left side of the visible area
        if(convertToWorldSpace(node->getPosition()).x + node->getContentSize().width < safeOffset)
            // 3. Find PointObject that corresponds to current node
            for(int i = 0; i < _parallaxArray->num; i++)
            {
                auto po = (PointObject*)_parallaxArray->arr[i];
                // If yes increase its current offset on the value of visible width
                if(po->getChild() == node)
                    po->setOffset(po->getOffset() +
                                  Point(visibleSize.width + node->getContentSize().width,0));
            }
    }
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
