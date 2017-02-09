#include "AppDelegate.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
	// OpenGL initialization done by cocos project creation script
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Happy Bunny");
		glview->setFrameSize(480, 800);
        director->setOpenGLView(glview);
	}

	Size screenSize = glview->getFrameSize();
	Size designSize(768, 1280);
	std::vector<std::string> searchPaths;	
	searchPaths.push_back("sounds");
	searchPaths.push_back("particles");

	if (screenSize.height > 800){
		//High Resolution
		searchPaths.push_back("images/high");
		director->setContentScaleFactor(1280.0f / designSize.height);
	}
	else if (screenSize.height > 600){
		//Mid resolution
		searchPaths.push_back("images/mid");
		director->setContentScaleFactor(800.0f / designSize.height);
	}
	else{
		//Low resolution
		searchPaths.push_back("images/low");
		director->setContentScaleFactor(320.0f / designSize.height);
	}
	FileUtils::getInstance()->setSearchPaths(searchPaths);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);
    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
