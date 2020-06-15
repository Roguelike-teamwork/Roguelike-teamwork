#include "testmanSelectScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h" 
#include "cocos2d.h"

USING_NS_CC;

Scene* testmanSelect::createScene()
{
	return testmanSelect::create();
}
//错误提示
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in testmanSelectScene.cpp\n");
}

bool testmanSelect::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。

	//exitMenu
	MenuItemImage* exitMenu = MenuItemImage::create(
		"ArtDesigning/SceneAndMap/StartGame/EXIT.png",
		"ArtDesigning/SceneAndMap/StartGame/EXITchosen.png",
		CC_CALLBACK_1(testmanSelect::menuExitCallBack, this)
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
	//playMenu
	MenuItemImage* playMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/PLAY.png",
		"ArtDesigning/Word&Others/button/PLAY.png",
		CC_CALLBACK_1(testmanSelect::menuPlayCallBack, this)
	);

	if (playMenu == nullptr ||
		playMenu->getContentSize().width <= 0 ||
		playMenu->getContentSize().height <= 0)
	{
		problemLoading("'playMenu.png'");
	}
	else
	{
		float x = visibleSize.width / 4*3;
		float y = visibleSize.height - 550;
		playMenu->setPosition(Vec2(x, y));
	}

	//audio
	auto offMusic = MenuItemImage::create("ArtDesigning/SceneAndMap/StartGame/on.png", "on.png");
	auto onMusic = MenuItemImage::create("ArtDesigning/SceneAndMap/StartGame/off.png", "off.png");
	offMusic->setScale(0.5);
	onMusic->setScale(0.5);
	MenuItemToggle* audioMenu = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(testmanSelect::menuAudioCallBack, this),
		offMusic, onMusic, NULL
	);
	audioMenu->setPosition(Vec2(visibleSize.width / 6 * 5 + 40, visibleSize.height - 180));

	//mu
	Menu* mu = Menu::create(exitMenu, playMenu,audioMenu, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);

	//background
	auto background = CCTMXTiledMap::create("ArtDesigning/SceneAndMap/StartGame/Helpbg.tmx");
	if (background == nullptr)

	{
		problemLoading("'helpbg.png'");
	}
	else
	{
		background->setAnchorPoint(Vec2(0.5, 0.5));
		background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
	}

	return true;
}
void testmanSelect::menuExitCallBack(cocos2d::Ref* pSender)
{
	//返回safetyMap
	Director::getInstance()->popScene();
	MenuItem* item = static_cast<MenuItem*>(pSender);


	log("Touch Exit Menu Item %p", item);
}

void testmanSelect::menuPlayCallBack(cocos2d::Ref* pSender)
{
	//开始游戏
	auto nextScene = GameScene::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = static_cast<MenuItem*>(pSender);
	
	log("Touch Play Menu Item %p", item);
}
void testmanSelect::menuAudioCallBack(cocos2d::Ref* pSender)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if (audio->isBackgroundMusicPlaying()) {
		audio->pauseBackgroundMusic();
	}
	else {
		audio->resumeBackgroundMusic();
	}
}
