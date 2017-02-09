#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOver::create();
    scene->addChild(layer);
    return scene;
}

bool GameOver::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	director = Director::getInstance();  
    visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
    auto pauseItem = MenuItemImage::create(
                                           "play.png",
                                           "play_pressed.png",
                                           CC_CALLBACK_1(GameOver::exitPause, this));
	pauseItem->setPosition(Vec2(origin.x + visibleSize.width - pauseItem->getContentSize().width / 2,
							origin.y + pauseItem->getContentSize().height / 2));
	auto menu = Menu::create(pauseItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2());
	bg->setPosition(0,0);
	this->addChild(bg, -1);
	auto label = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 96);
	label->enableOutline(Color4B(255, 0, 0, 100),6);
	label->enableGlow(Color4B(255, 0, 0, 255));
	label->enableShadow();
    label->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height /2);
	this->addChild(label, 1);
	auto label2 = Label::createWithSystemFont("Your score is", "Arial", 48);
    label2->setPosition(origin.x + visibleSize.width/2,origin.y + visibleSize.height /2.5);
	this->addChild(label2, 1);
	char scoreText[32];
	int score = UserDefault::getInstance()->getIntegerForKey("score",0);
	sprintf(scoreText, "%d", score);
	auto label3 = Label::createWithBMFont("font.fnt", scoreText);
    label3->setPosition(origin.x + visibleSize.width/2,origin.y + visibleSize.height /3.5);
	this->addChild(label3, 1);


    return true;
}

void GameOver::exitPause(cocos2d::Ref* pSender){
	Director::getInstance()->replaceScene(TransitionFlipX::create(1.0, HelloWorld::createScene()));
}
