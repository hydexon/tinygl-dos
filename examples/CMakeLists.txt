include_directories(./allegro4/include)
set(ALLEGRO_LIB ${CMAKE_SOURCE_DIR}/examples/allegro4/allegro.lib)
set(ALLEGASM_LIB ${CMAKE_SOURCE_DIR}/examples/allegro4/asmlib.lib)

add_executable(Gears gears.c)
target_link_libraries(Gears TinyGL ${ALLEGASM_LIB} ${ALLEGRO_LIB})

add_executable(triangle triangle.c)
target_link_libraries(triangle TinyGL ${ALLEGASM_LIB} ${ALLEGRO_LIB})

# add_executable(TinySDGLTriangle triangle.c)
# target_link_libraries(TinySDGLTriangle TinyGL ${ALLEGRO_LIB})
