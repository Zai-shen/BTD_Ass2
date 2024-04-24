idf_build_get_property(target IDF_TARGET)

if(${target} STREQUAL "linux")
   set(requires "esp_event" "esp_stubs" "protocol_examples_common" "nvs_flash")
endif()

set(srcs "main.c"
    "decode_jpeg.c"
    "decode_png.c"
    "pngle.c"
    "axp192.c"
    "st7789.c"
    "mpu6866controller.c"
    "display.c"
    "tcp_client_v4.c"
    "sgm2578.c"
    "fontx.c"
    )

set(requires "esp_event")

list(APPEND requires "protocol_examples_common" "nvs_flash" "esp_netif" "vfs" "spiffs" "driver")
list(APPEND srcs "tcp_client_v4.c")

idf_component_register(SRCS ${srcs} INCLUDE_DIRS "." REQUIRES ${requires})
