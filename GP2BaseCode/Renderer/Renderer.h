#pragma once

//Interface class
class IRenderer
{
public:
	//Always has to have an empty deconstructor
	virtual ~IRenderer(){};

	//pure virtual functions (see the =0) have to be implemented
	virtual bool init(void *pWindowHandle, bool fullscreen) = 0;
	virtual void clear(float r, float g, float b, float a) = 0;
	virtual void present() = 0;

};