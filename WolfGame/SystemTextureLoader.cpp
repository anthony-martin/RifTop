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
	if(D3D9Texture *tex = dynamic_cast<D3D9Texture*>(resource))
	{}
}

typedef BOOL (WINAPI *DwmGetDxSharedSurface_td) (
	__in HWND hwnd, 
	__out_opt UINT32* p1, 
	__out_opt UINT64* p2, 
	__out_opt UINT32* p3, 
	__out_opt UINT32* p4, 
	__out_opt UINT64* p5 );


DwmGetDxSharedSurface_td DwmGetSharedSurface = NULL;

HANDLE GetDWMSharedHandle(HWND hwnd)
{
    if (!DwmGetSharedSurface)
	{
        DwmGetSharedSurface = ((DwmGetDxSharedSurface_td)GetProcAddress(GetModuleHandle(TEXT("USER32")), "DwmGetDxSharedSurface"));
	}


    UINT32 hwndInt = (UINT32)hwnd;
    UINT64 p2 = -1,p5 = -1;
    UINT32 p1 = -1,p3 = -1,p4 = -1;

    DwmGetSharedSurface(hwnd,&p1,&p2,&p3,&p4,&p5);

    return (HANDLE)p1;
}


void SystemTextureLoader::loadResource ( Resource *resource)
{
	if(D3D11Texture *tex = dynamic_cast<D3D11Texture*>(resource))
	{
		//comment this out if building with standard ogre
		HANDLE texHandle = GetDWMSharedHandle( (HWND)(0x000F0516));
		tex->LoadSharedResource(texHandle);
	}
}