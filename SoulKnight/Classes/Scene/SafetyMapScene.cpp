#include "SafetyMapScene.h"
#include "StartGameScene.h"
#include "testmanSelectScene.h"
#include "MenuScene.h"
#include "BookScene.h"
#include "SimpleAudioEngine.h" 
#include "MovingActor/Constant.h"
#include "cocos2d.h"

USING_NS_CC;


Scene* SafetyMap::createScene()
{
	return SafetyMap::create();
}
//错误提示
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SafetyMapScene.cpp\n");
}

bool SafetyMap::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance(); 
	audio->playBackgroundMusic("ArtDesigning/Audio/SafetyMap.mp3", true);
	
	initMapLayer();
	initDiamond();
	scheduleUpdate();
	setGrade(0);
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。


	MenuItemImage* testManMenu = MenuItemImage::create(
		"ArtDesigning/Sprite/Fighter/hero.png",
		"ArtDesigning/Sprite/Fighter/hero.png",
		CC_CALLBACK_1(SafetyMap::menuTestManCallBack, this)
	);

	if (testManMenu == nullptr ||
		testManMenu->getContentSize().width <= 0 ||
		testManMenu->getContentSize().height <= 0)
	{
		problemLoading("'testManMenu .png'");
	}
	else
	{
		float x = visibleSize.width / 4*3;
		float y = visibleSize.height - 300;
		testManMenu->setPosition(Vec2(x, y));
	}

	MenuItemImage* menuMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/MENU.png",
		"ArtDesigning/Word&Others/button/MENU.png",
		CC_CALLBACK_1(SafetyMap::menuMenuCallBack, this)
	);

	if (menuMenu == nullptr ||
		menuMenu->getContentSize().width <= 0 ||
		menuMenu->getContentSize().height <= 0)
	{
		problemLoading("'menuMenu .png'");
	}
	else
	{
		menuMenu->setPosition(Vec2(visibleSize.width / 6 * 5 + 30, visibleSize.height - 180));
	}


	MenuItemImage* bookMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/book_closed.png",
		"ArtDesigning/Word&Others/button/book_closed.png",
		CC_CALLBACK_1(SafetyMap::BookMenuCallBack, this)
	);

	if (bookMenu == nullptr ||
		bookMenu->getContentSize().width <= 0 ||
		bookMenu->getContentSize().height <= 0)
	{
		problemLoading("'bookMenu.png'");
	}
	else
	{
		bookMenu->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height/2-20));
	}
	bookMenu->setScale(0.2);

	Menu* mu = Menu::create(testManMenu,menuMenu,bookMenu, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);


	return true;
}


void SafetyMap::initMapLayer()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。
 
	_map = CCTMXTiledMap::create("ArtDesigning/SceneAndMap/SafetyMap/SafetyMap.tmx");
	auto size = _map->getBoundingBox().size;
    _map->setAnchorPoint(Vec2(0.5, 0.5));
    _map->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	
	 addChild(_map, 0, 10000);//
}

void SafetyMap::BookMenuCallBack(cocos2d::Ref* pSender)
{
	//转到菜单项(可返回)
	auto nextScene = BookScene::create();
	Director::getInstance()->pushScene(nextScene);
	MenuItem* item = static_cast<MenuItem*>(pSender);


	log("Touch Book Menu Item %p", item);
}

void SafetyMap::menuMenuCallBack(cocos2d::Ref* pSender)
{
	//转到菜单项(可返回)
	auto nextScene = MenuScene::create();
	Director::getInstance()->pushScene(nextScene);
	MenuItem* item = static_cast<MenuItem*>(pSender);


	log("Touch Menu Menu Item %p", item);
}

void SafetyMap::initDiamond() 
{
	setDiamondNum(100);
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。
   
	diamond = Label::createWithBMFont("fonts/Curlz.fnt", "0");//Label::createWithBMFont表示添加图片字体，”helvetica-32.fnt”为图片字体的格式，”Score:0”表示在屏幕上显示的得分信息 
	diamond->setString(String::createWithFormat("%d", _diamondNum)->getCString());
	this->addChild(diamond, 3);
	float x = visibleSize.width / 6 * 5 - 80;
	float y = visibleSize.height - 180;
	diamond->setPosition(Vec2(x-20 , y+10));
	//diamond->setScale(2);
	diamond->setTag(TAG_layer_Score);
	diamondPicture = Sprite::create("ArtDesigning/Word&Others/others/diamond.png");
	this->addChild(diamondPicture, 3);
	diamondPicture->setPosition(x - 60, y+10);
	auto diamondBG = Sprite::create("ArtDesigning/Word&Others/others/diamondBG.png");
	this->addChild(diamondBG, 2);
	diamondBG->setScale(0.03);
	diamondBG->setPosition(x - 30,y);
}

void SafetyMap::menuTestManCallBack(cocos2d::Ref* pSender)
{
	//转到testmanSelect
	auto nextScene = testmanSelect::create(this);
	Director::getInstance()->pushScene(nextScene);
	MenuItem* item = static_cast<MenuItem*>(pSender);


	log("Touch testmanSelect Menu Item %p", item);
}

void SafetyMap::menuAudioCallBack(cocos2d::Ref* pSender)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if (audio->isBackgroundMusicPlaying()) {
		audio->pauseBackgroundMusic();
	}
	else {
		audio->resumeBackgroundMusic();
	}
}
