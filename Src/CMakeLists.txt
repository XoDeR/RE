#
# project: Src
#

if (NOT FIPS_IMPORT)
    cmake_minimum_required(VERSION 2.8)
    get_filename_component(FIPS_ROOT_DIR "Third/fips" ABSOLUTE)
    include("${FIPS_ROOT_DIR}/cmake/fips.cmake")
    fips_setup()
    fips_project(Src)
endif()

#-------------------------------------------------------------------------------
# it looks like bgfx can auto-detect all C preprocessor defines,
# for those that are tweakable, cmake options should be defined here
#

include_directories(Third/bx/include Third/bgfx/include)
if (FIPS_MACOS)
    include_directories(Third/bx/include/compat/osx)
elseif (FIPS_IOS)
    include_directories(Third/bx/include/compat/ios)
elseif (FIPS_PNACL)
    include_directories(Third/bx/include/compat/nacl)
elseif (FIPS_WINDOWS)
    include_directories(Third/bx/include/compat/msvc)
endif()

#-[ Tools ]---------------------------------------------------------------------

# Build tools only if not cross-compiling
if (NOT CMAKE_TOOLCHAIN_FILE)

fips_ide_group(Tools)
fips_begin_app(shaderc cmdline)
    fips_include_directories(Third/bgfx/3rdparty/glsl-optimizer/src/glsl)
    fips_dir(Third/bgfx/tools/shaderc GROUP "shaderc")
    fips_files(shaderc.cpp shaderc.h shaderc_glsl.cpp shaderc_hlsl.cpp)
    fips_dir(Third/bgfx/src GROUP "bgfx")
    fips_files(vertexdecl.cpp vertexdecl.h)
    fips_deps(bgfx-fcpp bgfx-glsl-optimizer)
    if (FIPS_WINDOWS)
        add_definitions(-D__STDC__ -D__STDC_VERSION__=199901L -Dstrdup=_strdup -Dalloca=_alloca -Disascii=__isascii)
        fips_include_directories(Third/bgfx/3rdparty/glsl-optimizer/include/c99)
        include_directories($ENV{DXSDK_DIR}include)
        set(DXSDK_LIB "$ENV{DXSDK_DIR}Lib\\${FIPS_WINDOWS_PLATFORM_NAME}\\")
        fips_libs(d3dx9 d3dcompiler dxguid)
    endif()
fips_end_app()

fips_begin_app(texturec cmdline)
    fips_include_directories(Third/bx/include)
    fips_include_directories(Third/bgfx/include)
    fips_include_directories(Third/bgfx/src)
    fips_dir(Third/bgfx/tools/texturec GROUP "texturec")
    fips_files(texturec.cpp)
    fips_dir(Third/bgfx/src GROUP "bgfx")
    fips_files(image.cpp image.h)
fips_end_app()

fips_begin_app(geometryc cmdline)
    fips_include_directories(Third/bx/include)
    fips_include_directories(Third/bgfx/include)
    fips_include_directories(Third/bgfx/3rdparty)
    fips_include_directories(AiBots/App/Os)
    fips_dir(Third/bgfx/tools/geometryc GROUP "geometryc")
    fips_files(geometryc.cpp)
    fips_dir(Third/bgfx/src GROUP "bgfx")
    fips_files(vertexdecl.cpp vertexdecl.h)
    fips_dir(Third/bgfx/3rdparty/forsyth-too GROUP "forsyth-too")
    fips_files(forsythtriangleorderoptimizer.cpp forsythtriangleorderoptimizer.h)
    fips_dir(AiBots/App/Os GROUP "bounds")
    fips_files(bounds.cpp bounds.h)
    fips_deps(bgfx-3rdparty)
fips_end_app()

fips_begin_app(makedisttex cmdline)
    if (FIPS_WINDOWS)
        add_definitions(-D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)
    endif()
    fips_include_directories(Third/bx/include)
    fips_include_directories(Third/bgfx/3rdparty)
    fips_dir(Third/bgfx/tools GROUP "makedisttex")
    fips_files(makedisttex.cpp)
    fips_dir(Third/bgfx/3rdparty/edtaa3 GROUP "edtaa3")
    fips_files(edtaa3func.cpp edtaa3func.h)
fips_end_app()

if (FIPS_CLANG)
    set_target_properties(shaderc PROPERTIES COMPILE_FLAGS "-Wno-strict-aliasing")
    set_target_properties(makedisttex PROPERTIES COMPILE_FLAGS "-Wno-missing-field-initializers")
    set_target_properties(texturec PROPERTIES COMPILE_FLAGS "-Wno-unused-variable")
elseif(FIPS_GCC)
    set_target_properties(shaderc PROPERTIES COMPILE_FLAGS "-Wno-strict-aliasing")
    set_target_properties(makedisttex PROPERTIES COMPILE_FLAGS "-Wno-missing-field-initializers")
    set_target_properties(texturec PROPERTIES COMPILE_FLAGS "-Wno-unused-variable")
elseif(FIPS_MSVC)
    set_target_properties(shaderc PROPERTIES LINK_FLAGS "/LIBPATH:\"${DXSDK_LIB}\\\"")
    set_target_properties(shaderc PROPERTIES COMPILE_FLAGS "/wd4152 /wd4204 /wd4242 /wd4244 /wd4668 /wd4996 /wd4291 /WX-")
endif()

fips_begin_lib(bgfx-fcpp)
    add_definitions(-DNINCLUDE=64 -DNWORK=65536 -DNBUF=65536 -DOLD_PREPROCESSOR=0)
    include_directories(Third/bgfx/3rdparty/fcpp)
    fips_dir(Third/bgfx/3rdparty/fcpp)
    fips_files(cpp1.c cpp2.c cpp3.c cpp4.c cpp5.c cpp6.c)
fips_end_lib()

fips_begin_lib(bgfx-glsl-optimizer)
    include_directories(Third/bgfx/3rdparty/glsl-optimizer/include)
    include_directories(Third/bgfx/3rdparty/glsl-optimizer/src/mesa)
    include_directories(Third/bgfx/3rdparty/glsl-optimizer/src/mapi)
    include_directories(Third/bgfx/3rdparty/glsl-optimizer/src/glsl)
    include_directories(Third/bgfx/3rdparty/glsl-optimizer/src)
    file(GLOB glsl_sources
        Third/bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/*.c
        Third/bgfx/3rdparty/glsl-optimizer/src/glsl/*.cpp
        Third/bgfx/3rdparty/glsl-optimizer/src/glsl/*.c
        Third/bgfx/3rdparty/glsl-optimizer/src/mesa/*.c
        Third/bgfx/3rdparty/glsl-optimizer/src/mesa/main/*.c
        Third/bgfx/3rdparty/glsl-optimizer/src/mesa/program/*.c
        Third/bgfx/3rdparty/glsl-optimizer/src/util/*.c
    )
    file(GLOB glsl_sources_remove
        Third/bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/glcpp.cpp
        Third/bgfx/3rdparty/glsl-optimizer/src/glsl/ir_set_program_inouts.cpp
        Third/bgfx/3rdparty/glsl-optimizer/src/glsl/main.cpp
        Third/bgfx/3rdparty/glsl-optimizer/src/glsl/builtin_stubs.cpp
    )
    list(REMOVE_ITEM glsl_sources ${glsl_sources_remove})
    fips_files(${glsl_sources})
fips_end_lib()

if (FIPS_MSVC)
    set_target_properties(bgfx-glsl-optimizer PROPERTIES COMPILE_FLAGS "/wd4291 /WX-")
endif()

endif() # NOT CMAKE_TOOLCHAIN_FILE

#-------------------------------------------------------------------------------
fips_ide_group("") # remove grouping

fips_begin_lib(bgfx)
    if (NOT FIPS_PNACL)
        include_directories(Third/bgfx/3rdparty/khronos)
    endif()
    include_directories(Third/bgfx/3rdparty)
    fips_dir(Third/bgfx/src GROUP "src")
    # note: add all files to the build process, bgfx knows
    # itself what to compile on each platform
    fips_files(
        bgfx.cpp bgfx_p.h
        charset.h config.h
        glimports.h
        image.cpp image.h
        ovr.cpp ovr.h
        renderdoc.cpp renderdoc.h
        renderer.h
        renderer_null.cpp
        vertexdecl.cpp vertexdecl.h
        varying.def.sc

        # hmm looks like these files must be included
        # on all platforms, otherwise linker errors
        # from Bgfx::s_rendererCreator result:
        renderer_gl.cpp renderer_gl.h
        renderer_d3d11.cpp renderer_d3d11.h
        renderer_d3d12.cpp
        renderer_d3d9.cpp renderer_d3d9.h
        renderer_vk.cpp
    )

    bgfx_shaders(FILES
        fs_clear0.sc
        fs_clear1.sc
        fs_clear2.sc
        fs_clear3.sc
        fs_clear4.sc
        fs_clear5.sc
        fs_clear6.sc
        fs_clear7.sc
        fs_debugfont.sc
        vs_clear.sc
        vs_debugfont.sc
    )

    if (FIPS_MACOS)
        fips_files(glcontext_nsgl.mm glcontext_nsgl.h)
    elseif (FIPS_IOS)
        fips_files(glcontext_eagl.mm glcontext_eagl.h)
    elseif (FIPS_LINUX)
        fips_files(glcontext_glx.cpp glcontext_glx.h)
    elseif (FIPS_PNACL)
        fips_files(glcontext_ppapi.cpp glcontext_ppapi.h)
    elseif (FIPS_WINDOWS)
        fips_files(glcontext_wgl.cpp glcontext_wgl.h)
    else()
        fips_files(glcontext_egl.cpp glcontext_egl.h)
    endif()
    fips_dir(Third/bgfx/include GROUP "include")
    fips_files(
        bgfx.c99.h
        bgfx.h
        bgfxdefines.h
        bgfxplatform.c99.h
        bgfxplatform.h
    )

    # untested, note: OSX and iOS currently
    # automatically link against GL
    if (FIPS_WINDOWS)
        fips_libs(opengl32)
    elseif (FIPS_LINUX)
        fips_libs(X11 Xrandr Xi Xinerama Xxf86vm Xcursor GL dl)
    elseif (FIPS_ANDROID)
        # FIXME!
    endif()
fips_end_lib()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set_target_properties(bgfx PROPERTIES COMPILE_DEFINITIONS "BGFX_CONFIG_DEBUG=1")
endif()

# Temporarily disable warning caused by some BX_CHECK use.
if (FIPS_CLANG OR FIPS_GCC)
    set_target_properties(bgfx PROPERTIES COMPILE_FLAGS "-Wno-unused-variable")
endif()

#-------------------------------------------------------------------------------
fips_begin_lib(app-os)
    fips_dir(AiBots/App/Os GROUP ".")
    fips_files(
        aviwriter.h
        bgfx_utils.cpp bgfx_utils.h
        bounds.cpp bounds.h
        camera.cpp camera.h
        common.h
        cube_atlas.cpp cube_atlas.h
    )
    fips_dir(AiBots/App/Os/imgui GROUP "imgui")
    bgfx_shaders(FILES
        fs_imgui_color.sc   vs_imgui_color.sc
        fs_imgui_cubemap.sc vs_imgui_cubemap.sc
        fs_imgui_image.sc   vs_imgui_image.sc
        fs_imgui_texture.sc vs_imgui_texture.sc
        fs_ocornut_imgui.sc vs_ocornut_imgui.sc
        fs_imgui_image_swizz.sc
    )
    fips_files(
        droidsans.ttf.h
        imgui.cpp imgui.h
        ocornut_imgui.cpp ocornut_imgui.h
        varying.def.sc
    )
    fips_dir(AiBots/App/Os/nanovg GROUP "nanovg")
    bgfx_shaders(FILES fs_nanovg_fill.sc vs_nanovg_fill.sc)
    fips_files(
        fontstash.h
        nanovg.cpp nanovg.h
        nanovg_bgfx.cpp
        varying.def.sc
    )
    fips_dir(AiBots/App/Os/font GROUP "font")
    bgfx_shaders(FILES
        fs_font_basic.sc
        vs_font_basic.sc
        fs_font_distance_field.sc
        vs_font_distance_field.sc
        fs_font_distance_field_subpixel.sc
        vs_font_distance_field_subpixel.sc
    )
    fips_files(
        font_manager.cpp font_manager.h
        text_buffer_manager.cpp text_buffer_manager.h
        text_metrics.cpp text_metrics.h
        utf8.cpp utf8.h varying.def.sc
    )
    fips_dir(AiBots/App/Os/entry GROUP "entry")
    fips_files(
        cmd.cpp cmd.h
        dbg.cpp dbg.h
        entry.cpp entry.h entry_p.h
        input.cpp input.h
    )
    if (FIPS_ANDROID)
        fips_files(entry_android.cpp)
    elseif (FIPS_EMSCRIPTEN)
        fips_files(entry_asmjs.cpp)
    elseif (FIPS_PNACL)
        fips_files(entry_nacl.cpp)
    elseif (FIPS_WINDOWS)
        fips_files(entry_windows.cpp)
    elseif (FIPS_LINUX)
        fips_files(entry_x11.cpp)
    elseif (FIPS_MACOS)
        fips_files(entry_osx.mm)
    elseif (FIPS_IOS)
        fips_files(entry_ios.mm)
    else()
        fips_files(entry_qnx.cpp entry_sdl.cpp entry_winrt.cpp)
    endif()
fips_end_lib()

#-[ 3rdparty ]------------------------------------------------------------------
fips_ide_group(3rdparty)

fips_begin_lib(bgfx-3rdparty)
    fips_dir(Third/bgfx/3rdparty/ib-compress GROUP "ib-compress")
    fips_files(
        indexbuffercompression.cpp indexbuffercompression.h
        indexbuffercompressionformat.h
        indexbufferdecompression.cpp indexbufferdecompression.h
        indexcompressionconstants.h
        readbitstream.h
        writebitstream.h
    )
    fips_dir(Third/bgfx/3rdparty/ocornut-imgui GROUP "ocornut-imgui")
    fips_files(
        imgui.cpp imgui.h imgui_demo.cpp imgui_draw.cpp imconfig.h imgui_internal.h
        memory_editor.h stb_rect_pack.h stb_textedit.h stb_truetype.h
    )
fips_end_lib()

file(GLOB_RECURSE BX_FILES Third/bx/include/bx/*.h Third/bx/include/compat/*.h Third/bx/include/tinystl/*.h)
fips_files(${BX_FILES})
source_group("bx" FILES ${BX_FILES})

#-[ Game App ]-------------------------------------------------------------------
if (NOT FIPS_IMPORT)

fips_ide_group(AiBots)
fips_include_directories(AiBots/App/Os)
fips_include_directories(AiBots)

fips_begin_app(AiBots cmdline)
	fips_dir(AiBots/App GROUP "App")
    fips_files(
		App.cpp
		App.h
		blendish.h
		nanovg.cpp
	)
	fips_dir(AiBots/Core/Base GROUP "Core/Base")
	fips_files(
		Config.h
		Platform.h
		Types.h
		Murmur.h
		Murmur.cpp
	)
	fips_dir(AiBots/Core/Os GROUP "Core/Os")
	if (FIPS_MACOS OR FIPS_IOS OR FIPS_LINUX OR FIPS_ANDROID)
    elseif (FIPS_WINDOWS)
        fips_files(
			Windows/Headers_Windows.h
		)
	endif()
	fips_dir(AiBots/Core/Debug GROUP "Core/Debug")
	if (FIPS_MACOS OR FIPS_IOS OR FIPS_LINUX OR FIPS_ANDROID)
    elseif (FIPS_WINDOWS)
        fips_files(
			Error.cpp
			Error.h
			StackTrace.h
			Windows/StackTrace_Windows.cpp
		)
	endif()
	fips_dir(AiBots/Core/Thread GROUP "Core/Thread")
	if (FIPS_MACOS OR FIPS_IOS OR FIPS_LINUX OR FIPS_ANDROID)
        fips_files(
			AtomicInt.h
			Mutex.h
			ScopedMutex.h
			Semaphore.h
			Thread.h
			Posix/AtomicInt_Posix.cpp
			Posix/Mutex_Posix.cpp
			Posix/Semaphore_Posix.cpp
			Posix/Thread_Posix.cpp
		)
    elseif (FIPS_WINDOWS)
        fips_files(
			AtomicInt.h
			Mutex.h
			ScopedMutex.h
			Semaphore.h
			Thread.h
			Windows/AtomicInt_Windows.cpp
			Windows/Mutex_Windows.cpp
			Windows/Semaphore_Windows.cpp
			Windows/Thread_Windows.cpp
		)
	endif()
	fips_dir(AiBots/Core/Memory GROUP "Core/Memory")
	fips_files(
		Allocator.h
		HeapAllocator.cpp
		HeapAllocator.h
		LinearAllocator.cpp
		LinearAllocator.h
		Memory.cpp
		Memory.h
		MemoryAux.h
		PoolAllocator.cpp
		PoolAllocator.h
		ProxyAllocator.cpp
		ProxyAllocator.h
		ScratchAllocator.cpp
		ScratchAllocator.h
		StackAllocator.cpp
		StackAllocator.h
		TempAllocator.h
	)
	fips_dir(AiBots/Core/Containers GROUP "Core/Containers")
	fips_files(
		Array.h
		Hash.h
		Queue.h
	)
	fips_dir(AiBots/Core/Strings GROUP "Core/Strings")
	fips_files(
		StringId32.h
		StringId32.cpp
		StringId64.h
		StringId64.cpp
		StringUtils.h
	)
	
    fips_deps(bgfx bgfx-3rdparty app-os)
fips_end_app()

endif() # NOT FIPS_IMPORT

if (NOT FIPS_IMPORT)
    fips_finish()
endif()

