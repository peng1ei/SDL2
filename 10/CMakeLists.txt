add_executable(sdl10 ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LTexture.cpp)
target_link_libraries(sdl10 SDL2 SDL2_image)