/*
 Touch Gestures Functionality
 AllienEscape (c) BrainLogix
 Written by: Zeeshan Ali
 */

#include "CCLayerPanZoom.h"
#include "Utils.h"

USING_NS_CC;

void CCLayerPanZoom::setMaxScale(float maxScale)
{
    _maxScale = maxScale;
    this->setScale(MIN(this->getScale(), _maxScale));
}

float CCLayerPanZoom::maxScale()
{
    return _maxScale;
}

void CCLayerPanZoom::setMinScale(float minScale)
{
    _minScale = minScale;
    this->setScale(MAX(this->getScale(), minScale));
}

float CCLayerPanZoom::minScale()
{
    return _minScale;
}

void CCLayerPanZoom::setRubberEffectRatio(float rubberEffectRatio)
{
    _rubberEffectRatio = rubberEffectRatio;

    // Avoid turning rubber effect On in frame mode.
    if (_mode == kCCLayerPanZoomModeFrame)
    {
        CCLOGERROR("CCLayerPanZoom#setRubberEffectRatio: rubber effect is not supported in frame mode.");
        _rubberEffectRatio = 0.0f;
    }

}

float CCLayerPanZoom::rubberEffectRatio()
{
    return _rubberEffectRatio;
}


CCLayerPanZoom* CCLayerPanZoom::layer()
{
    // 'layer' is an autorelease object
    CCLayerPanZoom *layer = CCLayerPanZoom::create();

    // return the scene
    return layer;
}

// on "init" you need to initialize your instance
bool CCLayerPanZoom::init()
{
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    //m_bIsRelativeAnchorPoint = true;
    m_bTouchEnabled = true;

    _maxScale = 3.0f;
    _minScale = 1.0f;
    _touches = CCArray::createWithCapacity(10);
    _touches->retain();

    _panBoundsRect = CCRectZero;
    _touchDistance = 0.0F;
    _maxTouchDistanceToClick = 315.0f;

    _mode = kCCLayerPanZoomModeSheet;
    _minSpeed = 100.0f;
    _maxSpeed = 1000.0f;
    _topFrameMargin = 100.0f;
    _bottomFrameMargin = 100.0f;
    _leftFrameMargin = 100.0f;
    _rightFrameMargin = 100.0f;

    _rubberEffectRatio = 0.0f;
    _rubberEffectRecoveryTime = 0.2f;
    _rubberEffectRecovering = false;
    _rubberEffectZooming = false;

    
    return true;
}

void CCLayerPanZoom::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        _touches->addObject(pTouch);
    }

    if (_touches->count() == 1)
    {
        _touchMoveBegan = false;
        time_t seconds;

        seconds = time (NULL);
        _singleTouchTimestamp = seconds/60;
    }
    else
        _singleTouchTimestamp = INFINITY;
}

void CCLayerPanZoom::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    bool multitouch =  _touches->count() > 1;
    float sizeX = this->boundingBox().getMaxX()-this->boundingBox().getMinX();
    float sizeY = this->boundingBox().getMaxY()-this->boundingBox().getMinY();

    if (multitouch)
    {
        
        // Get two of the touches to handle the zoom
        CCTouch* touchOne = (CCTouch*)_touches->objectAtIndex(0);
        CCTouch* touchTwo = (CCTouch*)_touches->objectAtIndex(1);
        
        // Get the touches and previous touches.
        CCPoint touchLocationOne = CCDirector::sharedDirector()->convertToGL(touchOne->getLocationInView());
        CCPoint touchLocationTwo = CCDirector::sharedDirector()->convertToGL(touchTwo->getLocationInView());
        
        CCPoint previousLocationOne = CCDirector::sharedDirector()->convertToGL(touchOne->getPreviousLocationInView());
        CCPoint previousLocationTwo = CCDirector::sharedDirector()->convertToGL(touchTwo->getPreviousLocationInView());
        
        // Get the distance for the current and previous touches.
        float currentDistance = sqrt(
                                       pow(touchLocationOne.x - touchLocationTwo.x, 2.0f) +
                                       pow(touchLocationOne.y - touchLocationTwo.y, 2.0f));
        
        float previousDistance = sqrt(
                                        pow(previousLocationOne.x - previousLocationTwo.x, 2.0f) +
                                        pow(previousLocationOne.y - previousLocationTwo.y, 2.0f));
        
        // Get the delta of the distances.
        float distanceDelta = currentDistance - previousDistance;
        // Next, position the camera to the middle of the pinch.
        // Get the middle position of the pinch.
        CCPoint pinchCenter = ccpMidpoint(touchLocationOne, touchLocationTwo);
        
        // Then, convert the screen position to node space... use your game layer to do this.
        pinchCenter = this->convertToNodeSpace(pinchCenter);
        
        // Finally, call the scale method to scale by the distanceDelta, pass in the pinch center as well.
        // Also, multiply the delta by PINCH_ZOOM_MULTIPLIER to slow down the scale speed.
        // A PINCH_ZOOM_MULTIPLIER of 0.005f works for me, but experiment to find one that you like.
        if(distanceDelta > -100 && distanceDelta < 100){
            this->setScale(this->getScale() + (distanceDelta * 0.005f));
        }else{
            CCLOG("%f x %f  -  %f",currentDistance, previousDistance, distanceDelta);
            
        }
        
        
        
        //////////////////////////////////////////////
        //////////////////////////////////////////////
        //////////////////////////////////////////////
        
        if (this->getPosition().x < -(sizeX/2))
        {
            this->setPosition(ccp(-(sizeX/2),this->getPosition().y));
        }
        else if (this->getPosition().x > ((sizeX-this->getContentSize().width)/2)){
            this->setPosition(ccp((sizeX-this->getContentSize().width)/2,this->getPosition().y));
        }
        
        
        if (this->getPosition().y < -(sizeY/2))
        {
            this->setPosition(ccp(this->getPosition().x,-(sizeY/2)));
        }
        else if (this->getPosition().y > ((sizeY-this->getContentSize().height)/2)){
            this->setPosition(ccp(this->getPosition().x,(sizeY-this->getContentSize().height)/2));
        }

        
        //////////////////////////////////////////////
        //////////////////////////////////////////////
        //////////////////////////////////////////////
        //////////////////////////////////////////////
        
        _touchDistance = INFINITY;
    }
    else
    {
        //CCLOG("single touch");
        
        // Get the single touch and it's previous & current position.
        CCTouch *touch = (CCTouch*)_touches->objectAtIndex(0);
        CCPoint curTouchPosition = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
        CCPoint prevTouchPosition = CCDirector::sharedDirector()->convertToGL(touch->getPreviousLocationInView());
        // Always scroll in sheet mode.
        if (_mode == kCCLayerPanZoomModeSheet)
        {
         //   this->setViewPointCenter(curTouchPosition);
            // Set new position of the layer.
            float posX = this->getPosition().x + curTouchPosition.x - prevTouchPosition.x;
            
            this->setPosition(ccp(posX,
                this->getPosition().y + curTouchPosition.y - prevTouchPosition.y));
            
            if (this->getPosition().x < -(sizeX/2))
            {
                this->setPosition(ccp(-(sizeX/2),this->getPosition().y));
            }
            else if (this->getPosition().x > ((sizeX-this->getContentSize().width)/2)){
                this->setPosition(ccp((sizeX-this->getContentSize().width)/2,this->getPosition().y));
            }
            
            
            if (this->getPosition().y < -(sizeY/2))
            {
                this->setPosition(ccp(this->getPosition().x,-(sizeY/2)));
            }
            else if (this->getPosition().y > ((sizeY-this->getContentSize().height)/2)){
                this->setPosition(ccp(this->getPosition().x,(sizeY-this->getContentSize().height)/2));
            }
            
            

        }

        // Accumulate touch distance for all modes.
        _touchDistance += ccpDistance(curTouchPosition, prevTouchPosition);

        // Inform delegate about starting updating touch position, if click isn't possible.
        if (_mode == kCCLayerPanZoomModeFrame)
        {
            if (_touchDistance > _maxTouchDistanceToClick && !_touchMoveBegan)
            {
                //ToDo add delegate here
                //[self.delegate layerPanZoom: self 
                //  touchMoveBeganAtPosition: [self convertToNodeSpace: prevTouchPosition]];
                _touchMoveBegan = true;
            }
        }
    }	
}

void CCLayerPanZoom::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    _singleTouchTimestamp = INFINITY;

    // Process click event in single touch.
    //ToDo add delegate
    
    if (  (_touchDistance < _maxTouchDistanceToClick) /*&& (self.delegate) */
        && (_touches->count() == 1))
    {
        CCTouch *touch = (CCTouch*)_touches->objectAtIndex(0);       
        CCPoint curPos = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
        
        //ToDo add delegate
        /*[self.delegate layerPanZoom: self
        clickedAtPoint: [self convertToNodeSpace: curPos]
        tapCount: [touch tapCount]];*/
    }

    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        _touches->removeObject(pTouch);
    }

    if (_touches->count() == 0)
    {
        _touchDistance = 0.0f;
    }

    if (!_touches->count() && !_rubberEffectRecovering)
    {
        this->recoverPositionAndScale();
    }
    
   
}

void CCLayerPanZoom::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){

    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        _touches->removeObject(pTouch);
    }

    if (_touches->count() == 0)
    {
        _touchDistance = 0.0f;
    }
}

// Updates position in frame mode.
void  CCLayerPanZoom::update(float dt){
    // Only for frame mode with one touch.
    if ( _mode == kCCLayerPanZoomModeFrame && _touches->count() == 1 )
    {
        // Do not update position if click is still possible.
        if (_touchDistance <= _maxTouchDistanceToClick)
            return;

        // Do not update position if pinch is still possible.
        time_t seconds;

        seconds = time (NULL);
        seconds /= 60;
        if (seconds - _singleTouchTimestamp < kCCLayerPanZoomMultitouchGesturesDetectionDelay)
            return;

        // Otherwise - update touch position. Get current position of touch.
        CCTouch *touch = (CCTouch*)_touches->objectAtIndex(0);
        CCPoint curPos = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

        // Scroll if finger in the scroll area near edge.
        if (this->frameEdgeWithPoint(curPos) != kCCLayerPanZoomFrameEdgeNone)
        {
            this->setPosition(ccp(this->getPosition().x + dt * this->horSpeedWithPosition( curPos), 
                this->getPosition().y + dt * this->vertSpeedWithPosition(curPos)));
        }

        // Inform delegate if touch position in layer was changed due to finger or layer movement.
        CCPoint touchPositionInLayer = this->convertToNodeSpace(curPos);
        if (!_prevSingleTouchPositionInLayer.equals(touchPositionInLayer))
        {
            _prevSingleTouchPositionInLayer = touchPositionInLayer;
            //ToDo add delegate
            //[self.delegate layerPanZoom: self 
            //      touchPositionUpdated: touchPositionInLayer];
        }

    }
}

void  CCLayerPanZoom::onEnter(){
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void  CCLayerPanZoom::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCLayer::onExit();
}
void CCLayerPanZoom::setPanBoundsRect(CCRect rect){
    _panBoundsRect = rect;
    this->setScale(this->minPossibleScale());
    this->setPosition(this->getPosition());
}

void CCLayerPanZoom::setPosition(CCPoint  position){
    CCPoint prevPosition = this->getPosition();
    //May be it wouldnot work
    CCNode::setPosition(position);

    if (!_panBoundsRect.equals(CCRectZero) && !_rubberEffectZooming)
    {
        if (_rubberEffectRatio && _mode == kCCLayerPanZoomModeSheet)
        {
            if (!_rubberEffectRecovering)
            {
                float topDistance = this->topEdgeDistance();
                float bottomDistance = this->bottomEdgeDistance();
                float leftDistance = this->leftEdgeDistance();
                float rightDistance = this->rightEdgeDistance();
                float dx = this->getPosition().x - prevPosition.x;
                float dy = this->getPosition().y - prevPosition.y;
                if (bottomDistance || topDistance)
                {
                    CCNode::setPosition(ccp(this->getPosition().x, 
                        prevPosition.y + dy * _rubberEffectRatio));                    
                }
                if (leftDistance || rightDistance)
                {
                    CCNode::setPosition(ccp(prevPosition.x + dx * _rubberEffectRatio, 
                        this->getPosition().y));                    
                }
            }
        }
        else
        {
            CCRect boundBox = this->boundingBox();
            if (this->getPosition().x - boundBox.size.width * this->getAnchorPoint().x > _panBoundsRect.origin.x)
            {
                CCNode::setPosition(ccp(boundBox.size.width * this->getAnchorPoint().x + _panBoundsRect.origin.x, 
                    this->getPosition().y));
            }	
            if (this->getPosition().y - boundBox.size.height * this->getAnchorPoint().y > _panBoundsRect.origin.y)
            {
                CCNode::setPosition(ccp(this->getPosition().x, boundBox.size.height * this->getAnchorPoint().y + 
                    _panBoundsRect.origin.y));
            }
            if (this->getPosition().x + boundBox.size.width * (1 - this->getAnchorPoint().x) < _panBoundsRect.size.width +
                _panBoundsRect.origin.x)
            {
                CCNode::setPosition(ccp(_panBoundsRect.size.width + _panBoundsRect.origin.x - 
                    boundBox.size.width * (1 - this->getAnchorPoint().x), this->getPosition().y));
            }
            if (this->getPosition().y + boundBox.size.height * (1 - this->getAnchorPoint().y) < _panBoundsRect.size.height + 
                _panBoundsRect.origin.y)
            {
                CCNode::setPosition(ccp(this->getPosition().x, _panBoundsRect.size.height + _panBoundsRect.origin.y - 
                    boundBox.size.height * (1 - this->getAnchorPoint().y)));
            }	
        }
    }
}

void CCLayerPanZoom::setScale(float scale){
    CCLayer::setScale( MIN(MAX(scale, _minScale), _maxScale));
}

void CCLayerPanZoom::recoverPositionAndScale(){
    if (!_panBoundsRect.equals(CCRectZero))
    {    
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        float rightEdgeDistance = this->rightEdgeDistance();
        float leftEdgeDistance = this->leftEdgeDistance();
        float topEdgeDistance = this->topEdgeDistance();
        float bottomEdgeDistance = this->bottomEdgeDistance();
        float scale = this->minPossibleScale();

        if (!rightEdgeDistance && !leftEdgeDistance && !topEdgeDistance && !bottomEdgeDistance)
        {
            return;
        }

        if (this->getScale() < scale)
        {
            _rubberEffectRecovering = true;
            CCPoint newPosition = CCPointZero;
            if (rightEdgeDistance && leftEdgeDistance && topEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);
                newPosition = ccp(winSize.width * 0.5f + dx, winSize.height * 0.5f + dy);
            }
            else if (rightEdgeDistance && leftEdgeDistance && topEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
                newPosition = ccp(winSize.width * 0.5f + dx, winSize.height - dy);
            }
            else if (rightEdgeDistance && leftEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
                newPosition = ccp(winSize.width * 0.5f + dx, dy);
            }
            else if (rightEdgeDistance && topEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
                newPosition = ccp(winSize.width  - dx, winSize.height  * 0.5f + dy);
            }
            else if (leftEdgeDistance && topEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
                newPosition = ccp(dx, winSize.height * 0.5f + dy);
            }
            else if (leftEdgeDistance && topEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
                float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
                newPosition = ccp(dx, winSize.height - dy);
            } 
            else if (leftEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
                float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
                newPosition = ccp(dx, dy);
            } 
            else if (rightEdgeDistance && topEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
                float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
                newPosition = ccp(winSize.width - dx, winSize.height - dy);
            } 
            else if (rightEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
                float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
                newPosition = ccp(winSize.width - dx, dy);
            } 
            else if (topEdgeDistance || bottomEdgeDistance)
            {
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
                newPosition = ccp(this->getPosition().x, winSize.height * 0.5f + dy);
            }
            else if (leftEdgeDistance || rightEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                newPosition = ccp(winSize.width * 0.5f + dx, this->getPosition().y);
            } 

            CCMoveTo *moveToPosition = CCMoveTo::create( _rubberEffectRecoveryTime,newPosition);
            CCScaleTo *scaleToPosition = CCScaleTo::create( _rubberEffectRecoveryTime,scale);
            CCFiniteTimeAction *sequence = CCSpawn::create(scaleToPosition, moveToPosition, CCCallFunc::create( this, callfunc_selector(CCLayerPanZoom::recoverEnded)), NULL);
            this->runAction(sequence);

        }
        else
        {
            _rubberEffectRecovering = false;
            CCMoveTo *moveToPosition = CCMoveTo::create(_rubberEffectRecoveryTime,ccp(this->getPosition().x + rightEdgeDistance - leftEdgeDistance, 
                this->getPosition().y + topEdgeDistance - bottomEdgeDistance));
            CCFiniteTimeAction *sequence = CCSpawn::create(moveToPosition, CCCallFunc::create( this, callfunc_selector(CCLayerPanZoom::recoverEnded)), NULL);
            this->runAction(sequence);

        }
    }
}

void CCLayerPanZoom::recoverEnded(){
    _rubberEffectRecovering = false;
}

float CCLayerPanZoom::topEdgeDistance(){
    CCRect boundBox = this->boundingBox();
    return (int)(MAX(_panBoundsRect.size.height + _panBoundsRect.origin.y - this->getPosition().y - 
        boundBox.size.height * (1 - this->getAnchorPoint().y), 0));
}

float CCLayerPanZoom::leftEdgeDistance(){
    CCRect boundBox = this->boundingBox();
    return (int)(MAX(this->getPosition().x - boundBox.size.width * this->getAnchorPoint().x - _panBoundsRect.origin.x, 0));
}    

float CCLayerPanZoom::bottomEdgeDistance(){
    CCRect boundBox = this->boundingBox();
    return (int)(MAX(this->getPosition().y - boundBox.size.height * this->getAnchorPoint().y - _panBoundsRect.origin.y, 0));
}

float CCLayerPanZoom::rightEdgeDistance(){
    CCRect boundBox = this->boundingBox();
    return (int)(MAX(_panBoundsRect.size.width + _panBoundsRect.origin.x - this->getPosition().x - 
        boundBox.size.width * (1 - this->getAnchorPoint().x), 0));
}

float CCLayerPanZoom::minPossibleScale(){
    if (!_panBoundsRect.equals(CCRectZero))
    {
        return MAX(_panBoundsRect.size.width / this->getContentSize().width,
            _panBoundsRect.size.height /this->getContentSize().height);
    }
    else 
    {
        return _minScale;
    }
}

CCLayerPanZoomFrameEdge CCLayerPanZoom::frameEdgeWithPoint( CCPoint point){
    bool isLeft = point.x <= _panBoundsRect.origin.x + _leftFrameMargin;
    bool isRight = point.x >= _panBoundsRect.origin.x + _panBoundsRect.size.width - _rightFrameMargin;
    bool isBottom = point.y <= _panBoundsRect.origin.y + _bottomFrameMargin;
    bool isTop = point.y >= _panBoundsRect.origin.y + _panBoundsRect.size.height - _topFrameMargin;

    if (isLeft && isBottom)
    {
        return kCCLayerPanZoomFrameEdgeBottomLeft;
    }
    if (isLeft && isTop)
    {
        return kCCLayerPanZoomFrameEdgeTopLeft;
    }
    if (isRight && isBottom)
    {
        return kCCLayerPanZoomFrameEdgeBottomRight;
    }
    if (isRight && isTop)
    {
        return kCCLayerPanZoomFrameEdgeTopRight;
    }

    if (isLeft)
    {
        return kCCLayerPanZoomFrameEdgeLeft;
    }
    if (isTop)
    {
        return kCCLayerPanZoomFrameEdgeTop;
    }
    if (isRight)
    {
        return kCCLayerPanZoomFrameEdgeRight;
    }
    if (isBottom)
    {
        return kCCLayerPanZoomFrameEdgeBottom;
    }

    return kCCLayerPanZoomFrameEdgeNone;
}

float CCLayerPanZoom::horSpeedWithPosition(CCPoint pos){
    CCLayerPanZoomFrameEdge edge = this->frameEdgeWithPoint(pos);
    float speed = 0.0f;
    if (edge == kCCLayerPanZoomFrameEdgeLeft)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
            (_panBoundsRect.origin.x + _leftFrameMargin - pos.x) / _leftFrameMargin;
    }
    if (edge == kCCLayerPanZoomFrameEdgeBottomLeft || edge == kCCLayerPanZoomFrameEdgeTopLeft)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
            (_panBoundsRect.origin.x + _leftFrameMargin - pos.x) / (_leftFrameMargin * sqrt(2.0f));
    }
    if (edge == kCCLayerPanZoomFrameEdgeRight)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
            (pos.x - _panBoundsRect.origin.x - _panBoundsRect.size.width + 
            _rightFrameMargin) / _rightFrameMargin);
    }
    if (edge == kCCLayerPanZoomFrameEdgeBottomRight || edge == kCCLayerPanZoomFrameEdgeTopRight)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
            (pos.x - _panBoundsRect.origin.x - _panBoundsRect.size.width + 
            _rightFrameMargin) / (_rightFrameMargin * sqrt(2.0f)));
    }
    return speed;
}

float CCLayerPanZoom::vertSpeedWithPosition(CCPoint pos){
    CCLayerPanZoomFrameEdge edge = this->frameEdgeWithPoint(pos);
    float speed = 0.0f;
    if (edge == kCCLayerPanZoomFrameEdgeBottom)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
            (_panBoundsRect.origin.y + _bottomFrameMargin - pos.y) / _bottomFrameMargin;
    }
    if (edge == kCCLayerPanZoomFrameEdgeBottomLeft || edge == kCCLayerPanZoomFrameEdgeBottomRight)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
            (_panBoundsRect.origin.y + _bottomFrameMargin - pos.y) / (_bottomFrameMargin * sqrt(2.0f));
    }
    if (edge == kCCLayerPanZoomFrameEdgeTop)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
            (pos.y - _panBoundsRect.origin.y - _panBoundsRect.size.height + 
            _topFrameMargin) / _topFrameMargin);
    }
    if (edge == kCCLayerPanZoomFrameEdgeTopLeft || edge == kCCLayerPanZoomFrameEdgeTopRight)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
            (pos.y - _panBoundsRect.origin.y - _panBoundsRect.size.height + 
            _topFrameMargin) / (_topFrameMargin * sqrt(2.0f)));
    }
    return speed;
} 
