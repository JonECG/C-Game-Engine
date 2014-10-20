#pragma once

#include "UIElement.h"

namespace std
{
	template<class _Fty>
	class function;
}

class EXPORT Button : public UIElement
{
	bool isSubscribed;
	std::function<void()> *pressEvent;

protected:
	void updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset );
	void paintElement( Graphics *g, const glm::vec2& offset ) const;
public:
	Button( const glm::vec2& position, const glm::vec2& dimensions, const char* title );
	~Button();
	void subscribeMouseClickEvent( std::function<void()> function );
};

