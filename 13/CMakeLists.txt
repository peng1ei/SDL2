add_executable(sdl13 ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LTexture.cpp)
target_link_libraries(sdl13 SDL2 SDL2_image)