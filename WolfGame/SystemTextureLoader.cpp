#include "stdafx.h"
#include "SystemTextureLoader.h"

using namespace Ogre;

SystemTextureLoader::SystemTextureLoader(void)
	:ManualResourceLoader()
{
}


SystemTextureLoader::~SystemTextureLoader(void)
{
}

void SystemTextureLoader::prepareResource ( Resource *resource)
{
	if(D3D11Texture *tex = dynamic_cast<D3D11Texture*>(resource))
	{}
}


void SystemTextureLoader::loadResource ( Resource *resource)
{
	if(D3D11Texture *tex = dynamic_cast<D3D11Texture*>(resource))
	{
		ID3D11ShaderResourceView *texture = tex->getTexture();

		if(texture)
		{
		}

	}
}