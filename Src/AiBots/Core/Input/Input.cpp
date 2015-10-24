// Copyright (c) 2015 Volodymyr Syvochka
#include "Input.h"

#include "Core/Input/Keyboard.h"
#include "Core/Input/Mouse.h"
#include "Core/Input/Touch.h"
#include "Core/Memory/Memory.h"

namespace Rio
{
	namespace InputGlobalFn
	{
		const size_t BUFFER_SIZE = 0 +
			+sizeof(Keyboard)
			+ sizeof(Mouse)
			+ sizeof(Touch);

		char buffer[BUFFER_SIZE];
		Keyboard* keyboard = NULL;
		Mouse* mouse = NULL;
		Touch* touch = NULL;

		void init()
		{
			keyboard = new (buffer)Keyboard();
			mouse = new (keyboard + 1) Mouse();
			touch = new (mouse + 1) Touch();
		}

		void shutdown()
		{
			keyboard->~Keyboard();
			keyboard = NULL;
			mouse->~Mouse();
			mouse = NULL;
			touch->~Touch();
			touch = NULL;
		}

		void update()
		{
			keyboard->update();
			mouse->update();
			touch->update();
		}

		Keyboard& getKeyboard()
		{
			return *keyboard;
		}

		Mouse& getMouse()
		{
			return *mouse;
		}

		Touch& getTouch()
		{
			return *touch;
		}
	} // namespace InputGlobalFn

} // namespace Rio
