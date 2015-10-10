#pragma once
#include <stdint.h>
#include "bgfxdefines.h"
#include "entry/entry.h"

struct NVGcontext;

struct DemoData
{
	int fontNormal, fontBold, fontIcons;
	int images[12];
};

class App
{
public:
	void init();
	void update();
	void shutdown();

	uint32_t width = 1280;
	uint32_t height = 720;

	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_VSYNC;

	entry::MouseState mouseState;

	uint16_t textureHandle = 0;

	int64_t timeOffset = 0;

	NVGcontext* nvg = nullptr;
	DemoData data;
private:
	int loadDemoData(struct NVGcontext* vg, struct DemoData* data);
	void freeDemoData(struct NVGcontext* vg, struct DemoData* data);
	void renderSprite(struct NVGcontext* vg, float width, float height, uint16_t textureHandle);
};