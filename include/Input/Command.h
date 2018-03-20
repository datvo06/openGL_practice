#ifndef __COMMAND_ABSTRACT__
#define __COMMAND_ABSTRACT__
#include <Object/Actor.h>


/*! Command pattern */
class Command
{
	public:
		virtual ~Command() {}
		virtual void execute(Actor* actor) = 0;
		virtual void undo() = 0;
};
#endif
