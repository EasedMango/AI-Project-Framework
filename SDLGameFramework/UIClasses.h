#pragma once
#include <functional>
#include <string>

#include "Renderer.h"
#include "Transform.h"

struct UIElement
{
	bool active= true;
};

struct Button
{
	std::vector< std::function<void()>> callback;

	Button() : callback()
	{
		
	};

	Button( std::function<void()> functions)
	{
		(callback.push_back(functions));
	}

	void AddCallback(const std::function<void()>& callback_)
	{
		this->callback.push_back(callback_);
	}

	void OnClick() const
	{
		for (auto& callback_ : this->callback)
		{
			callback_();
		}
	}
};
