#include "Animation.h"
#include "Defines.h"

Animation::Animation(void)
{
	NEWOUT("Animation erstellt - (" << this << ")");
}

bool Animation::isAnimationComplete()
{
	return isComplete;
}

Animation::~Animation(void)
{
	DELOUT("Animation gelöscht - (" << this << ")");
}
