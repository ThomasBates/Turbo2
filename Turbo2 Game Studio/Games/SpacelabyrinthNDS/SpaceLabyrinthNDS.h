
#pragma once

#include "ApplicationNDS.h"
#include "IImage.h"
#include "ISpaceLabyrinthFactory.h"

class SpaceLabyrinthNDS : public ISpaceLabyrinthFactory
{
private:
	ApplicationNDS *_application;

public:
	//  Constructors and Destructors
	SpaceLabyrinthNDS(ApplicationNDS *application);
	~SpaceLabyrinthNDS();

	//  ISpaceLabyrinthFactory Methods
	virtual int Initialize();
	virtual int BeginUpdate();
	virtual int EndUpdate();
	virtual int Resize(int width, int height);
	virtual int Reset();
	virtual int Finalize();

	virtual int DrawWall(double left, double top, double back, double right, double bottom, double front);

protected:
	//  Local Support Methods
	virtual void InitSubSprites();
	virtual int LoadTextures();
	virtual int LoadTexture(int id, const u8 *imageData);
	virtual IImage *LoadImage(const u8 *imageData);
};