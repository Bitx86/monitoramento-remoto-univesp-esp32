idf_component_register(SRCS "sistema-vacinas-esp32.c" "wifi.c" "mqtt.c" "temp.c"
                        INCLUDE_DIRS ".")

#idf_component_register(SRCS "tests/test.c"
#                        INCLUDE_DIRS ".")

# Embute o certificado como símbolo binário acessível em C
target_add_binary_data(${COMPONENT_TARGET} "cert.pem" TEXT)