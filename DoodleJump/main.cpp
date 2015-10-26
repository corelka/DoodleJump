
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include <iostream>
#include <windows.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

video::ITexture* txt(ITexture* txt, IVideoDriver* drv, int newX, int newZ)
{
	IImage* back_img = drv->createImageFromData(txt->getColorFormat(), txt->getSize(), txt->lock(), false);
	IImage* back_img_trg = drv->createImage(ECF_A1R5G5B5, dimension2du(newX, newZ));
	back_img->copyToScaling(back_img_trg);
	ITexture* texture = drv->addTexture("text", back_img_trg);
	texture->grab();
	return texture;
}

int main(int argc, char** argv)
{
	IrrlichtDevice *device =
		createDevice(EDT_SOFTWARE, dimension2du(800, 600), 16,
		false, false, false, 0);

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	//smgr->addSkyDomeSceneNode(driver->getTexture("../models_obj/background.png"));
	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", rect<int>(10, 10, 200, 22), true);//pattern for text!
	//smgr->addSkyDomeSceneNode(driver->getTexture("../models_obj/background.png"), 100, 100, 1.0f, 1.0f);
	
	IAnimatedMesh* hero_mesh = smgr->getMesh("../models_obj/knight.obj");
	IAnimatedMeshSceneNode* hero = smgr->addAnimatedMeshSceneNode(hero_mesh);
	if (hero)
	{
		hero->setMaterialFlag(EMF_LIGHTING, false);
		hero->setFrameLoop(0, 310);
		hero->setMaterialTexture(0, driver->getTexture("../models_obj/knight.tga"));
		hero->setPosition(vector3df(0, 0, 0));
		hero->setRotation(vector3df(0, 90, 0));
	}
	
	ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(10, 1, 9), vector3df(0,1,9));
	
	ITexture *back_tex = driver->getTexture("../models_obj/background.png");
	ITexture *calc_back = txt(back_tex, driver, driver->getScreenSize().Width, driver->getScreenSize().Height);
	
	
	
	while (device->run())
	{
		driver->beginScene(true, true, SColor(0, 200, 200, 200));
		driver->draw2DImage(calc_back, position2d < s32 >(0, 0), rect < s32 >(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height), 0, SColor(255, 255, 255, 255), true);
		smgr->drawAll();
		guienv->drawAll();
		//camera->setTarget(camera->getTarget() + vector3df(0, 0, 0));
		driver->endScene();
	}
	device->drop();

	return 0;
}

