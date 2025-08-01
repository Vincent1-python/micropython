set(IDF_TARGET esp32p4)

set(SDKCONFIG_DEFAULTS
    boards/sdkconfig.base
	boards/ESP32_GENERIC_P4/sdkconfig.board
)
set(MP_DL_FACE_RECOGNITION_ENABLED 1)
set(MP_DL_IMAGENET_CLS_ENABLED 1)
