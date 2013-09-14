#pragma once
#include "stdafx.h"

using namespace Ogre;

class SystemTextureLoader : public ManualResourceLoader
{
public:
	SystemTextureLoader(void);
	~SystemTextureLoader(void);

	void prepareResource(Resource *resource);

	void loadResource(Resource *resource);

};

