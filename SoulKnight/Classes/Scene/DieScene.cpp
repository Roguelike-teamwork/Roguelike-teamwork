#include "DieScene.h"
#include "StartGameScene.h"
#include "SimpleAudioEngine.h" 
#include "SafetyMapScene.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* DieScene::createScene()
{
	return DieScene::create();
}
//错误提示
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelpScene.cpp\n");
}

bool DieScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("ArtDesigning/Audio/Die.mp3", true);

	//menu
	MenuItemImage* exitMenu = MenuItemImage::create(
		"ArtDesigning/SceneAndMap/StartGame/CANCEL.png",
		"ArtDesigning/SceneAndMap/StartGame/CANCEL.png",
		CC_CALLBACK_1(DieScene::menuExitCallBack, this)
	);

	if (exitMenu == nullptr ||
		exitMenu->getContentSize().width <= 0 ||
		exitMenu->getContentSize().height <= 0)
	{
		problemLoading("'exitMenu.png'");
	}
	else
	{
		float x = visibleSize.width / 4;
		float y = visibleSize.height - 550;
		exitMenu->setPosition(Vec2(x, y));
	}

	MenuItemImage* restartMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/PLAY.png",
		"ArtDesigning/Word&Others/button/PLAY.png",
		CC_CALLBACK_1(DieScene::menuRestartCallBack, this)
	);

	if (restartMenu == nullptr ||
		restartMenu->getContentSize().width <= 0 ||
		restartMenu->getContentSize().height <= 0)
	{
		problemLoading("'cancelMenu.png'");
	}
	else
	{
		float x = visibleSize.width / 2;
		float y = visibleSize.height - 550;
		restartMenu->setPosition(Vec2(x, y));
	}



	auto offMusic = MenuItemImage::create("ArtDesigning/SceneAndMap/StartGame/on.png", "on.png");
	auto onMusic = MenuItemImage::create("ArtDesigning/SceneAndMap/StartGame/off.png", "off.png");
	MenuItemToggle* audioMenu = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(DieScene::menuAudioCallBack, this),
		offMusic, onMusic, NULL
	);
	audioMenu->setPosition(Vec2(visibleSize.width / 4 * 3, visibleSize.height - 550));


	Menu* mu = Menu::create(exitMenu, audioMenu, restartMenu, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);

	//background
	auto background = Sprite::create("ArtDesigning/SceneAndMap/StartGame/Die.png");
	background->setScale(1.5);
	if (background == nullptr)

	{
		problemLoading("'Setting.png'");
	}
	else
	{
		background->setAnchorPoint(Vec2(0.5, 0.5));
		background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
	}

	return true;
}
void DieScene::menuExitCallBack(cocos2d::Ref* pSender)
{//转到start地图
	auto nextScene = StartGame::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = static_cast<MenuItem*>(pSender);
}
void DieScene::menuRestartCallBack(cocos2d::Ref* pSender)
{
	auto nextScene = SafetyMap::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = static_cast<MenuItem*>(pSender);
}
void DieScene::menuAudioCallBack(cocos2d::Ref* pSender)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if (audio->isBackgroundMusicPlaying()) {
		audio->pauseBackgroundMusic();
	}
	else {
		audio->resumeBackgroundMusic();
	}
}
