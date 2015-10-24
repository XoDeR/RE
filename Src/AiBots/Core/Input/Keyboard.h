// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Input/KeyCode.h"
#include <cstring> // mem* functions

namespace Rio
{
	// Interface for accessing keyboard input device.
	struct Keyboard
	{
		Keyboard()
			: keyModifier(0)
			, lastButton(KeyboardButton::NONE)
		{
			memset(lastKeyboardState, 0, KeyboardButton::COUNT);
			memset(currentKeyboardState, 0, KeyboardButton::COUNT);
		}

		// Returns whether the specified modifier is pressed.
		// A modifier is a special key that modifies the normal action
		// of another key when the two are pressed in combination.
		// currently three different modifier keys are supported: Shift, Ctrl and Alt.
		bool isModifierPressed(ModifierButton::Enum modifier) const
		{
			return (keyModifier & (uint8_t)modifier) == modifier;
		}

		// Returns whether the button is pressed in the current frame.
		bool isButtonPressed(KeyboardButton::Enum b) const
		{
			return (~lastKeyboardState[b] & currentKeyboardState[b]) != 0;
		}

		// Returns whether the specified button is released in the current frame.
		bool isButtonReleased(KeyboardButton::Enum b) const
		{
			return (lastKeyboardState[b] & ~currentKeyboardState[b]) != 0;
		}

		// Returns whether any button is pressed in the current frame.
		bool isAnyButtonPressed()
		{
			return isButtonPressed(lastButton);
		}

		// Returns whether any button is released in the current frame.
		bool isAnyButtonReleased()
		{
			return isButtonReleased(lastButton);
		}

		void setKeyboardButtonState(KeyboardButton::Enum b, bool state)
		{
			lastButton = b;
			currentKeyboardState[b] = state;
		}

		void update()
		{
			memcpy(lastKeyboardState, currentKeyboardState, KeyboardButton::COUNT);
		}

	public:
		uint8_t keyModifier;
		KeyboardButton::Enum lastButton;
		uint8_t lastKeyboardState[KeyboardButton::COUNT];
		uint8_t currentKeyboardState[KeyboardButton::COUNT];
	};

} // namespace Rio
