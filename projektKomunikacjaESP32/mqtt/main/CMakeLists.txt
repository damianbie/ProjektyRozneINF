idf_component_register(SRCS "main.cpp" "System.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES wifiConnection nvs_flash mqtt)
