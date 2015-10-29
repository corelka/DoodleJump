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

//guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", rect<int>(10, 10, 200, 22), true);//pattern for text!

f32 g = -0.1;

int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));
	IrrlichtDevice *device =
		createDevice(EDT_SOFTWARE, dimension2du(800, 600), 16, false, false, false, 0);
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	
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
		
	block blocks[30];
	int y = -5;
	int x = 0;

	f32 randy = -10;
	f32 randx = 0;

	for (int i = 0; i < 30; i++)
	{
		randx += rand() % 30;
		if (randx > 15) randx -= 30;
		randy += rand() % 4;
		blocks[i] = platform(blocks[i], smgr, driver, randx, randy);
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

	long long points = 0;
	int last_block = 0;

	while (device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, SColor(0, 200, 200, 200));
		driver->draw2DImage(calc_back, position2d < s32 >(0, 0), rect < s32 >(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height), 0, SColor(255, 255, 255, 255), true);
		smgr->drawAll();
		hero->setPosition(hero->getPosition() + vector3df(0,g,0));
		g -= 0.001;
		
		/*---------------MOVEMENT_(UP_DOWN)--/just_for_debug!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-------------------------*/
		if (hero->getPosition().Y > 45)
		{
			hero->setPosition(vector3df(hero->getPosition().X, hero->getPosition().Y-30, hero->getPosition().Z));
			for (int i = 0; i < 30; i++)
			{
				blocks[i].node->setPosition(vector3df(blocks[i].node->getPosition().X, blocks[i].node->getPosition().Y - 30, blocks[i].node->getPosition().Z));
			}
			randy = randy - 30;
		}
		if (hero->getPosition().Y < -10)
			hero->setPosition(vector3df(hero->getPosition().X, 0, hero->getPosition().Z));
		/*-------------------------------------------------------------------------------------------------------*/

		/*---------------MOVEMENT_(LEFT_RIGHT)-------------------------------------------------------------------*/
		if (hero->getPosition().X < -20)
			hero->setPosition(hero->getPosition() + vector3df(39, 0, 0));
		if (hero->getPosition().X > 20)
			hero->setPosition(hero->getPosition() + vector3df(-39, 0, 0));
		/*-------------------------------------------------------------------------------------------------------*/
		
		
		for (int i = 0; i < 30; i++)
		{
			if (hero->getTransformedBoundingBox().intersectsWithBox(blocks[i].node->getTransformedBoundingBox()))
				if (hero->getTransformedBoundingBox().getCenter().X >= blocks[i].node->getPosition().X - 0.5 && hero->getTransformedBoundingBox().getCenter().X <= blocks[i].node->getPosition().X + 2.5 && hero->getTransformedBoundingBox().getCenter().Y - 1.5 >= blocks[i].node->getPosition().Y)
				{
					//std::cout << fabs(hero->getTransformedBoundingBox().getCenter().X) << "->" << fabs(blocks[i]->getPosition().X) << '\n';
					hero->setFrameLoop(0,50);

					g = 0.1;
					if (last_block != i)
					{
						last_block = i;
						points++;
					}
				}
			if (blocks[i].node->getPosition().Y < (hero->getPosition().Y - 15))
			{
				f32 x = rand() % 30;
				if (x > 15)x -= 30;
				randy += rand() % 4;
				float p = (rand() % 10 * (points%500));
				if (blocks[i].IsBreakabel == false && p > 50)
					set_breakable(blocks[i], smgr, driver);

				//else set_usual(&blocks[i], smgr, driver);
				
				std::cout << p << " " << points << " " << blocks[i].IsBreakabel << '\n';
				blocks[i].node->setPosition(vector3df(x, randy, 0));
			}
		}
		
		
		if (NewRec.IsKeyDown(KEY_LEFT))
		{
			hero->setPosition(hero->getPosition()+vector3df(0.15,0,0));
			hero->setRotation(vector3df(hero->getRotation().X, -20, hero->getRotation().Z));
		}
		if (NewRec.IsKeyDown(KEY_RIGHT))
		{
			hero->setPosition(hero->getPosition() + vector3df(-0.15, 0, 0));
			hero->setRotation(vector3df(hero->getRotation().X, 20, hero->getRotation().Z));
		}

		

		camera->setPosition(vector3df(0,hero->getPosition().Y,20));
		camera->setTarget(vector3df(0, hero->getPosition().Y, 0));
		int m = hero->getPosition().Y;
		//std::cout << points << '\n';
		guienv->drawAll();
		driver->endScene();
	}
	device->drop();

	return 0;
}