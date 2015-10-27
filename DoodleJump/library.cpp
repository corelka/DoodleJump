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