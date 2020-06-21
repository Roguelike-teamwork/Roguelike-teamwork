#include "testmanSelectScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h" 
#include "cocos2d.h"
#include "Scene/Stage_1.h"

USING_NS_CC;

Scene* testmanSelect::createScene(SafetyMap* scene) {

	return testmanSelect::create(scene);
}
//错误提示
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in testmanSelectScene.cpp\n");
}

testmanSelect* testmanSelect::create(SafetyMap* scene)
{
	testmanSelect* newScene = new testmanSelect();
	if (newScene && newScene->init(scene))
	{
		newScene->autorelease();
		return newScene;
	}
	CC_SAFE_DELETE(newScene);
	return NULL;
}

bool testmanSelect::init(SafetyMap* scene)
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。
	setPSafetyMap(scene);
	initDiamond();
	initUpgrade();
	//exitMenu
	MenuItemImage* exitMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/CANCEL.png",
		"ArtDesigning/Word&Others/button/CANCEL.png",
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

	MenuItemImage* menuMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/MENU.png",
		"ArtDesigning/Word&Others/button/MENU.png",
		CC_CALLBACK_1(testmanSelect::menuMenuCallBack, this)
	);

	if (menuMenu == nullptr ||
		menuMenu->getContentSize().width <= 0 ||
		menuMenu->getContentSize().height <= 0)
	{
		problemLoading("'menuMenu .png'");
	}
	else
	{
		float x = visibleSize.width / 6 * 5 + 30;
		float y = visibleSize.height - 180;
		menuMenu->setPosition(Vec2(x,y));
	}
	
	MenuItemImage* upgradeMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/BG.png",
		"ArtDesigning/Word&Others/button/BG.png",
		CC_CALLBACK_1(testmanSelect::menuUpgradeCallBack, this)
	);

	if (upgradeMenu == nullptr ||
		upgradeMenu->getContentSize().width <= 0 ||
		upgradeMenu->getContentSize().height <= 0)
	{
		problemLoading("'upgradeMenu .png'");
	}
	else
	{
		float x = visibleSize.width / 2;
		float y = visibleSize.height - 550;
		upgradeMenu->setPosition(Vec2(x, y));
	}
	
	//mu
	Menu* mu = Menu::create(exitMenu, playMenu,menuMenu,upgradeMenu, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);
	//background
	auto background = Sprite::create("ArtDesigning/SceneAndMap/SafetyMap/Knight.png");
	if (background == nullptr)

	{
		problemLoading("'knight.png'");
	}
	else
	{
		background->setAnchorPoint(Vec2(0.5, 0.5));
		background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
	}

	return true;
}
void testmanSelect::initUpgrade()
{
	
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标

	setGrade(getPSafetyMap()->getGrade());
	grade = Sprite::create("ArtDesigning/Word&Others/others/grade0.png");
	grade->setTexture(String::createWithFormat("ArtDesigning/Word&Others/others/grade%d.png", getGrade())->getCString());
	grade->setAnchorPoint(Vec2(0.5, 0.5));
	grade->setScale(0.4);
	grade->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 50);
	this->addChild(grade, 2);
	auto fighter_half = Sprite::create("ArtDesigning/Word&Others/others/hero_half.png");
	fighter_half->setAnchorPoint(Vec2(0.5, 0.5));
	fighter_half->setScale(3);
	fighter_half->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y+65);
	this->addChild(fighter_half,2);
}

void testmanSelect::menuUpgradeCallBack(cocos2d::Ref* pSender)
{
	if (getGrade() < 3)
	{
		setGrade(getGrade() + 1);
		setDiamondNum(getDiamondNum() - getGrade() * 10);
		getPSafetyMap()->setDiamondNum(getDiamondNum());
		getPSafetyMap()->setGrade(getGrade());
		getPSafetyMap()->diamond->setString(String::createWithFormat("%d", _diamondNum)->getCString());
		diamond->setString(String::createWithFormat("%d", _diamondNum)->getCString());
		grade->setTexture(String::createWithFormat("ArtDesigning/Word&Others/others/grade%d.png", getGrade())->getCString());
	}
	MenuItem* item = static_cast<MenuItem*>(pSender);
	log("Touch upgrade Menu Item %p", item);
}

void testmanSelect::menuMenuCallBack(cocos2d::Ref* pSender)
{
	//转到菜单项(可返回)
	auto nextScene = MenuScene::create();
	Director::getInstance()->pushScene(nextScene);
	MenuItem* item = static_cast<MenuItem*>(pSender);


	log("Touch Menu Menu Item %p", item);
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
	auto nextScene = Stage_1::create(this);
	Director::getInstance()->pushScene(nextScene);
	MenuItem* item = static_cast<MenuItem*>(pSender);
	
	log("Touch Play Menu Item %p", item);
}

void testmanSelect::initDiamond()
{
	setDiamondNum(getPSafetyMap()->getDiamondNum());
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。

	diamond = Label::createWithBMFont("fonts/Curlz.fnt", "0");//Label::createWithBMFont表示添加图片字体，”helvetica-32.fnt”为图片字体的格式，”Score:0”表示在屏幕上显示的得分信息 
	diamond->setString(String::createWithFormat("%d", _diamondNum)->getCString());
	this->addChild(diamond, 3);
	float x = visibleSize.width / 6 * 5 - 20;
	float y = visibleSize.height - 180;
	diamond->setPosition(Vec2(x - 20, y + 10));
	//diamond->setScale(2);
	diamond->setTag(TAG_layer_Score);
	diamondPicture = Sprite::create("ArtDesigning/Word&Others/others/diamond.png");
	this->addChild(diamondPicture, 3);
	diamondPicture->setPosition(x - 60, y + 10);
	auto diamondBG = Sprite::create("ArtDesigning/Word&Others/others/diamondBG.png");
	this->addChild(diamondBG, 2);
	diamondBG->setScale(0.03);
	diamondBG->setPosition(x - 30, y);
}
