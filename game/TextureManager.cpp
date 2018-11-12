﻿#include "TextureManager.h"

TextureManager* TextureManager::__Instance = NULL;

TextureManager * TextureManager::GetInstance()
{
	if (__Instance == NULL)
		__Instance = new TextureManager();
	return __Instance;
}

void TextureManager::AddTexture(eID type, GTexture *texture)
{
	_ArrTextures[type] = texture;
}

GTexture * TextureManager::GetTexture(eID type)
{
	return _ArrTextures[type];
}

void TextureManager::LoadResource()
{
	AddTexture(eID::FONT, new GTexture("Resources/font/1.png", 13, 3, 37, 0));
	AddTexture(eID::BOARD, new GTexture("Resources/blackboard.png"));
	AddTexture(eID::BOARDHEALTH, new GTexture("Resources/heal.png", 3, 1, 3));
	AddTexture(eID::RENDERBBOX, new GTexture("textures/bbox.png")); // dùng để vẽ BBOX

	AddTexture(eID::MAP1, new GTexture("Resources/map/1.png", 10, 4, 40));



	AddTexture(eID::BRICK, new GTexture("Resources/ground/2.png"));
	AddTexture(eID::TORCH, new GTexture("Resources/ground/0.png", 2, 1, 2));


	AddTexture(eID::ITEMDAGGER, new GTexture("Resources/item/4.png"));
	AddTexture(eID::LARGEHEART, new GTexture("Resources/item/1.png"));
	AddTexture(eID::UPGRADEMORNINGSTAR, new GTexture("Resources/item/3.png"));

	AddTexture(eID::DAGGER, new GTexture("Resources/weapon/1.png"));
	AddTexture(eID::MORNINGSTAR, new GTexture("Resources/weapon/morningstar.png", 4, 3, 12));

	AddTexture(eID::SIMON, new GTexture("Resources/simon.png", 8, 3, 24));


	AddTexture(eID::FIRE, new GTexture("Resources/other/1.png", 4, 1, 4));
	AddTexture(eID::HIT, new GTexture("Resources/other/0.png", 2,1,2));

}





TextureManager::TextureManager()
{
	LoadResource();// load đúng 1 lần
}


TextureManager::~TextureManager()
{
}