#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"
#include "Core/Math/Vector2.h"

#include <cstring> // memset, memcpy

namespace Rio
{

	// Maximum number of pointers supported by Touch.
	const uint32_t MAX_POINTER_IDS = 4;

	// Interface for accessing touch panel input device.
	struct Touch
	{
		Touch()
			: lastPointer(0xFF)
		{
			memset(lastTouchState, 0, MAX_POINTER_IDS);
			memset(currentTouchState, 0, MAX_POINTER_IDS);
		}

		// Returns whether the pointer is pressed in the current frame.
		bool isPointerDown(uint8_t p)
		{
			if (p >= MAX_POINTER_IDS) return false;
			return (~lastTouchState[p] & currentTouchState[p]) != 0;
		}

		// Returns whether the pointer is released in the current frame.
		bool isPointerUp(uint8_t p)
		{
			if (p >= MAX_POINTER_IDS) return false;
			return (lastTouchState[p] & ~currentTouchState[p]) != 0;
		}

		// Returns whether any pointer is pressed in the current frame.
		bool isAnyPointerDown()
		{
			return isPointerDown(lastPointer);
		}

		// Returns whether any pointer is released in the current frame.
		bool isAnyPointerUp()
		{
			return isPointerUp(lastPointer);
		}

		// Returns the position of the pointer in window space.
		// Coordinates in window space have the origin at the
		// upper-left corner of the window. +X extends from left
		// to right and +Y extends from top to bottom.
		Vector2 getPointerXy(uint8_t p)
		{
			if (p >= MAX_POINTER_IDS) return Vector2Fn::ZERO;
			return Vector2(x[p], y[p]);
		}

		void setTouchPosition(uint8_t p, uint16_t x, uint16_t y)
		{
			if (p >= MAX_POINTER_IDS) return;
			this->x[p] = x;
			this->y[p] = y;
		}

		void setWindowMetrics(uint16_t width, uint16_t height)
		{
			windowWidth = width;
			windowHeight = height;
		}

		void setPointerState(uint16_t x, uint16_t y, uint8_t p, bool state)
		{
			setTouchPosition(p, x, y);

			lastPointer = p;
			currentTouchState[p] = state;
		}

		void update()
		{
			memcpy(lastTouchState, currentTouchState, MAX_POINTER_IDS);
		}

	public:
		uint8_t lastPointer;
		uint8_t lastTouchState[MAX_POINTER_IDS];
		uint8_t currentTouchState[MAX_POINTER_IDS];

		uint16_t x[MAX_POINTER_IDS];
		uint16_t y[MAX_POINTER_IDS];

		// Window size
		uint16_t windowWidth;
		uint16_t windowHeight;
	};

} // namespace Rio
