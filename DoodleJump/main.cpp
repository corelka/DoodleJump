
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

int main(int argc, char** argv)
{
	IrrlichtDevice *device =
		createDevice(EDT_SOFTWARE, dimension2du(640, 480), 16,
		false, false, false, 0);

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	
	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<int>(10, 10, 200, 22), true);//pattern for text!
	
	
	IAnimatedMesh* mesh = smgr->getMesh("../models_obj/knight.obj");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setFrameLoop(0, 310);
		node->setMaterialTexture(0, driver->getTexture("../models_obj/knight.tga"));
	}
	node->setPosition(vector3df(0, 0, 0));
	ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(5, 1, 9), vector3df(0,1,9));
	node->setRotation(vector3df(0, 90, 0));

	while (device->run())
	{
		driver->beginScene(true, true, SColor(0, 200, 200, 200));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
	device->drop();

	return 0;
}

