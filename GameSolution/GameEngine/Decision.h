#pragma once

#include <functional>
#include "Engine.h"
class State;

class ENGINE_SHARED Decision
{
	Decision *trueNode, *falseNode;

	std::function<bool(void*)> question;
	//std::function<void()> question;
	State *resultState;
public:
	Decision( Decision *trueNode, Decision *falseNode, std::function<bool(void*)> question );
	Decision( State *result );

	State* evaluate( void * context );
};

