#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PauseScene.h"
#include "GameOverScene.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();    
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Director *_director;
	cocos2d::Size _visibleSize;	
	cocos2d::Sprite* _sprBomb;
	cocos2d::Sprite* _sprPlayer;
	cocos2d::Vector<cocos2d::Sprite*> _bombs;
	cocos2d::MenuItemImage* _muteItem;
	cocos2d::MenuItemImage* _unmuteItem;
	int _score;
	int _musicId;
	void initPhysics();
	void pauseCallback(cocos2d::Ref* pSender);
	void muteCallback(cocos2d::Ref* pSender);
	bool onCollision(cocos2d::PhysicsContact& contact);
	void setPhysicsBody(cocos2d::Sprite* sprite);
	void initTouch();
	void movePlayerByTouch(cocos2d::Touch* touch, cocos2d::Event* event);
	void movePlayerIfPossible(float newX);
	bool explodeBombs(cocos2d::Touch* touch, cocos2d::Event* event);
	void movePlayerByAccelerometer(cocos2d::Acceleration* acceleration, cocos2d::Event* event);
	void initAccelerometer();
	void initBackButtonListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void updateScore(float dt);
	void addBombs(float dt);	
	void initAudio();
	void initAudioNewEngine();
	void initMuteButton();
};

#endif // __HELLOWORLD_SCENE_H__
