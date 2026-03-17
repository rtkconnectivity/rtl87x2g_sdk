
##################################################
# matter-port
##################################################

if(DEFINED ENABLE_PW_RPC AND "${ENABLE_PW_RPC}" STREQUAL "ON")
    target_compile_definitions(openthread-rtl87x2g PUBLIC "ENABLE_PW_RPC=1")
else()
    target_compile_definitions(openthread-rtl87x2g PUBLIC "ENABLE_PW_RPC=0")
endif()

if(matter_enable_cfu)
    target_compile_definitions(openthread-rtl87x2g PUBLIC "MATTER_ENABLE_CFU=1" "FEATURE_SUPPORT_CFU=1")
else()
    target_compile_definitions(openthread-rtl87x2g PUBLIC "MATTER_ENABLE_CFU=0")
endif()

target_sources(openthread-rtl87x2g
    PRIVATE
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/kvmgr.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_uart.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_kvs.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_led.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_utils.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_wdt.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/factory_data/factory.pb.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/factory_data/pb_common.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/factory_data/pb_decode.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/factory_data/pb_encode.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_ble/matter_ble.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_ble/matter_ble_service.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_ble/matter_ble_task.c
    ${REALTEK_SDK_ROOT}/subsys/matter/vendor/rtl87x2g/matter_gpio.c
)

if(matter_enable_ota_requestor)
    target_include_directories(openthread-rtl87x2g
        PRIVATE
            ${REALTEK_SDK_ROOT}/subsys/dfu
    )

    target_sources(openthread-rtl87x2g
        PRIVATE
        ${REALTEK_SDK_ROOT}/subsys/dfu/dfu_common.c
        ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_ota.c
    )
endif()

if(matter_enable_ble_ota)
    target_compile_definitions(openthread-rtl87x2g PUBLIC
        SUPPORT_BLE_OTA=1
        SUPPORT_NORMAL_OTA=0
    )

    target_include_directories(openthread-rtl87x2g
        PRIVATE
            ${REALTEK_SDK_ROOT}/subsys/dfu
    )

    target_sources(openthread-rtl87x2g
        PRIVATE
        ${REALTEK_SDK_ROOT}/subsys/dfu/ble_dfu_transport.c
        ${REALTEK_SDK_ROOT}/subsys/dfu/dfu_app.c
        ${REALTEK_SDK_ROOT}/subsys/dfu/dfu_common.c
        ${REALTEK_SDK_ROOT}/subsys/dfu/dfu_service.c
        ${REALTEK_SDK_ROOT}/subsys/dfu/ota_service.c
    )
endif()

if(matter_factory_data_encryption)
    target_compile_definitions(openthread-rtl87x2g PUBLIC "CONFIG_FACTORY_DATA_ENCRYPTION=1")
else()
    target_compile_definitions(openthread-rtl87x2g PUBLIC "CONFIG_FACTORY_DATA_ENCRYPTION=0")
endif()

if(matter_dac_key_encryption)
    target_compile_definitions(openthread-rtl87x2g PUBLIC "CONFIG_DAC_KEY_ENC=1")
endif()

if(matter_enable_ram_reduce)
    target_compile_definitions(openthread-rtl87x2g PUBLIC "CONFIG_ENABLE_RAM_REDUCE=1")
endif()

target_link_options(openthread-rtl87x2g
    PUBLIC
        "LINKER:SHELL:-L ${PROJECT_BINARY_DIR}/lib"
        "LINKER:SHELL:-L ${PROJECT_BINARY_DIR}/chip/lib"
)

target_compile_definitions(openthread-rtl87x2g
    PUBLIC
        "BUILD_MATTER"
)

target_include_directories(openthread-rtl87x2g
    PRIVATE
        ${REALTEK_SDK_ROOT}/subsys/matter/vendor
        ${REALTEK_SDK_ROOT}/subsys/matter/vendor/rtl87x2g
        ${REALTEK_SDK_ROOT}/subsys/matter/vendor/matter_ble
)

##################################################
# matter-cli-ftd
##################################################

if(${OT_CMAKE_NINJA_TARGET} STREQUAL "matter-cli-ftd")
    add_executable(matter-cli-ftd
        main_ns.c
    )

    target_include_directories(matter-cli-ftd
        PRIVATE
            $<TARGET_PROPERTY:ot-config,INTERFACE_INCLUDE_DIRECTORIES>
            ${REALTEK_SDK_ROOT}/subsys/cfu
            ${REALTEK_SDK_ROOT}/subsys/dfu
            ${OT_REALTEK_ROOT}/vendor/rtl87x2g/internal/cfu
            ${OT_REALTEK_ROOT}/vendor/bertl87x2ge4/internal/config_param
    )

    target_compile_definitions(matter-cli-ftd
        PRIVATE
            $<TARGET_PROPERTY:ot-config,INTERFACE_COMPILE_DEFINITIONS>
            "DLPS_EN=0"
    )

    if (matter_enable_rpc)
        target_link_libraries(matter-cli-ftd
            -Wl,--start-group
            chip_main
            CHIP
            PwRpc
            cli_uart
            openthread-cli-ftd
            openthread-rtl87x2g
            openthread-ftd
            lwip
            -Wl,--end-group
            ot-config-ftd
            ot-config
        )
    else()
        target_link_libraries(matter-cli-ftd
            -Wl,--start-group
            chip_main
            CHIP
            cli_uart
            openthread-cli-ftd
            openthread-rtl87x2g
            openthread-ftd
            lwip
            -Wl,--end-group
            ot-config-ftd
            ot-config
        )
    endif()
endif()

##################################################
# matter-cli-mtd
##################################################

if(${OT_CMAKE_NINJA_TARGET} STREQUAL "matter-cli-mtd")
    add_executable(matter-cli-mtd
        main_ns.c
        ${REALTEK_SDK_ROOT}/bsp/power/io_dlps.c
    )

    target_include_directories(matter-cli-mtd
        PRIVATE
            $<TARGET_PROPERTY:ot-config,INTERFACE_INCLUDE_DIRECTORIES>
    )

    if(DEFINED ENABLE_DLPS AND "${ENABLE_DLPS}" STREQUAL "ON")
        target_compile_definitions(matter-cli-mtd
            PRIVATE
                $<TARGET_PROPERTY:ot-config,INTERFACE_COMPILE_DEFINITIONS>
                "DLPS_EN=1"
        )
        target_compile_definitions(openthread-rtl87x2g PRIVATE "DLPS_EN=1")
    else()
        target_compile_definitions(matter-cli-mtd
            PRIVATE
                $<TARGET_PROPERTY:ot-config,INTERFACE_COMPILE_DEFINITIONS>
                "DLPS_EN=0"
        )
        target_compile_definitions(openthread-rtl87x2g PRIVATE "DLPS_EN=0")
    endif()

    target_link_libraries(matter-cli-mtd
        -Wl,--start-group
        chip_main
        CHIP
        cli_uart
        openthread-cli-mtd
        openthread-rtl87x2g
        openthread-mtd
        lwip
        -Wl,--end-group
        ot-config-mtd
        ot-config
    )
endif()

##################################################
# cli-uart
##################################################

if(${OT_CMAKE_NINJA_TARGET} STREQUAL "matter-cli-ftd" OR ${OT_CMAKE_NINJA_TARGET} STREQUAL "matter-cli-mtd")
    add_library(cli_uart
        ${OT_REALTEK_ROOT}/openthread/examples/apps/cli/cli_uart.cpp
    )

    target_include_directories(cli_uart
        PRIVATE
            $<TARGET_PROPERTY:ot-config,INTERFACE_INCLUDE_DIRECTORIES>
    )

    target_compile_definitions(cli_uart
        PRIVATE
            $<TARGET_PROPERTY:ot-config,INTERFACE_COMPILE_DEFINITIONS>
    )

    if(DEFINED ENABLE_CLI AND "${ENABLE_CLI}" STREQUAL "ON")
        target_compile_definitions(openthread-rtl87x2g PUBLIC "ENABLE_CLI=1")
    else()
        target_compile_definitions(openthread-rtl87x2g PUBLIC "ENABLE_CLI=0")
    endif()
endif()