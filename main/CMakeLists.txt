idf_component_register(SRCS 
                    "main.cpp"

                    INCLUDE_DIRS 
                    "./"

                    REQUIRES

                    PRIV_REQUIRES
                    )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++23)

