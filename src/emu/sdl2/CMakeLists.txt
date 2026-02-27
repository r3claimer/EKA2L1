add_executable(eka2l1_sdl2
    include/sdl2/emu_window_sdl2.h
    include/sdl2/gl_context_sdl2.h
    src/emu_window_sdl2.cpp
    src/gl_context_sdl2.cpp
    src/dialog_driver_sdl2.cpp
    src/main.cpp
)

target_include_directories(eka2l1_sdl2 PUBLIC include)

target_link_libraries(eka2l1_sdl2 PRIVATE
    common
    config
    drivers
    epoc
    epocservs
    epockern
    epocpkg
    gdbstub
    uvw
    j2me
    stb
    yaml-cpp
    SDL2
    glad
)

if (EKA2L1_ENABLE_SCRIPTING_ABILITY)
    set_target_properties(eka2l1_sdl2 PROPERTIES ENABLE_EXPORTS 1)

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_link_libraries(eka2l1_sdl2 PRIVATE -Wl,--whole-archive $<TARGET_FILE:scripting> -Wl,--no-whole-archive)
    else()
        target_link_libraries(eka2l1_sdl2 PRIVATE scripting)
    endif()
endif()

set_target_properties(eka2l1_sdl2 PROPERTIES OUTPUT_NAME eka2l1_sdl2
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")

add_dependencies(eka2l1_sdl2
    audiooutputrouting
    avkonfep
    ecam
    scdv
    mediaclientaudio
    mediaclientaudiostream
    mediaclientvideo
    vibractrl
    backlightctrl
    postingsurfacefactory
    goommonitor)

add_custom_command(
    TARGET eka2l1_sdl2
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:eka2l1_sdl2>/scripts"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:eka2l1_sdl2>/scripts/disabled"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:eka2l1_sdl2>/resources"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:eka2l1_sdl2>/resources/upscale"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:eka2l1_sdl2>/compat"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/miscs/panic/panic.json" "$<TARGET_FILE_DIR:eka2l1_sdl2>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/miscs/utils/leavehook.py" "$<TARGET_FILE_DIR:eka2l1_sdl2>/scripts/disabled/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/src/scripts/" "$<TARGET_FILE_DIR:eka2l1_sdl2>/scripts/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/src/emu/drivers/resources/$<IF:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},aarch64>,gles,gl>/" "$<TARGET_FILE_DIR:eka2l1_sdl2>/resources/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/src/emu/drivers/resources/upscale/" "$<TARGET_FILE_DIR:eka2l1_sdl2>/resources/upscale/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/src/emu/drivers/resources/defaultbank.hsb" "$<TARGET_FILE_DIR:eka2l1_sdl2>/resources/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/src/emu/drivers/resources/defaultbank.sf2" "$<TARGET_FILE_DIR:eka2l1_sdl2>/resources/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/miscs/compat/" "$<TARGET_FILE_DIR:eka2l1_sdl2>/compat/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/src/external/SDL_GameControllerDB/gamecontrollerdb.txt" "$<TARGET_FILE_DIR:eka2l1_sdl2>/resources/"
)

if (EKA2L1_ENABLE_SCRIPTING_ABILITY AND LUA_DLL_PATH)
    add_custom_command(
        TARGET eka2l1_sdl2
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LUA_DLL_PATH}" "$<TARGET_FILE_DIR:eka2l1_sdl2>"
    )
endif()
