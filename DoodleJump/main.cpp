#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include <iostream>
#include <windows.h>
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
	IrrlichtDevice *device =
		createDevice(EDT_SOFTWARE, dimension2du(800, 600), 16, false, false, false, 0);
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	
	

	IAnimatedMeshSceneNode* hero = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/knight.x"));
	if (hero)
	{
		hero->setMaterialFlag(EMF_LIGHTING, false);
		hero->setMaterialTexture(0, driver->getTexture("../models_obj/knight.tga"));
		hero->setPosition(vector3df(0, 0, 0));
		hero->setRotation(vector3df(90, 0, 180));
		hero->setLoopMode(false);
										//hero->setFrameLoop(0, 14);
										//hero->setAnimationSpeed(15);
	}

	IAnimatedMeshSceneNode* blocks[10];

	for (int i = 0; i < 10; i++)
	{
		blocks[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/platform00.x"));
		if (blocks[i])
		{
			blocks[i]->setMaterialFlag(EMF_LIGHTING, false);
			blocks[i]->setMaterialTexture(0, driver->getTexture("../models_obj/platform00.tga"));
			f32 randx = rand() % 40;
			if (randx > 20)randx = randx - 40;
			f32 randy = rand() % 30;
			if (randy > 15)randy = randy - 30;
			blocks[i]->setPosition(vector3df(randx, randy, 0));
			blocks[i]->setRotation(vector3df(0, 0, 0));
		}
	}

	KeyReceiver NewRec(hero);
	device->setEventReceiver(&NewRec);
	
	/*IAnimatedMeshSceneNode* block = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/platform00.x"));
	if (block)
	{
		block->setMaterialFlag(EMF_LIGHTING, false);
		block->setMaterialTexture(0, driver->getTexture("../models_obj/platform00.tga"));
		block->setPosition(vector3df(0, -5, 0));
		block->setRotation(vector3df(0, 0, 0));
		//hero->setLoopMode(false);
	}*/
	
	/*IAnimatedMeshSceneNode* block_un = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/platform01.x"));
	if (block_un)
	{
		block_un->setMaterialFlag(EMF_LIGHTING, false);
		block_un->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
		block_un->setPosition(vector3df(-5, -5, 0));
		block_un->setRotation(vector3df(0, 0, 0));
		block_un->setLoopMode(false);
	}*/

	/*IAnimatedMeshSceneNode* spring = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/spring.x"));
	if (spring)
	{
		spring->setMaterialFlag(EMF_LIGHTING, false);
		spring->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
		spring->setPosition(vector3df(5, -5, 0));
		spring->setRotation(vector3df(0, 0, 0));
		spring->setLoopMode(false);
	}*/

	ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(0, 0, 20), vector3df(0,0,0));
	
	ITexture *back_tex = driver->getTexture("../models_obj/background.png");
	ITexture *calc_back = ResizeTexture(back_tex, driver, driver->getScreenSize().Width, driver->getScreenSize().Height);

	while (device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, SColor(0, 200, 200, 200));
		driver->draw2DImage(calc_back, position2d < s32 >(0, 0), rect < s32 >(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height), 0, SColor(255, 255, 255, 255), true);
		smgr->drawAll();
		hero->setPosition(hero->getPosition() + vector3df(0,g,0));
		g -= 0.001;
		for(int i=0;i<10;i++)
			if (hero->getTransformedBoundingBox().intersectsWithBox(blocks[i]->getTransformedBoundingBox()))
		{
			hero->setFrameLoop(0,50);
			g = 0.1;
		}
		
		if (hero->getPosition().X < -17)
			hero->setPosition(hero->getPosition() + vector3df(36, 0, 0));
		if (hero->getPosition().X > 20)
			hero->setPosition(hero->getPosition() + vector3df(-36, 0, 0));
		if (hero->getPosition().Y < -20)
			hero->setPosition(vector3df(0, 0, 0));
		if (NewRec.IsKeyDown(KEY_LEFT))
		{
			hero->setPosition(hero->getPosition()+vector3df(0.1,0,0));
			hero->setRotation(vector3df(hero->getRotation().X, -20, hero->getRotation().Z));
		}
		if (NewRec.IsKeyDown(KEY_RIGHT))
		{
			hero->setPosition(hero->getPosition() + vector3df(-0.1, 0, 0));
			hero->setRotation(vector3df(hero->getRotation().X, 20, hero->getRotation().Z));
		}
		camera->setPosition(vector3df(0,hero->getPosition().Y,20));
		camera->setTarget(vector3df(0, hero->getPosition().Y, 0));

		guienv->drawAll();
		driver->endScene();
	}
	device->drop();

	return 0;
}