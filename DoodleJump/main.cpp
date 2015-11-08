#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include "library.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device;
IVideoDriver *driver;
ISceneManager *smgr;
IGUIEnvironment *guienv;

wchar_t* about = L" Hell Escape v1.0 \n Created for xpn2015\n\n\n Inspired by #helenarebel";
bool state = false;
bool IsNewGame = true;

class KeyReceiver : public IEventReceiver
{
private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	IAnimatedMeshSceneNode* hero;
public:
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			hero->setRotation(vector3df(hero->getRotation().X, 0, hero->getRotation().Z));
			if (event.KeyInput.Key == KEY_ESCAPE && IsNewGame == false)
				state = !state;
		}
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			switch (event.GUIEvent.EventType)
			{
			case EGET_BUTTON_CLICKED:
				if (id == 101)
				{
					event.GUIEvent.Caller->setText(L"Continue");
					IsNewGame = false;
					state = true;
				}
				if (id == 103)
				{
					device->closeDevice();
				}
				if (id == 102)
				{
					guienv->addMessageBox(L"About", about, true, 1, 0, 108);
				}
				break;
			}
		}

		return false;
	}
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	KeyReceiver(IAnimatedMeshSceneNode* hero) :hero(hero)
	{
		for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
};

//guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", rect<int>(10, 10, 200, 22), true);//pattern for text!
IGUIButton* b1;
void CreateGUI()
{
	guienv->getSkin()->setFont(guienv->getFont("../lib/irrlicht-1.8.3/media/fonthaettenschweiler.bmp"), EGDF_DEFAULT);
	b1 = guienv->addButton(rect<s32>(driver->getScreenSize().Width / 2 - 80, driver->getScreenSize().Height / 2 - 200, driver->getScreenSize().Width / 2 + 80, driver->getScreenSize().Height / 2 - 150), 0, 101, L"New Game");
	IGUIButton* b2 = guienv->addButton(rect<s32>(driver->getScreenSize().Width / 2 - 80, driver->getScreenSize().Height / 2 - 100, driver->getScreenSize().Width / 2 + 80, driver->getScreenSize().Height / 2 - 50), 0, 102, L"About");
	IGUIButton* b3 = guienv->addButton(rect<s32>(driver->getScreenSize().Width / 2 - 80, driver->getScreenSize().Height / 2, driver->getScreenSize().Width / 2 + 80, driver->getScreenSize().Height / 2 + 50), 0, 103, L"Exit");
}

f32 g = -0.1;

void RenderAll(ITexture* back)
{
	driver->beginScene(true, true, SColor(255, 155, 155, 155));
	driver->draw2DImage(back, position2d < s32 >(0, 0), rect < s32 >(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height), 0, SColor(255, 255, 255, 255), true);
	if (state == true)smgr->drawAll();
	if (state == false)guienv->drawAll();
	driver->endScene();
}


int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));

	device = createDevice(EDT_SOFTWARE, dimension2du(800, 600), 16, false, false, false, 0);
	device->setWindowCaption(L"Hell Escape");
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	CreateGUI();
	
	/*-----------------------------HERO_CREATION-----------------------------------------------------------------------------*/
	IAnimatedMeshSceneNode* hero = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/knight.x"));
	if (hero)
	{
		hero->setMaterialFlag(EMF_LIGHTING, false);
		hero->setMaterialTexture(0, driver->getTexture("../models_obj/knight.tga"));
		hero->setPosition(vector3df(0, 0, 0.5));
		hero->setRotation(vector3df(90, 0, 180));
		hero->setLoopMode(false);
		hero->setAnimationSpeed(13);
										//hero->setFrameLoop(0, 14);
										//hero->setAnimationSpeed(15);
	}

	/*-----------------------------------------------------------------------------------------------------------------------*/

	/*------------------------------CREATION_OF_BLOCKS-----------------------------------------------------------------------*/
		
	block blocks[35];
	int y = -5;
	int x = 0;

	f32 randy = -10;
	f32 randx = 0;

	blocks[0] = platform(blocks[0], smgr, driver, 0, -3);

	for (int i = 1; i < 30; i++)
	{
		randx += rand() % 30;
		if (randx > 15) randx -= 30;
		randy += rand() % 4;
		blocks[i] = platform(blocks[i], smgr, driver, randx, randy);
	}
	for (int i = 0; i < 5; i++)
	{
		blocks[30 + i] = platform(blocks[30 + i], smgr, driver, -50, 0);
		set_breakable(blocks[30 + i], smgr, driver);
		blocks[30 + i].node->setFrameLoop(0, 0);
		blocks[30 + i].node->setAnimationSpeed(20);

	}

	/*IAnimatedMeshSceneNode* springs[2];
	for (int i = 0; i < 2; i++)
	{
		springs[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/spring.x"));
		if (springs[i])
		{
			springs[i]->setMaterialFlag(EMF_LIGHTING, false);
			springs[i]->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
			//spring->setPosition(vector3df(5, -5, 0));
			//spring->setRotation(vector3df(0, 0, 0));
			
			springs[i]->setCurrentFrame(0);
			springs[i]->setLoopMode(false);
			//blocks[0].node->addChild(spring);
			springs[i]->setPosition(vector3df(0, 0.5, 0));
		}

	}*/
	IAnimatedMeshSceneNode* spring = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/spring.x"));
	if (spring)
	{
		spring->setMaterialFlag(EMF_LIGHTING, false);
		spring->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
		//spring->setPosition(vector3df(5, -5, 0));
		//spring->setRotation(vector3df(0, 0, 0));
		spring->setFrameLoop(0, 0);
		spring->setLoopMode(false);
		spring->setCurrentFrame(0);
		blocks[0].node->addChild(spring);
		spring->setPosition(vector3df(0, 0.3, 0));
	}

	/*------------------------------------------------------------------------------------------------------------------------*/
	KeyReceiver NewRec(hero);
	device->setEventReceiver(&NewRec);
	//usual platform
	/*IAnimatedMeshSceneNode* block = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/platform00.x"));
	if (block)
	{
		block->setMaterialFlag(EMF_LIGHTING, false);
		block->setMaterialTexture(0, driver->getTexture("../models_obj/platform00.tga"));
		block->setPosition(vector3df(0, -5, 0));
		block->setRotation(vector3df(0, 0, 0));
		//hero->setLoopMode(false);
	}*/
	//breakable platform
	/*IAnimatedMeshSceneNode* block_un = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/platform01.x"));
	if (block_un)
	{
		block_un->setMaterialFlag(EMF_LIGHTING, false);
		block_un->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
		block_un->setPosition(vector3df(-5, -5, 0));
		block_un->setRotation(vector3df(0, 0, 0));
		block_un->setLoopMode(false);
	}*/
	//spring
	/*IAnimatedMeshSceneNode* spring = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/spring.x"));
	if (spring)
	{
		spring->setMaterialFlag(EMF_LIGHTING, false);
		spring->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
		spring->setPosition(vector3df(5, -5, 0));
		spring->setRotation(vector3df(0, 0, 0));
		spring->setLoopMode(false);
	}*/

	ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(0, 0, 20), vector3df(0, 0, 0));
	
	ITexture *back_tex = driver->getTexture("../models_obj/background.png");
	ITexture *calc_back = ResizeTexture(back_tex, driver, driver->getScreenSize().Width, driver->getScreenSize().Height);

	long float score = 0;
	int last_pos = 0;

	while (device->run())
		if (device->isWindowActive())
		{
			RenderAll(calc_back);
						
			if (state == true)
			{
				hero->setPosition(hero->getPosition() + vector3df(0, g, 0));
				g -= 0.001;
				if (fabs(g) < 0.0000009)last_pos = hero->getPosition().Y+1;
				if (g > 0 && hero->getPosition().Y>last_pos)score+=0.1;
				//std::cout << score << ' '<<g<<' '<<last_pos<<'\n';
				//---------------MOVEMENT_(UP_DOWN)--/just_for_debug!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-------------------------
				if (hero->getPosition().Y > 45)
				{
					//score += 45;
					hero->setPosition(vector3df(hero->getPosition().X, hero->getPosition().Y - 30, hero->getPosition().Z));
					for (int i = 0; i < 35; i++)
					{
						blocks[i].node->setPosition(vector3df(blocks[i].node->getPosition().X, blocks[i].node->getPosition().Y - 30, blocks[i].node->getPosition().Z));
					}
					randy = randy - 30;
				}
				if (hero->getPosition().Y < -10)
				{

					state = false;
					IsNewGame = true;
					hero->setPosition(vector3df(0, 20, 0.5));
					g = -0.1;
					b1->setText(L"New Game");
					score = 0;
				}
					//hero->setPosition(vector3df(hero->getPosition().X, 0, hero->getPosition().Z));
				//-------------------------------------------------------------------------------------------------------

				//---------------MOVEMENT_(LEFT_RIGHT)-------------------------------------------------------------------
				if (hero->getPosition().X < -20)
					hero->setPosition(hero->getPosition() + vector3df(39, 0, 0));
				if (hero->getPosition().X > 20)
					hero->setPosition(hero->getPosition() + vector3df(-39, 0, 0));
				//-------------------------------------------------------------------------------------------------------


				for (int i = 0; i < 35; i++)
				{

						if (hero->getTransformedBoundingBox().intersectsWithBox(blocks[i].node->getTransformedBoundingBox()))
							if (hero->getTransformedBoundingBox().getCenter().X >= blocks[i].node->getPosition().X - 0.5 && hero->getTransformedBoundingBox().getCenter().X <= blocks[i].node->getPosition().X + 2.5 && hero->getTransformedBoundingBox().getCenter().Y - 1.5 >= blocks[i].node->getPosition().Y && g <= 0)
							{
								//std::cout << fabs(hero->getTransformedBoundingBox().getCenter().X) << "->" << fabs(blocks[i]->getPosition().X) << '\n';
								if (i < 30)
								{
									hero->setFrameLoop(0, 50);
									if (!blocks[i].node->getChildren().empty())
									{
										if (spring->getTransformedBoundingBox().intersectsWithBox(hero->getTransformedBoundingBox()))
										{
											spring->setFrameLoop(0, 20);
											g = 0.2;
										}
										else g = 0.1;
									}
									else g = 0.1;
								}
								else
								{
									blocks[i].node->setFrameLoop(0, 15);
								}
							}
					

					if (blocks[i].node->getPosition().Y < (hero->getPosition().Y - 15))
					{
						f32 x = rand() % 30;
						if (x > 15)x -= 30;
						int tmpy = rand()%9;
						if (i == 1)tmpy = rand() % 10;
						else if (i < 30)tmpy = rand() % 4;
						//float p = (rand() % 10 * (score / 100));
						
						blocks[i].node->setPosition(vector3df(x, randy+tmpy, 0));
						if(i<30)randy += tmpy;
						if (i == 0) spring->setFrameLoop(0, 0);


						/*	if (blocks[i].IsBreakabel == true)
								set_usual(blocks[i], smgr, driver);*/

						//std::cout << p << " " << score % 1000 << " " << blocks[i].IsBreakabel << '\n';
						

					}
				}
				/*int p = (int(score) / 100) % 5;
				for (int i = 0; i < p; i++)
				{
					f32 x = rand() % 30;
					if (x > 15)x -= 30;
					f32 y = rand() % 10;
					blocks[30 + i].node->setPosition(vector3df(x, randy-y, 0));
					std::cout << "lil!";
				}*/


				if (NewRec.IsKeyDown(KEY_LEFT))
				{
					hero->setPosition(hero->getPosition() + vector3df(0.15, 0, 0));
					hero->setRotation(vector3df(hero->getRotation().X, -20, hero->getRotation().Z));
				}
				if (NewRec.IsKeyDown(KEY_RIGHT))
				{
					hero->setPosition(hero->getPosition() + vector3df(-0.15, 0, 0));
					hero->setRotation(vector3df(hero->getRotation().X, 20, hero->getRotation().Z));
				}
				

				

				camera->setPosition(vector3df(0, hero->getPosition().Y, 20));
				camera->setTarget(vector3df(0, hero->getPosition().Y, 0));
				int m = hero->getPosition().Y;
			}
		}
	device->drop();
	
	return 0;
}