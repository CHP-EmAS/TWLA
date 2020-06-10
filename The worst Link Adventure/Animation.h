#pragma once

#include <string>
#include <list>

class Animation
{
public:
	Animation(void);

	virtual void animate() = 0;

	bool isAnimationComplete();

	virtual ~Animation(void);
protected:
	bool isComplete;
};

