#include "library.h"



video::ITexture* ResizeTexture(ITexture* txt, IVideoDriver* drv, int newX, int newZ)
{
	IImage* back_img = drv->createImageFromData(txt->getColorFormat(), txt->getSize(), txt->lock(), false);
	IImage* back_img_trg = drv->createImage(ECF_A1R5G5B5, dimension2du(newX, newZ));
	back_img->copyToScaling(back_img_trg);
	ITexture* texture = drv->addTexture("text", back_img_trg);
	texture->grab();
	return texture;
}

bool KeyReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	
	if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
	{
		hero->setRotation(vector3df(hero->getRotation().X, 0, hero->getRotation().Z));
	}
	return false;
}

bool KeyReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

KeyReceiver::KeyReceiver(IAnimatedMeshSceneNode* node) :hero(node)
{
	for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

void set_breakable(block block, ISceneManager* smg, IVideoDriver* drv)
{
	block.node->setMesh(smg->getMesh("../models_obj/platform01.x"));
	block.node->setMaterialTexture(0, drv->getTexture("../models_obj/platform01.tga"));
	block.node->setLoopMode(false);
	block.IsBreakabel = true;
}

void set_usual(block block, ISceneManager* smg, IVideoDriver* drv)
{
	block.node->setMesh(smg->getMesh("../models_obj/platform00.x"));
	block.node->setMaterialTexture(0, drv->getTexture("../models_obj/platform00.tga"));
	block.node->setLoopMode(false);
	block.IsBreakabel = false;
}

block platform(block unit, ISceneManager* smg, IVideoDriver* drv, f32 x, f32 y)
{
	unit.node = smg->addAnimatedMeshSceneNode(smg->getMesh("../models_obj/platform00.x"));
	if (unit.node)
	{
		unit.node->setMaterialFlag(EMF_LIGHTING, false);
		unit.node->setMaterialTexture(0, drv->getTexture("../models_obj/platform00.tga"));
		unit.node->setPosition(vector3df(x, y, 0));
		unit.node->setRotation(vector3df(0, 0, 0));
		unit.node->setLoopMode(false);
	}
	unit.IsBreakabel = false;
	return unit;
}