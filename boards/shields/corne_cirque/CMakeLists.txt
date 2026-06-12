if(CONFIG_ZMK_SPLIT_ROLE_PERIPHERAL AND CONFIG_ZMK_POINTING)
  zephyr_library_sources(cirque_startup_delay.c)
endif()
