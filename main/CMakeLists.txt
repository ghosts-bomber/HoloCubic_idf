file(GLOB_RECURSE DRIVE_SRC ${CMAKE_SOURCE_DIR} "drives/*.c")
file(GLOB_RECURSE SYSTEM_SRC ${CMAKE_SOURCE_DIR} "system/*.c" "system/*.cpp")
file(GLOB_RECURSE UI_SRC ${CMAKE_SOURCE_DIR} "ui/*.c" "system/*.cpp")
idf_component_register(SRCS 
                    "main.cpp"
                    ${DRIVE_SRC}
                    ${SYSTEM_SRC}
                    ${UI_SRC}

                    INCLUDE_DIRS 
                    "./"
                    "drives"
                    "system"
                    "ui"

                    REQUIRES

                    PRIV_REQUIRES
                    )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++23)

