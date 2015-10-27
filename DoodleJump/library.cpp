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
