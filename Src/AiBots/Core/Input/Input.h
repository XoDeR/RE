// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

namespace Rio
{
	struct Keyboard;
	struct Mouse;
	struct Touch;

	// Global input-related functions.
	namespace InputGlobalFn
	{
		void init();
		void shutdown();
		// Updates input devices.
		void update();
		Keyboard& getKeyboard();
		Mouse& getMouse();
		// Returns the default touch panel.
		Touch& getTouch();
	} // namespace InputGlobalFn

} // namespace Rio
