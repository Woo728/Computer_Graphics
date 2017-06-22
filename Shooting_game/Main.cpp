// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <fmod.h>
#include <string>
#include "Sound.h"

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define ENEMY_NUM 25
#define BULLET_NUM 100



// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface
LPD3DXFONT dxfont;    // the pointer to the font object
int t_score = 0;
char str[100];
bool keyup = true;
bool flag_hero;
bool flag_explosion;

// 시간
FLOAT t = .0f;			
DWORD dwOldTime = 0;
FLOAT starttime;
FLOAT endtime;
FLOAT playtime;

// sprite declarations
LPDIRECT3DTEXTURE9 sprite;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_hero;
LPDIRECT3DTEXTURE9 sprite_hero1; // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_enemy;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_bullet;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_explosion;


									 // function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory

void init_game(void);
void do_game_logic(void);
bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1);



// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

using namespace std;


enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };


//기본 클래스 
class entity {

public:
	float x_pos;
	float y_pos;
	int status;
	int HP;

};


bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1)
{

	if (((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1)) < (((size0 + size1) * (size0 + size1))/5))
		return true;
	else
		return false;

}



//주인공 클래스 
class Hero :public entity {

public:
	void move(int i);
	void init(float x, float y);
	bool check_collision(float x, float y);
};

void Hero::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}

void Hero::move(int i)
{
	switch (i)
	{
	case MOVE_UP:
		y_pos -= 8 * t;
		break;

	case MOVE_DOWN:
		y_pos += 8 * t;
		break;


	case MOVE_LEFT:
		x_pos -= 6 * t;
		break;


	case MOVE_RIGHT:
		x_pos += 6 * t;
		break;

	}

}

bool Hero::check_collision(float x, float y)
{

	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		return true;
	}
	else {
		return false;
	}
}




// 적 클래스 
class Enemy :public entity {

public:
	void fire();
	void init(float x, float y);
	void move();

};

void Enemy::init(float x, float y)
{

	x_pos = x;
	y_pos = y;
	flag_explosion = false;
}


void Enemy::move()
{
	x_pos -= 1 * t;

}

void Enemy::fire()
{
	flag_explosion = true;
}






// 총알 클래스 
class Bullet :public entity {

public:
	bool bShow = false;
	int score = 0;

	void init(float x, float y);
	void move();
	bool show();
	void hide();
	void active();
	bool check_collision(float x, float y);


};


bool Bullet::check_collision(float x, float y)
{

	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		bShow = false;
		t_score = t_score + 10;
		return true;

	}
	else {

		return false;
	}
}




void Bullet::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}



bool Bullet::show()
{
	return bShow;

}


void Bullet::active()
{
	bShow = true;

}



void Bullet::move()
{
	x_pos += 20 * t;
}

void Bullet::hide()
{
	bShow = false;
}




//객체 생성 
Hero hero;
Enemy enemy[ENEMY_NUM];
Bullet bullet[BULLET_NUM];
CSound sound;



// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"street flight",
		WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	//게임 오브젝트 초기화 
	init_game();


	//sound.PlaySoundBG(1);


	// enter the main loop:
	MSG msg;

	while (TRUE)
	{
		starttime = timeGetTime();
		DWORD starting_point = GetTickCount();
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		do_game_logic();

		render_frame();

		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);




		while ((GetTickCount() - starting_point) < 10);
		
		endtime = timeGetTime();
		playtime += (endtime - starttime);
	}

	//sound.StopSoundBG(1);
	
	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_SPACE:
			keyup = true;       
			break;
		}
	}break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"img\\nightskycut.png",    // the file name
		800,    // default width
		560,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"img\\sasuke(w).png",    // the file name
		704,    // default width
		64,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"img\\attack(w).png",    // the file name
		256,    // default width
		64,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero1);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"img\\enemy_1.png",    // the file name
		1152,    // default width
		64,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"img\\weapon.png",    // the file name
		192,    // default width
		64,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_bullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"img\\explosion.png",    // the file name
		480,    // default width
		80,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_explosion);    // load to sprite

	D3DXCreateFont(d3ddev,    // the D3D Device
		20,    // font height of 30
		0,    // default font width
		FW_BOLD,    // font weight
		1,    // not using MipLevels
		false,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		DEFAULT_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		L"Arial",    // use Facename Arial
		&dxfont);    // the font object


	return;
}


void init_game(void)
{
	//객체 초기화 
	hero.init(50, 250);
	hero.HP = 6;

	//적들 초기화 
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		enemy[i].init((float)(rand() % 300 + 700), rand() % 430 + 60);
	}

	//총알 초기화
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].init(hero.x_pos + 0.5f, hero.y_pos);
	}

	string strBGFileName[] = { "sound\\Naruto_bgm.mp3" };
	string strEFFFileName[] = { "sound\\suriken.mp3", "sound\\bomb.mp3", "sound\\whip.mp3" };
	sound.CreateBGsound(1, strBGFileName);
	sound.CreateEFFsound(3, strEFFFileName);
}


void do_game_logic(void)
{
	//주인공 처리 
	if (KEY_DOWN(VK_UP))
		hero.move(MOVE_UP);

	if (KEY_DOWN(VK_DOWN))
		hero.move(MOVE_DOWN);

	if (KEY_DOWN(VK_LEFT))
		hero.move(MOVE_LEFT);

	if (KEY_DOWN(VK_RIGHT))
		hero.move(MOVE_RIGHT);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (hero.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
		{
			hero.HP--;
			//sound.PlaySoundEFF(3);
			hero.init(50, 250);
			enemy[i].init((float)(rand() % 300 + 700), rand() % 430 + 60);
		}
		sound.Update();
	}

	//적들 처리 
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].x_pos < 0)
			enemy[i].init((float)(rand() % 300 + 700), rand() % 430 + 60);
		else
			enemy[i].move();
	}


	//총알 처리



	if (KEY_DOWN(VK_SPACE))
	{
		if (keyup == true)
		{
			for (int i = 0; i < BULLET_NUM; i++)
			{
				if (bullet[i].show() == false)
				{
					//sound.PlaySoundEFF(1);
					bullet[i].active();
					bullet[i].init(hero.x_pos + 0.5f, hero.y_pos);
					break;
				}
				sound.Update();
			}
			keyup = false;
		}
	}

	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].show() == true)
		{
			if (bullet[i].x_pos > 1000)
				bullet[i].hide();
			else
				bullet[i].move();
		}

	}

	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].show() == true)
		{
			for (int j = 0; j < ENEMY_NUM; j++)
			{
				if (bullet[i].check_collision(enemy[j].x_pos, enemy[j].y_pos) == true)
				{
					//sound.PlaySoundEFF(2);
					enemy[j].init((float)(rand() % 300 + 700), rand() % 430 + 60);
					bullet[i].hide();
				}
				sound.Update();
			}
		}
	}


}
	


		
		
	


// this is the function used to render a single frame
void render_frame(void)
{
	t = (timeGetTime() - dwOldTime) *.05f;
	dwOldTime = timeGetTime();

	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene


	


	// create a RECT to contain the text
	static RECT textbox;

	SetRect(&textbox, 10, 20, 0, 0);

	sprintf( str, "Total Score : %d   Total time : %3.3f", t_score, (playtime/1000));

	dxfont->DrawTextA(NULL, str, -1, &textbox, DT_NOCLIP, D3DXCOLOR(255.0f, 255.0f, 255.0f, 255.0f));


	SetRect(&textbox, 10, 560, 0, 0);
	switch (hero.HP)
	{
	case 6:
		sprintf(str, "HP ♥ ♥ ♥ ♥ ♥ ♥");
		break;
	case 5:
		sprintf(str, "HP ♥ ♥ ♥ ♥ ♥");
		break;
	case 4:
		sprintf(str, "HP ♥ ♥ ♥ ♥");
		break;
	case 3:
		sprintf(str, "HP ♥ ♥ ♥");
		break;
	case 2:
		sprintf(str, "HP ♥ ♥");
		break;
	case 1:
		sprintf(str, "HP ♥");
		break;
	case 0:
		sprintf(str, "game over");
		break;
	}

	dxfont->DrawTextA(NULL, str, -1, &textbox, DT_NOCLIP, D3DXCOLOR(255.0f, 255.0f, 255.0f, 255.0f));



	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    


	//BACKGROUND
	RECT part0;
	SetRect(&part0, 0, 50, 800, 550);
	D3DXVECTOR3 center0(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position0(0.0f, 50.0f, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(sprite, &part0, &center0, &position0, D3DCOLOR_ARGB(255, 255, 255, 255));


	if (KEY_DOWN(VK_SPACE))
		flag_hero = true;
	if(KEY_UP(VK_SPACE))
		flag_hero = false;

	switch (flag_hero)
	{
	case false:
	{
		////주인공 
		static double frame = 10.0;
		if (frame == 10.0) frame = 0.0;
		if (frame < 10.0) frame = frame + 0.5;

		int xpos = (int)frame * 64;

		RECT part;
		SetRect(&part, xpos, 0, xpos + 64, 64);
		D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position(hero.x_pos, hero.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	case true:
	{
		////주인공 공격시
		static double frame_a = 5.0;
		if (KEY_DOWN(VK_SPACE)) frame_a = 0.0;
		if (frame_a < 5.0) frame_a = frame_a + 0.5;

		int xpos_a = (int)frame_a * 64;

		RECT part_a;
		SetRect(&part_a, xpos_a, 0, xpos_a + 64, 64);
		D3DXVECTOR3 center_a(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position_a(hero.x_pos, hero.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_hero1, &part_a, &center_a, &position_a, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	}

	////총알 
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].bShow == true)
		{
			static double frame1 = 2.0;
			if (frame1 == 2.0) frame1 = 0.0;
			if (frame1 < 2.0) frame1 = frame1 + 0.5;

			int xpos1 = (int)frame1 * 64;

			RECT part1;
			SetRect(&part1, xpos1, 0, xpos1 + 64, 64);
			D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position1(bullet[i].x_pos, bullet[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_bullet, &part1, &center1, &position1, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}


	////enemy
	static double frame2 = 17.0;
	if (frame2 == 17.0) frame2 = 0.0;
	if (frame2 < 17.0) frame2 = frame2 + 0.5;

	int xpos2 = (int)frame2 * 64;

	RECT part2;
	SetRect(&part2, xpos2, 0, xpos2 + 64, 64);
	D3DXVECTOR3 center2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

	for (int i = 0; i<ENEMY_NUM; i++)
	{

		D3DXVECTOR3 position2(enemy[i].x_pos, enemy[i].y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_enemy, &part2, &center2, &position2, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (flag_explosion == true)
	{
		static double frame2_e = 5.0;
		if (frame2_e == 5.0) frame2_e = 0.0;
		if (frame2_e < 5.0) frame2_e = frame2_e + 0.5;

		int xpos2_e = (int)frame2_e * 80;

		RECT part2_e;
		SetRect(&part2_e, xpos2_e, 0, xpos2_e + 80, 80);
		D3DXVECTOR3 center2_e(8.0f, 8.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY_NUM; i++)
		{

			D3DXVECTOR3 position2_e(enemy[i].x_pos, enemy[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_explosion, &part2_e, &center2_e, &position2_e, D3DCOLOR_ARGB(127, 255, 255, 255));
		}
	}
	

	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	sprite->Release();
	d3ddev->Release();
	d3d->Release();

	//객체 해제 
	sprite_hero->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();

	sound.ReleaseSound();

	return;
}
