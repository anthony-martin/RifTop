#pragma once
#include "stdafx.h"

using namespace Ogre;

class SystemTextureLoader : public ManualResourceLoader
{
private:
	HWND m_WindowHandle;
public:
	SystemTextureLoader(HWND window);
	~SystemTextureLoader(void);

	void prepareResource(Resource *resource);

	void loadResource(Resource *resource);

};

