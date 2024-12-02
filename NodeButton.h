#include "cocos2d.h"

class NodeButton : public cocos2d::Node {
protected:
	cocos2d::EventListenerTouchOneByOne* m_listener;
	cocos2d::Rect m_touchRect;
	enum class TouchEventType {
		BEGAN,
		MOVED_IN,
		MOVED_OUT,
		ENDED,
		CANCELLED
	};
	bool touchContains;
public:
	bool init() {
		cocos2d::Node::init();
		initTouchListener();
		setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
		return true;
	}
private:
	void initTouchListener() {
		m_listener = cocos2d::EventListenerTouchOneByOne::create();
		m_listener->setSwallowTouches(true);
		m_listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
			cocos2d::Vec2 touchPos = touch->getLocation();
			if (getParent())	touchPos = getParent()->convertToNodeSpace(touchPos);

            m_touchRect.size = getContentSize();
            m_touchRect.origin.x = getPosition().x - (getAnchorPoint().x * getContentSize().width);
            m_touchRect.origin.y = getPosition().y - (getAnchorPoint().y * getContentSize().height);
			if (m_touchRect.containsPoint(touchPos)) {
				eventListener(TouchEventType::BEGAN);
				touchContains = true;
				return true;
			}
			return false;
		};
		m_listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
			cocos2d::Vec2 touchPos = touch->getLocation();
			if (getParent())	touchPos = getParent()->convertToNodeSpace(touchPos);

			bool isContains = m_touchRect.containsPoint(touchPos);

			if (isContains != touchContains){
                (isContains) ? (eventListener(TouchEventType::MOVED_IN)) : (eventListener(TouchEventType::MOVED_OUT));
				touchContains = isContains;
			}
		};
		m_listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
			cocos2d::Vec2 touchPos = touch->getLocation();
			if (getParent())	touchPos = getParent()->convertToNodeSpace(touchPos);

			if (m_touchRect.containsPoint(touchPos))	eventListener(TouchEventType::ENDED);
			else										    eventListener(TouchEventType::CANCELLED);
			return true;
		};
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this);
	}
	virtual void eventListener(TouchEventType type) {}
public:
	CREATE_FUNC(NodeButton);
};
