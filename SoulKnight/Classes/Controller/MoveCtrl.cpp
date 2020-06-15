#include "Controller/MoveCtrl.h"
#include "MovingActor/Constant.h"
#include <functional>

//该控制器基于学长的代码编写

MoveController* MoveController::createMoveController()
{
	MoveController* layer = MoveController::create();
	if(layer)
	{
		layer->ctrlInit();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

void MoveController::ctrlInit()
{
	listener = EventListenerKeyboard::create();

	//设置绑定监听器
	listener->onKeyPressed = CC_CALLBACK_2(MoveController::onKeyPress, this);
	listener->onKeyReleased = CC_CALLBACK_2(MoveController::onKeyRelease, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener,2);

}

bool MoveController::update(EventKeyboard::KeyCode keycode, keyPress type)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		if (type == PRESS)
			pressW = true;
		else if (type == RELEASE)
		{
			pressW = false;
		}
			break;
	case EventKeyboard::KeyCode::KEY_S:
		if (type == PRESS)
			pressS = true;
		else
			pressS = false;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		if (type == PRESS)
			pressA = true;
		else
			pressA = false;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		if (type == PRESS)
			pressD = true;
		else
			pressD = false;
	}
	updateDir();
	return true;
}

bool MoveController::updateDir()
{
	if (pressW)
	{
		firstDirection = EDirection::UP;
		if (pressA)
			direction = EDirection::UPLEFT;
		else if (pressD)
			direction = EDirection::UPRIGHT;
		else if (pressS)
			direction = EDirection::NODIR;
		else
			direction = EDirection::UP;
	}
	else if (pressS)
	{
		firstDirection = EDirection::DOWN;
		if (pressA)
			direction = EDirection::DOWNLEFT;
		else if (pressD)
			direction = EDirection::DOWNRIGHT;
		else if (pressW)
			direction = EDirection::NODIR;
		else
			direction = EDirection::DOWN;
	}
	else
	{
		
		if (pressA)
		{
			firstDirection = EDirection::LEFT;
			if (pressD)
				direction = EDirection::NODIR;
			else
				direction = EDirection::LEFT;
		}
		else if (pressD)
		{
			firstDirection = EDirection::RIGHT;
			if (pressA)
				direction = EDirection::NODIR;
			else
				direction = EDirection::RIGHT;
		}
		else
			direction = EDirection::NODIR;
	}
	return true;
}

bool MoveController::onKeyPress(EventKeyboard::KeyCode keycode, Event* event)
{
	update(keycode,keyPress::PRESS);
	if (keycode != EventKeyboard::KeyCode::KEY_J && keycode != EventKeyboard::KeyCode::KEY_K && keycode != EventKeyboard::KeyCode::KEY_L)
	{
		isCanMove = true;
		lastDirection = direction;
	}
	return true;
}

bool MoveController::onKeyRelease(EventKeyboard::KeyCode keycode, Event* event)
{
	update(keycode, keyPress::RELEASE);
	if (keycode != EventKeyboard::KeyCode::KEY_J && keycode != EventKeyboard::KeyCode::KEY_K && keycode != EventKeyboard::KeyCode::KEY_L)
	
		isCanMove = false;

		return true;
}