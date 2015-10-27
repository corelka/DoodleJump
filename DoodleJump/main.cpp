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

int main(int argc, char** argv)
{
	IrrlichtDevice *device =
		createDevice(EDT_SOFTWARE, dimension2du(800, 600), 16, false, false, false, 0);
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	
	KeyReceiver NewRec;
	device->setEventReceiver(&NewRec);

	IAnimatedMeshSceneNode* hero = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/knight.x"));
	if (hero)
	{
		hero->setMaterialFlag(EMF_LIGHTING, false);
		hero->setMaterialTexture(0, driver->getTexture("../models_obj/knight.tga"));
		hero->setPosition(vector3df(0, 0, 0));
		hero->setRotation(vector3df(90, 0, 180));
										//hero->setFrameLoop(0, 14);
										//hero->setAnimationSpeed(15);
	}
	
	IAnimatedMeshSceneNode* block = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/platform01.x"));
	if (block)
	{
		block->setMaterialFlag(EMF_LIGHTING, false);
		block->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
		block->setPosition(vector3df(0, -5, 0));
		block->setRotation(vector3df(0, 0, 0));
	}
	
	IAnimatedMeshSceneNode* block_un = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/platform00.x"));
	if (block_un)
	{
		block_un->setMaterialFlag(EMF_LIGHTING, false);
		block_un->setMaterialTexture(0, driver->getTexture("../models_obj/platform00.tga"));
		block_un->setPosition(vector3df(-5, -5, 0));
		block_un->setRotation(vector3df(0, 0, 0));
	}

	IAnimatedMeshSceneNode* spring = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../models_obj/spring.x"));
	if (spring)
	{
		spring->setMaterialFlag(EMF_LIGHTING, false);
		spring->setMaterialTexture(0, driver->getTexture("../models_obj/platform01.tga"));
		spring->setPosition(vector3df(5, -5, 0));
		spring->setRotation(vector3df(0, 0, 0));
	}

	ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(0, 0, 20), vector3df(0,0,0));
	
	ITexture *back_tex = driver->getTexture("../models_obj/background.png");
	ITexture *calc_back = ResizeTexture(back_tex, driver, driver->getScreenSize().Width, driver->getScreenSize().Height);
	
	while (device->run())
	{
		driver->beginScene(true, true, SColor(0, 200, 200, 200));
		driver->draw2DImage(calc_back, position2d < s32 >(0, 0), rect < s32 >(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height), 0, SColor(255, 255, 255, 255), true);
		smgr->drawAll();

		if (NewRec.IsKeyDown(KEY_LEFT))
		{
			hero->setPosition(hero->getPosition()+vector3df(0.1,0,0));
			hero->setRotation(vector3df(90, -20, 180));
		}

		if (NewRec.IsKeyDown(KEY_LEFT))
		{
			hero->setPosition(hero->getPosition() + vector3df(0.1, 0, 0));
			hero->setRotation(vector3df(90, -20, 180));
		}

		guienv->drawAll();
		driver->endScene();
	}
	device->drop();

	return 0;
}