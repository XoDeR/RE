#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Math/Vector2.h"

#include <cstring> // memset, memcpy

namespace Rio
{
	// Enumerates mouse buttons.
	struct MouseButton
	{
		enum Enum
		{
			NONE,
			LEFT,
			MIDDLE,
			RIGHT,
			COUNT
		};
	};

	// Mouse input device.
	struct Mouse
	{
		Mouse()
			: lastButton(MouseButton::NONE)
			, mouseWheelState(0.0f)
		{
			memset(lastMouseState, 0, MouseButton::COUNT);
			memset(currentMouseState, 0, MouseButton::COUNT);
		}

		// Returns whether the b button is pressed in the current frame.
		bool isMouseButtonPressed(MouseButton::Enum b)
		{
			return (~lastMouseState[b] & currentMouseState[b]) != 0;
		}

		// Returns whether the b button is released in the current frame.
		bool isMouseButtonReleased(MouseButton::Enum b)
		{
			return (lastMouseState[b] & ~currentMouseState[b]) != 0;
		}

		// Returns whether any button is pressed in the current frame.
		bool isAnyMouseButtonPressed()
		{
			return isMouseButtonPressed(lastButton);
		}

		// Returns whether any button is released in the current frame.
		bool isAnyMouseButtonReleased()
		{
			return isMouseButtonReleased(lastButton);
		}

		// Returns the position of the cursor in window space.
		// Coordinates in window space have the origin at the
		// upper-left corner of the window. +X extends from left
		// to right and +Y extends from top to bottom.
		Vector2 getCursorXy()
		{
			return Vector2(x, y);
		}

		// Sets the position of the cursor in window space.
		// Coordinates in window space have the origin at the
		// upper-left corner of the window. +X extends from left
		// to right and +Y extends from top to bottom.
		void setCursorXy(const Vector2& position)
		{
			x = (uint16_t)position.x;
			y = (uint16_t)position.y;
		}

		// Returns the relative position of the cursor in window space.
		// Coordinates in window space have the origin at the
		// upper-left corner of the window. +X extends from left
		// to right and +Y extends from top to bottom.
		// Relative coordinates are mapped to a float varying
		// from 0.0 to 1.0 where 0.0 is the origin and 1.0 the
		// maximum extent of the considered axis.
		Vector2 getCursorRelativeXy()
		{
			return Vector2((float)x / windowWidth, (float)y / windowHeight);
		}

		// Sets the relative position of the cursor in window space.
		// Coordinates in window space have the origin at the
		// upper-left corner of the window. +X extends from left
		// to right and +Y extends from top to bottom.
		// Relative coordinates are mapped to a float varying
		// from 0.0 to 1.0 where 0.0 is the origin and 1.0 the
		// maximum extent of the considered axis.
		void setCursorRelativeXy(const Vector2& position)
		{
			setCursorXy(Vector2(position.x * (float)windowWidth, position.y * (float)windowHeight));
		}

		// Returns the mouse wheel state in the current frame.
		// A positive or negative value is returned when the wheel is up or down
		// respectively, 0.0 otherwise.
		float getMouseWheelState()
		{
			return mouseWheelState;
		}

		void setMousePosition(uint16_t x, uint16_t y)
		{
			this->x = x;
			this->y = y;
		}

		void setWindowMetrics(uint16_t width, uint16_t height)
		{
			windowWidth = width;
			windowHeight = height;
		}

		void setMouseButtonState(uint16_t x, uint16_t y, MouseButton::Enum b, bool state)
		{
			setMousePosition(x, y);
			lastButton = b;
			currentMouseState[b] = state;
		}

		void setMouseWheelState(uint16_t x, uint16_t y, float wheel)
		{
			setMousePosition(x, y);
			mouseWheelState = wheel;
		}

		void update()
		{
			mouseWheelState = 0.0f;
			memcpy(lastMouseState, currentMouseState, MouseButton::COUNT);
		}
	public:
		MouseButton::Enum lastButton;
		float mouseWheelState;
		uint8_t lastMouseState[MouseButton::COUNT];
		uint8_t currentMouseState[MouseButton::COUNT];

		// Position within the window
		uint16_t x;
		uint16_t y;

		// Window size
		uint16_t windowWidth;
		uint16_t windowHeight;
	};

} // namespace Rio

