﻿#include "Scene_2.h"



Scene_2::Scene_2(Simon * _si, GameTime* _ga)
{
	simon = _si;
	_gameTime = _ga;
	


	LoadResources();

}


Scene_2::~Scene_2()
{
	SAFE_DELETE(TileMap);
	SAFE_DELETE(board);
	SAFE_DELETE(gridGame);
	SAFE_DELETE(camera);
}

void Scene_2::KeyState(BYTE * state)
{
	if (simon->GetFreeze() == true) // Đang bóng băng thì không quan tâm phím
	{
		return;
	}
	 

	if (simon->isJumping && simon->isWalking)
		return;

	if (Game::GetInstance()->IsKeyDown(DIK_DOWN))
	{
		simon->Sit();

		if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
			simon->Right();

		if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
			simon->Left();

		return;
	}
	else
		simon->Stop();

	if (simon->isJumping)
		return;

	if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
	{
		if (simon->isAttacking)
		{
			float vx, vy;
			simon->GetSpeed(vx, vy);
			simon->SetSpeed(0, vy);
			return;
		}

		simon->Right();
		simon->Go();
	}
	else
		if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
		{
			if (simon->isAttacking)
			{
				float vx, vy;
				simon->GetSpeed(vx, vy);
				simon->SetSpeed(0, vy);
				return;
			}
			simon->Left();
			simon->Go();
		}
		else
		{
			simon->Stop();
		}


}

void Scene_2::OnKeyDown(int KeyCode)
{
	if (simon->GetFreeze() == true) // Đang bóng băng thì không quan tâm phím
	{
		return;
	}

	// chưa xét khi hết time hoặc die thì sao?




//	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	if (KeyCode == DIK_ESCAPE)
		DestroyWindow(/*hWnd*/ Game::GetInstance()->GetWindowHandle()); // thoát

	if (KeyCode == DIK_Q)
		simon->SetPosition(SIMON_POSITION_DEFAULT);



	if (KeyCode == DIK_1) // vị trí simon
	{
		DebugOut(L"[SIMON] X = %f , Y = %f \n", simon->GetX() + 10, simon->GetY());
	}




	if (KeyCode == DIK_2) // lấy tọa độ world của chuột 
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(Game::GetInstance()->GetWindowHandle(), &p);
		DebugOut(L"[MOUSE POSITION] x: %d , y = %d\n", p.x + (int)camera->GetXCam(), p.y + (int)camera->GetYCam());

	}


	if (KeyCode == DIK_3) // set vi tri simon 1275 
	{
		DebugOut(L"[SET POSITION SIMON] x = 1275 \n");
		simon->SetPosition(1275.0f, 0);
	}




	if (KeyCode == DIK_X)
	{
		//DebugOut(L"[SIMON] X = %f , Y = %f \n", simon->x + 10, simon->y);

		if (simon->isAttacking == false)
			sound->Play(eSound::soundWhip);

		simon->Attack(simon->_weaponMain);

	}


	if (KeyCode == DIK_Z)
	{
		if (simon->_weaponSub != NULL && simon->_weaponSub->GetFinish() == true && simon->GetHeartCollect() > 0)
		{
			simon->SetHeartCollect(simon->GetHeartCollect() - 1); // giảm 1 heart
			simon->Attack(simon->_weaponSub);
			sound->Play(eSound::soundDagger);
		}
	}

	if (simon->isJumping && simon->isWalking)
	{
		return;
	}





	if (KeyCode == DIK_SPACE)
	{
		if (Game::GetInstance()->IsKeyDown(DIK_LEFT) || Game::GetInstance()->IsKeyDown(DIK_RIGHT))
		{
			simon->Stop();
			float vx, vy;
			simon->GetSpeed(vx, vy);
			simon->SetSpeed(SIMON_WALKING_SPEED * simon->GetTrend()/**vx - 0.0001f*/, vy - SIMON_VJUMP);
			simon->isJumping = 1;
			simon->isWalking = 1;
		}
		else
		{ 
			simon->Jump();
		}



	}

}

void Scene_2::OnKeyUp(int KeyCode)
{
	if (simon->GetFreeze() == true) // Đang bóng băng thì không quan tâm phím
	{
		return;
	}

	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	switch (KeyCode)
	{
	case DIK_SPACE:
		//simon->Sit();
		break;
	}
}


void Scene_2::LoadResources()
{
	TextureManager * _textureManager = TextureManager::GetInstance(); // Đã gọi load resource
	sound = Sound::GetInstance();

	if (simon==NULL)
		simon = new Simon();

	if (_gameTime == NULL)
		_gameTime = new GameTime();
	  
	TileMap = new Map();
	TileMap->LoadMap(eID::MAP2);

	camera = new Camera(Window_Width, Window_Height);
	camera->SetPosition(0, 0);


	board = new Board(0, 0);

	simon->SetPosition(SIMON_POSITION_DEFAULT);
	simon->SetPositionBackup(SIMON_POSITION_DEFAULT);

	gridGame = new Grid();
	gridGame->ReadFileToGrid("Resources/map/Obj_2.txt"); // đọc các object từ file vào Grid



	listItem.clear();
	listEffect.clear();

	 
	// bật nhạc game
	if (sound->isPlaying(eSound::musicState1) == false)
		sound->Play(eSound::musicState1, true);

//	SceneManager::GetInstance()->SetIsNotLoadResource(false);

}

void Scene_2::Update(DWORD dt)
{
	if (simon->GetFreeze() == true)
	{
		simon->UpdateFreeze(dt);
		if (simon->GetFreeze() == true)
			return;
	}


	if (_gameTime->GetTime() >= GAME_TIME_SCENE2) // hết thời gian
	{
		sound->Play(eSound::musicLifeLost);
		if (simon->GetLives() == 0)
			return;
		bool result = simon->LoseLife();
		if (result == true) // còn mạng để chơi tiếp, giảm mạng reset máu xong
		{
			ResetResource(); // reset lại game
		}
		return;
	}
	else
		_gameTime->Update();



	if (GAME_TIME_SCENE2 - _gameTime->GetTime() <= 30) // đúng còn lại 30 giây thì bật sound loop
	{
		if (_gameTime->CheckIsJustChanged() == true) // Kiểm tra _time vừa thay đổi thì mới play nhạc. Nếu chỉ kt <=30s thì cứ mỗi deltatime nó sẽ Play nhạc -> thừa, ko đều
		{
			sound->Play(eSound::soundStopTimer);
		}
	}


	gridGame->GetListObject(listObj, camera); // lấy hết các object "còn Alive" trong vùng camera;
	//DebugOut(L"[Grid] ListObject by Camera = %d\n", listObj.size());


	simon->Update(dt, &listObj);

	camera->SetPosition(simon->GetX() - Window_Width / 2 + 30, camera->GetYCam()); // cho camera chạy theo simon
	camera->Update();

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Update(dt, &listObj);  // đã kiểm tra "Alive" lúc lấy từ lưới ra

	for (UINT i = 0; i < listItem.size(); i++)
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Update(dt, &listObj); // trong các hàm update chỉ kiểm tra va chạm với đất

	for (UINT i = 0; i < listEffect.size(); i++)
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Update(dt);

	CheckCollision();
}





void Scene_2::Render()
{

	TileMap->DrawMap(camera);


	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Render(camera);

	for (UINT i = 0; i < listItem.size(); i++)
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Render(camera);

	for (UINT i = 0; i < listEffect.size(); i++)
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Render(camera);

	simon->Render(camera);

	board->Render(camera, simon, 1, simon->_weaponSub, GAME_TIME_SCENE2 - _gameTime->GetTime());



}

void Scene_2::ResetResource()
{
	SAFE_DELETE(gridGame);
	gridGame = new Grid();
	gridGame->ReadFileToGrid("Resources/map/Obj_2.txt"); // đọc lại các object từ list

	listItem.clear();
	listEffect.clear();

	_gameTime->SetTime(0); // đếm lại từ 0
	sound->Stop(eSound::musicState1); // tắt nhạc nền
	sound->Play(eSound::musicState1, true); // mở lại nhạc nền
}

void Scene_2::CheckCollision()
{
	CheckCollisionWeapon();

	CheckCollisionSimonWithItem();

	CheckCollisionSimonWithObjectHidden();
}

void Scene_2::CheckCollisionWeapon()
{
	// main weapon
	if (simon->_weaponMain->GetFinish() == false) // Vũ khí đang hoạt động
	{
		for (UINT i = 0; i < listObj.size(); i++)
			if (listObj[i]->GetType() == eID::TORCH)
				if (simon->_weaponMain->isCollision(listObj[i]) == true)
				{
					GameObject *gameObjTorch = dynamic_cast<GameObject*>(listObj[i]);
					gameObjTorch->SubHealth(1);

					listEffect.push_back(new Hit((int)gameObjTorch->GetX() + 14, (int)gameObjTorch->GetY() + 14)); // hiệu ứng lửa
					listEffect.push_back(new Fire((int)gameObjTorch->GetX() - 5, (int)gameObjTorch->GetY() + 8)); // hiệu ứng lửa
					listItem.push_back(GetNewItem(gameObjTorch->GetId(), eID::TORCH, gameObjTorch->GetX() + 5, gameObjTorch->GetY()));

					sound->Play(eSound::soundHit);
				}
	}


	// subweapon
	if (simon->_weaponSub != NULL && simon->_weaponSub->GetFinish() == false)
	{
		for (UINT i = 0; i < listObj.size(); i++)
			if (listObj[i]->GetType() == eID::TORCH)
				if (simon->_weaponSub->isCollision(listObj[i]) == true)
				{
					GameObject *gameObjTorch = dynamic_cast<GameObject*>(listObj[i]);

					gameObjTorch->SubHealth(1);

					simon->_weaponSub->SetFinish(true);   // cây kiếm trúng object thì tắt luôn

					listEffect.push_back(new Hit((int)gameObjTorch->GetX() + 14, (int)gameObjTorch->GetY() + 14)); // hiệu ứng lửa
					listEffect.push_back(new Fire((int)gameObjTorch->GetX() - 5, (int)gameObjTorch->GetY() + 8)); // hiệu ứng lửa
					listItem.push_back(GetNewItem(gameObjTorch->GetId(), eID::TORCH, gameObjTorch->GetX() + 5, gameObjTorch->GetY()));

					sound->Play(eSound::soundHit);

				}
	}



}

void Scene_2::CheckCollisionSimonWithItem()
{
	for (UINT i = 0; i < listItem.size(); i++)
		if (listItem[i]->GetFinish() == false && listItem[i]->isWaitingDisplay() == false) // chưa kết thúc và "không phải" đang chờ để hiển thị
		{
			if (simon->isCollisionWithItem(listItem[i]) == true) // có va chạm
			{
				switch (listItem[i]->GetType())
				{
				case eID::LARGEHEART:
				{
					simon->SetHeartCollect(simon->GetHeartCollect() + 5);
					listItem[i]->SetFinish(true);
					sound->Play(eSound::soundCollectItem);
					break;
				}
				case eID::UPGRADEMORNINGSTAR:
				{
					MorningStar * objMorningStar = dynamic_cast<MorningStar*>(simon->_weaponMain);
					objMorningStar->UpgradeLevel(); // Nâng cấp vũ khí roi
					listItem[i]->SetFinish(true);
					simon->SetFreeze(true); // bật trạng thái đóng băng
					sound->Play(eSound::soundCollectWeapon);
					break;
				}

				case eID::ITEMDAGGER:
				{
					SAFE_DELETE(simon->_weaponSub);
					simon->_weaponSub = new Dagger();
					listItem[i]->SetFinish(true);
					sound->Play(eSound::soundCollectWeapon);
					break;
				}

				case eID::MONNEY:
				{
					listItem[i]->SetFinish(true);
					sound->Play(eSound::soundCollectItem);



					break;
				}


				default:
					DebugOut(L"[CheckCollisionSimonWithItem] Khong nhan dang duoc loai Item!\n");
					break;
				}



			}
		}





}

void Scene_2::CheckCollisionSimonWithObjectHidden()
{
	for (UINT i = 0; i < listObj.size(); i++)
	{
		if (listObj[i]->GetType() == eID::OBJECT_HIDDEN)
		{
			GameObject * gameObject = dynamic_cast<GameObject*>(listObj[i]);
			if (gameObject->GetHealth() > 0)
			{
				LPCOLLISIONEVENT e = simon->SweptAABBEx(listObj[i]);
				if (0 < e->t && e->t <= 1) // có va chạm xảy ra
				{
					switch (gameObject->GetId())
					{
					case 7: // đụng trúng cửa
					{


						break;
					}

					case 8:
					{
						sound->Play(eSound::soundDisplayMonney);
						listItem.push_back(GetNewItem(gameObject->GetId(), eID::OBJECT_HIDDEN, simon->GetX(), simon->GetY()));
						// chưa hiện effect 1000d
						simon->SetScore(simon->GetScore() + 1000);

						break;
					}
					}


					gameObject->SubHealth(1);

				}
			}
		}
	}
}




Item * Scene_2::GetNewItem(int Id, eID Type, float X, float Y)
{
	if (Type == eID::TORCH)
	{
		if (Id == 1 || Id == 4)
			return new LargeHeart(X, Y);

		if (Id == 2 || Id == 3)
			return new UpgradeMorningStar(X, Y);

		if (Id == 5)
			return new ItemDagger(X, Y);
	}

	if (Type == eID::OBJECT_HIDDEN)
	{
		if (Id == 8)
			return new Monney(1240, 305);

	}

	return new LargeHeart(X, Y);
}