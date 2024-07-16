file(GLOB_RECURSE DRIVE_SRC ${CMAKE_SOURCE_DIR} "drives/*.c")

idf_component_register(SRCS 
                    "main.cpp"
                    ${DRIVE_SRC}

                    INCLUDE_DIRS 
                    "./"
                    "drives"

                    REQUIRES

                    PRIV_REQUIRES
                    )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++23)

