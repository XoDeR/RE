#-------------------------------------------------------------------------------
#   fips_osx.cmake
#   OSX/IOS specific cmake functions.
#   FIXME: OSX framework resolution doesn't seem to work
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#   fips_frameworks_osx(frameworks ...)
#   OSX specific: Add one or more OSX frameworks for linking with the
#   current target.
#
macro(fips_frameworks_osx frameworks)
    foreach (fw ${ARGV})
        list(APPEND CurFrameworks ${fw})
    endforeach()
endmacro()

#-------------------------------------------------------------------------------
#   fips_osx_recurse_frameworks(input output)
#
macro(fips_osx_recurse_frameworks target output)
    get_property(fws GLOBAL PROPERTY ${target}_frameworks)
    foreach(fw ${fws})
        list(APPEND ${output} ${fws})
    endforeach()
    get_property(deps GLOBAL PROPERTY ${target}_deps)
    foreach(dep ${deps})
        fips_osx_recurse_frameworks(${dep} ${output})
    endforeach()
endmacro()

#-------------------------------------------------------------------------------
#   fips_osx_resolve_frameworks(target)
#   Recursively resolve dependencies of a target.
#
macro(fips_osx_resolve_frameworks target)
    set(resolvedFws)
    fips_osx_recurse_frameworks(${target} resolvedFws)
    if (resolvedFws)
        list(REMOVE_DUPLICATES resolvedFws)
    endif()
    if (FIPS_CMAKE_VERBOSE)
        message("${target} Frameworks: ${resolvedFws}")
    endif()
    foreach (fw ${resolvedFws})
        unset(found_framework CACHE)
        find_library(found_framework ${fw})
        target_link_libraries(${target} ${found_framework})
        unset(found_framework CACHE)
    endforeach()
endmacro()

#-------------------------------------------------------------------------------
#   fips_copy_osx_dylib_files(target isbundle)
#   Copy OSX dynamic link libs to the executables directory.
#
macro(fips_copy_osx_dylib_files target isbundle)
    if (FIPS_IOS OR FIPS_OSX)
        if (${isbundle})
            set(XCODE_OUTPUT_DIR \${TARGET_BUILD_DIR}/\${FULL_PRODUCT_NAME}/Contents/MacOS)
        else()
            set(XCODE_OUTPUT_DIR \${TARGET_BUILD_DIR})
        endif()
        foreach (dylib ${CurDylibFiles})
            set(fullDylibName lib${dylib}.dylib)
            if (FIPS_CMAKE_VERBOSE)
                message("Add post build step to copy dylib: ${FIPS_PROJECT_DIR}/lib/osx/${fullDylibName}")
            endif()
            add_custom_command(TARGET ${target} POST_BUILD COMMAND pwd\; cp ${FIPS_PROJECT_DIR}/lib/osx/${fullDylibName} ${XCODE_OUTPUT_DIR})
        endforeach()
    endif()
endmacro()

#-------------------------------------------------------------------------------
#   fips_osx_generate_plist_file(target)
#
macro(fips_osx_generate_plist_file target)
    set(FIPS_OSX_PLIST_PATH ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
    set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${FIPS_OSX_PLIST_PATH})
    if (FIPS_IOS)
        file(WRITE ${FIPS_OSX_PLIST_PATH}
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\">\n"
            "  <dict>\n"
            "    <key>CFBundleDevelopmentRegion</key>\n"
            "    <string>English</string>\n"
            "    <key>CFBundleDisplayName</key>\n"
            "    <string>\${MACOSX_BUNDLE_BUNDLE_NAME}</string>\n"
            "    <key>CFBundleExecutable</key>\n"
            "    <string>\${MACOSX_BUNDLE_EXECUTABLE_NAME}</string>\n"
            "    <key>CFBundleIconFile</key>\n"
            "    <string>\${MACOSX_BUNDLE_ICON_FILE}</string>\n"
            "    <key>CFBundleIconFiles</key>\n"
            "    <array>\n"
            "      <string>icon.png</string>\n"
            "      <string>icon-57.png</string>\n"
            "      <string>icon-114.png</string>\n"
            "    </array>\n"
            "    <key>CFBundleIdentifier</key>\n"
            "    <string>\${MACOSX_BUNDLE_GUI_IDENTIFIER}</string>\n"
            "    <key>CFBundleInfoDictionaryVersion</key>\n"
            "    <string>6.0</string>\n"
            "    <key>CFBundleName</key>\n"
            "    <string>\${MACOSX_BUNDLE_BUNDLE_NAME}</string>\n"
            "    <key>CFBundlePackageType</key>\n"
            "    <string>APPL</string>\n"
            "    <key>CFBundleSignature</key>\n"
            "    <string>????</string>\n"
            "    <key>CFBundleVersion</key>\n"
            "    <string>1.0</string>\n"
            "    <key>LSRequiresIPhoneOS</key>\n"
            "    <true/>\n"
            "    <key>UIStatusBarHidden</key>\n"
            "    <true/>\n"
            "    <key>UIViewControllerBasedStatusBarAppearance</key>\n"
            "    <false/>\n"
            "    <key>UIInterfaceOrientation</key>\n"
            "    <string>UIInterfaceOrientationLandscapeRight</string>\n"
            "    <key>UISupportedInterfaceOrientations</key>\n"
            "    <array>\n"
            "      <string>UIInterfaceOrientationLandscapeRight</string>\n"
            "    </array>\n"
            "    <key>UIRequiredDeviceCapabilities</key>\n"
            "    <array>\n"
            "      <string>opengles-2</string>\n"
            "      <string>armv7</string>\n"
            "      <string>accelerometer</string>\n"
            "    </array>\n"
            "  </dict>\n"
            "</plist>\n")
    elseif (FIPS_MACOS)
        file(WRITE ${FIPS_OSX_PLIST_PATH}
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\">\n"
            "  <dict>\n"
            "    <key>CFBundleDevelopmentRegion</key>\n"
            "    <string>English</string>\n"
            "    <key>CFBundleDisplayName</key>\n"
            "    <string>\${MACOSX_BUNDLE_BUNDLE_NAME}</string>\n"
            "    <key>CFBundleExecutable</key>\n"
            "    <string>\${MACOSX_BUNDLE_EXECUTABLE_NAME}</string>\n"
            "    <key>CFBundleIconFile</key>\n"
            "    <string>\${MACOSX_BUNDLE_ICON_FILE}</string>\n"
            "    <key>CFBundleIconFiles</key>\n"
            "    <array>\n"
            "      <string>icon.png</string>\n"
            "      <string>icon-57.png</string>\n"
            "      <string>icon-114.png</string>\n"
            "    </array>\n"
            "    <key>CFBundleIdentifier</key>\n"
            "    <string>\${MACOSX_BUNDLE_GUI_IDENTIFIER}</string>\n"
            "    <key>CFBundleInfoDictionaryVersion</key>\n"
            "    <string>6.0</string>\n"
            "    <key>CFBundleName</key>\n"
            "    <string>\${MACOSX_BUNDLE_BUNDLE_NAME}</string>\n"
            "    <key>CFBundlePackageType</key>\n"
            "    <string>APPL</string>\n"
            "    <key>CFBundleShortVersionString</key>\n"
            "    <string>1.0</string>\n"
            "    <key>CFBundleSignature</key>\n"
            "    <string>????</string>\n"
            "    <key>CFBundleVersion</key>\n"
            "    <string>1</string>\n"
            "    <key>LSMinimumSystemVersion</key>\n"
            "    <string>\${MACOSX_DEPLOYMENT_TARGET}</string>\n"
            "    <key>NSHighResolutionCapable</key>\n"
            "    <true/>\n"
            "  </dict>\n"
            "</plist>\n")
    endif()
endmacro()

#-------------------------------------------------------------------------------
#   fips_osx_add_target_properties(target)
#   Setup setup special target properties for OSX/iOS.
#
macro(fips_osx_add_target_properties target)
    if (FIPS_IOS OR FIPS_OSX)
        fips_osx_generate_plist_file(${target})
        if (FIPS_IOS)
            set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")    
            set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
        endif()
        set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "fips.${target}")
        set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_ICON_FILE "Icon.png")
        if (${CMAKE_GENERATOR} STREQUAL "Xcode")
            set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_EXECUTABLE_NAME \${EXECUTABLE_NAME})
            set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_PRODUCT_NAME \${PRODUCT_NAME})
            set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME \${PRODUCT_NAME})
        else()
            set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_PRODUCT_NAME "${target}")
            set_target_properties(${target} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "${target}")
        endif()
    endif()
endmacro()


