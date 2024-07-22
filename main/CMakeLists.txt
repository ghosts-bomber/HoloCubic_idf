file(GLOB_RECURSE DRIVE_SRC ${CMAKE_SOURCE_DIR} "drives/*.c")
file(GLOB_RECURSE SYSTEM_SRC ${CMAKE_SOURCE_DIR} "system/*.c" "system/*.cpp")
file(GLOB_RECURSE UI_SRC ${CMAKE_SOURCE_DIR} "ui/*.c" "ui/*.cpp")
file(GLOB_RECURSE WEATHER_SRC ${CMAKE_SOURCE_DIR} "app/weather/*.c" "app/weather/*.cpp")
idf_component_register(SRCS 
                    "main.cpp"
                    ${DRIVE_SRC}
                    ${SYSTEM_SRC}
                    ${UI_SRC}
                    ${WEATHER_SRC}
                    

                    INCLUDE_DIRS 
                    "./"
                    "drives"
                    "system"
                    "ui"
                    "app"

                    REQUIRES

                    PRIV_REQUIRES
                    )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++23)

