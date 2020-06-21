#include "HelpScene.h"
#include "StartGameScene.h"
#include "SimpleAudioEngine.h" 
#include "cocos2d.h"

USING_NS_CC;

Scene* Help::createScene()
{
	return Help::create();
}
//错误提示
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelpScene.cpp\n");
}

bool Help::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。

	//menu
	MenuItemImage* exitMenu = MenuItemImage::create(
		"ArtDesigning/SceneAndMap/StartGame/EXIT.png",
		"ArtDesigning/SceneAndMap/StartGame/EXITchosen.png",
		CC_CALLBACK_1(Help::menuExitCallBack, this)
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
		float y = visibleSize.height - 600;
		exitMenu->setPosition(Vec2(x, y));
	}


	Menu* mu = Menu::create(exitMenu,NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 11);


	Sprite* bg = Sprite::create("ArtDesigning/Word&Others/others/board.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setScale(0.6);
	float x = visibleSize.width / 2+120;
	float y = visibleSize.height/ 2+60;
	bg->setPosition(Vec2(x, y));
	this->addChild(bg,2);
	//background
	auto background = Sprite::create("ArtDesigning/SceneAndMap/StartGame/helpbg.png");
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
void Help::menuExitCallBack(cocos2d::Ref* pSender)
{
	//转到start地图
	auto nextScene = StartGame::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = static_cast<MenuItem*>(pSender);


	log("Touch Exit Menu Item %p", item);
}

