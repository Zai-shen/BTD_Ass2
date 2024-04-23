idf_build_get_property(target IDF_TARGET)

#if(${target} STREQUAL "linux")
#    set(requires esp_event esp_stubs protocol_examples_common nvs_flash)
#endif()

set(srcs "main.c"
    "decode_jpeg.c"
    "decode_png.c"
    "pngle.c"
    "axp192.c"
    "st7789.c"
    "mpu6866controller.c"
    )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS "." REQUIRES ${requires})
