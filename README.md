RifTop
======


Base code working from my work towards the oculus rift gamejam. I plan to use this as a starting point to develop an oculus compatble window manager for windows to streamline the development process so that work can be done with both the rift on or off depending on style.

Todo list in no particular order.
Standardise class field and method naming.
Convert to using IOC
Add Unit test assembly
Create window manager controlling windows window handles
Populate billboards with window textures
Local only input and window movement
Pump messages back to active window
Detect newly opened windows.
Create a fork of Ogre with the changes required to build this


In Progress
Bring system windows in as textures.

Build Steps

You will need 
Ogre 3D use the 1.9 branch from the ogre repo at the bottom. Still in testing I will look at merging once I know it's stable
Boost 1.53
Oculus Rift SDK
Directx SDK (also required for Ogre). I am just going to assume this gets installed to c:\ if it doesnt on your system you will need to change the path.
A directx11 capable machine

If you set your folder structure up as follows the relative paths should work.
Create a top level folder aka Riftop

copy folders to these relative locations. The final folder is the name of the folder to be copied.

Riftop\Oculus\LibOVR  		< Oculus SDK
Riftop\Ogre\Bin			< Ogre build from where you built ogre.
Riftop\Ogre\Lib			< Ogre build
Riftop\Ogre\Include		< Ogre build
Riftop\Ogre\OgreMain		< Ogre source from the ogre source code.
Riftop\Ogre\Components		< Ogre source
Riftop\Ogre\RenderSystems	< Ogre source
Riftop\boost			< boost 1.53 should be the folder just above include and lib
Riftop\Wolfgame			< This repo. I'll rename it eventaully it's just not high on the todo list yet.

Ogre Repository
https://bitbucket.org/anthony_martin/ogre
