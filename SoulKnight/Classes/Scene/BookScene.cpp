#include "BookScene.h"
#include "SimpleAudioEngine.h" 
#include "cocos2d.h"

USING_NS_CC;

Scene* BookScene::createScene()
{
	return BookScene::create();
}
//错误提示
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelpScene.cpp\n");
}

bool BookScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。

	//menu
	MenuItemImage* cancelMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/CANCEL.png",
		"ArtDesigning/Word&Others/button/CANCEL.png",
		CC_CALLBACK_1(BookScene::menuCancelCallBack, this)
	);

	if (cancelMenu == nullptr ||
		cancelMenu->getContentSize().width <= 0 ||
		cancelMenu->getContentSize().height <= 0)
	{
		problemLoading("'cancelMenu.png'");
	}
	else
	{
		float x = visibleSize.width / 6*5;
		float y = visibleSize.height - 180;
		cancelMenu->setPosition(Vec2(x, y));
	}


	Menu* mu = Menu::create(cancelMenu, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);

	//background
	auto background = Sprite::create("ArtDesigning/Word&Others/button/book_opened.png");
	if (background == nullptr)

	{
		problemLoading("'book_opened.png'");
	}
	else
	{
		background->setAnchorPoint(Vec2(0.5, 0.5));
		background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
	}
	//////////////////////////////////




	return true;
}
void BookScene::menuCancelCallBack(cocos2d::Ref* pSender)
{
	//返回上一场景
	Director::getInstance()->popScene();
	MenuItem* item = static_cast<MenuItem*>(pSender);

	log("Touch Cancel Menu Item %p", item);
}

