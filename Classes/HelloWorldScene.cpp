#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "audio/include/AudioEngine.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../cocos2d/cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::experimental;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();	
	scene->getPhysicsWorld()->setGravity(Vect(0,0));
    auto layer = HelloWorld::create();
	#if COCOS2D_DEBUG
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	#endif
    scene->addChild(layer);
    return scene;
}

//Move the player if it does not go outside of the screen
void HelloWorld::movePlayerIfPossible(float newX){
	float sprHalfWidth = _sprPlayer->getBoundingBox().size.width /2 ;
	if(newX >= sprHalfWidth && newX < _visibleSize.width - sprHalfWidth){
		_sprPlayer->setPositionX(newX);
	}
}

void HelloWorld::movePlayerByTouch(Touch* touch, Event* event)
{
	Vec2 touchLocation = touch->getLocation();
	if(_sprPlayer->getBoundingBox().containsPoint(touchLocation)){
		movePlayerIfPossible(touchLocation.x);
	}
}

bool HelloWorld::explodeBombs(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Vec2 touchLocation = touch->getLocation();
	cocos2d::Vector<cocos2d::Sprite*> toErase;

	for(auto bomb : _bombs)
	{
		if(bomb->getBoundingBox().containsPoint(touchLocation)){
			AudioEngine::play2d("bomb.mp3");
			//auto explosion = ParticleExplosion::create();
			auto explosion = ParticleSystemQuad::create("explosion.plist");
			explosion->setPosition(bomb->getPosition());
			/*explosion->setTotalParticles(800);
			explosion->setSpeed(3.5f);
			explosion->setLife(300.0f);	*/
			this->addChild(explosion);
			bomb->setVisible(false);
			this->removeChild(bomb);
			toErase.pushBack(bomb);
		}
	}

	for(auto bomb : toErase)
	{
		_bombs.eraseObject(bomb);
	}

	return true;
}

void HelloWorld::initTouch()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::explodeBombs,this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::movePlayerByTouch,this);
	listener->onTouchEnded = [=](Touch* touch, Event* event){};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::initAccelerometer()
{
	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::movePlayerByAccelerometer, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::movePlayerByAccelerometer(cocos2d::Acceleration* acceleration, cocos2d::Event* event)
{
	movePlayerIfPossible(_sprPlayer->getPositionX() + (acceleration->x * 10));
}

bool HelloWorld::onCollision(PhysicsContact& contact)
{
	CCLOG("Collision detected");
	auto playerShape = _sprPlayer->getPhysicsBody()->getFirstShape();
	if(playerShape != contact.getShapeA() && playerShape != contact.getShapeB())
	{
		return false;
	}

	if(_muteItem -> isVisible())
	{
		//CocosDenshion
		/*SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playEffect("uh.wav");*/

		//New audio engine
		AudioEngine::stopAll();
		AudioEngine::play2d("uh.mp3");
	}

	UserDefault::getInstance()->setIntegerForKey("score",_score);
	_director->replaceScene(TransitionFlipX::create(1.0, GameOver::createScene()));
	auto body = _sprBomb -> getPhysicsBody();
	body->setVelocity(Vect());
	body->applyTorque(100900.5f);
	return false;
}

void HelloWorld::setPhysicsBody(cocos2d::Sprite* sprite)
{
	auto body = PhysicsBody::createCircle(sprite->getContentSize().width/2);
	body->setContactTestBitmask(true);
	body->setDynamic(true);
	sprite -> setPhysicsBody(body);
}

void HelloWorld::updateScore(float dt)
{
	_score += 10;
}

void HelloWorld::addBombs(float dt)
{
	Sprite* bomb = nullptr;
	for(int i = 0 ; i < 3 ; i++)
	{
		bomb = Sprite::create("bomb.png");
		bomb->setPosition(CCRANDOM_0_1() * _visibleSize.width, _visibleSize.height + bomb->getContentSize().height/2);
		this->addChild(bomb,1);
		setPhysicsBody(bomb);
		bomb->getPhysicsBody()->setVelocity(Vect(0, ( (CCRANDOM_0_1() + 0.2f) * -250) ));
		_bombs.pushBack(bomb);
	}
}

void HelloWorld::initAudioNewEngine()
{
	if(AudioEngine::lazyInit())
	{
		_musicId = AudioEngine::play2d("music.mp3");
		AudioEngine::setVolume(_musicId, 1);
		AudioEngine::setLoop(_musicId,true);
		CCLOG("Audio initialized successfully");
	}else
	{
		CCLOG("Error while initializing new audio engine");
	}
}

void HelloWorld::initAudio()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music.mp3",true);
	SimpleAudioEngine::getInstance()->preloadEffect("uh.wav");
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
}

void HelloWorld::initPhysics()
{
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onCollision,this);
	getEventDispatcher() ->addEventListenerWithSceneGraphPriority(contactListener,this);

}

void HelloWorld::initMuteButton()
{
	_muteItem = MenuItemImage::create(
                                           "mute.png",
                                           "mute.png",
                                           CC_CALLBACK_1(HelloWorld::muteCallback, this));

	_muteItem->setPosition(Vec2(_visibleSize.width - _muteItem->getContentSize().width/2 ,
					_visibleSize.height - _muteItem->getContentSize().height / 2));

    _unmuteItem = MenuItemImage::create(
                                           "unmute.png",
                                           "unmute.png",
                                           CC_CALLBACK_1(HelloWorld::muteCallback, this));

	_unmuteItem->setPosition(Vec2(_visibleSize.width - _unmuteItem->getContentSize().width/2 ,
					_visibleSize.height - _unmuteItem->getContentSize().height /2));
	_unmuteItem -> setVisible(false);

	auto menu = Menu::create(_muteItem, _unmuteItem , nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

}

void HelloWorld::muteCallback(cocos2d::Ref* pSender)
{
	if(_muteItem -> isVisible())
	{
		//CocosDenshion
		//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		AudioEngine::setVolume(_musicId, 0);


	}else
	{
		//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
		AudioEngine::setVolume(_musicId, 1);
	}

	_muteItem->setVisible(!_muteItem->isVisible());
	_unmuteItem->setVisible(!_muteItem->isVisible());
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	_score = 0;
	_director = Director::getInstance();
    _visibleSize = _director->getVisibleSize();
	auto origin = _director->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
                                           "pause.png",
                                           "pause_pressed.png",
                                           CC_CALLBACK_1(HelloWorld::pauseCallback, this));

	closeItem->setPosition(Vec2(_visibleSize.width - closeItem->getContentSize().width/2 ,
                                closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	_sprBomb = Sprite::create("bomb.png");
	_sprBomb->setPosition(_visibleSize.width / 2, _visibleSize.height + _sprBomb->getContentSize().height/2);
	this->addChild(_sprBomb,1);
	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2());
	bg->setPosition(0,0);
	this->addChild(bg, -1);
	_sprPlayer = Sprite::create("player.png");
	_sprPlayer->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.23);
	setPhysicsBody(_sprPlayer);

	this->addChild(_sprPlayer, 0);
	//Animations
	Vector<SpriteFrame*> frames;
	Size playerSize = _sprPlayer->getContentSize();
	frames.pushBack(SpriteFrame::create("player.png", Rect(0, 0, playerSize.width, playerSize.height)));
	frames.pushBack(SpriteFrame::create("player2.png", Rect(0, 0, playerSize.width, playerSize.height)));
	auto animation = Animation::createWithSpriteFrames(frames,0.2f);
	auto animate = Animate::create(animation);
	_sprPlayer->runAction(RepeatForever::create(animate));
	setPhysicsBody(_sprBomb);
	initPhysics();
	_sprBomb->getPhysicsBody()->setVelocity(Vect(0,-100));
	initTouch();
	initAccelerometer();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	setKeepScreenOnJni(true);
	#endif
	initBackButtonListener();
	schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateScore), 3.0f);
	schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addBombs), 8.0f);
	initAudioNewEngine();
	initMuteButton();
	_bombs.pushBack(_sprBomb);
    return true;
}

void HelloWorld::pauseCallback(cocos2d::Ref* pSender){
	_director->pushScene(TransitionFlipX::create(1.0, Pause::createScene()));
}

void HelloWorld::initBackButtonListener(){
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
		Director::getInstance()->end();
	}
}
