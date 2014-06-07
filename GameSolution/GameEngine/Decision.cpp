#include "Decision.h"
#include "State.h"

Decision::Decision( Decision *trueNode, Decision *falseNode, std::function<bool(void*)> question )
{
	resultState = nullptr;
	this->trueNode = trueNode;
	this->falseNode = falseNode;
	this->question = question;
}

Decision::Decision( State *result )
{
	resultState = result;
	trueNode = nullptr;
	falseNode = nullptr;
	question = nullptr;
}

State* Decision::evaluate( void * context )
{
	State *result = resultState;
	if ( question != nullptr )
		result = question( context ) ? trueNode->evaluate( context ) : falseNode->evaluate( context );

	result = result->copy();

	return result;
}