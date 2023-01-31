idf_component_register(SRCS "main.cpp" "System.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES asio WiFiConnecton tcpClient nvs_flash)
