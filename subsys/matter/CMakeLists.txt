#
# Copyright (c) 2022 Project CHIP Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.10.2)
project(rtk-matter VERSION 0.2.0 LANGUAGES C CXX ASM)

# TODO: Modify according to IC
set(MATTER_ADAPTER_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(OT_REALTEK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../openthread")

set(openthread_root "${OT_REALTEK_ROOT}/openthread")
set(bee_matter_root "$ENV{MATTER_EXAMPLE_PATH}/../../../..")
set(matter_output_path ${PROJECT_BINARY_DIR})
set(matter_example_path $ENV{MATTER_EXAMPLE_PATH})

set(RT_PLATFORM_VALUES
    "bee4"
    "bee3plus"
)
set_property(CACHE RT_PLATFORM PROPERTY STRINGS ${RT_PLATFORM_VALUES})
if(NOT RT_PLATFORM IN_LIST RT_PLATFORM_VALUES)
    message(FATAL_ERROR "Please select a supported platform: ${RT_PLATFORM_VALUES}")
endif()

string(FIND "${BUILD_TARGET}" "rtl8777g" FOUND_INDEX)
if(NOT FOUND_INDEX EQUAL -1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBOARD_RTL8777G")
endif()

string(FIND "${BUILD_TARGET}" "rtl8771htv" FOUND_INDEX)
if(NOT FOUND_INDEX EQUAL -1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBOARD_RTL8771HTV")
endif()

if(${BUILD_TARGET} STREQUAL "evb" OR ${BUILD_TARGET} STREQUAL "evb_dual")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBOARD_EVB")
endif()

set(LD_FILE "${OT_REALTEK_ROOT}/vendor/${RT_PLATFORM}/${BUILD_TARGET}/app.ld.gen")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# openthread config
set(OT_PLATFORM "external" CACHE STRING "")
set(OT_SLAAC ON CACHE BOOL "")
set(OT_BORDER_AGENT OFF CACHE BOOL "")
set(OT_BORDER_ROUTER OFF CACHE BOOL "")
set(OT_COMMISSIONER OFF CACHE BOOL "")
set(OT_JOINER ON CACHE BOOL "")
set(OT_DNS_CLIENT ON CACHE BOOL "")
set(OT_DHCP6_SERVER OFF CACHE BOOL "")
set(OT_COAP_OBSERVE OFF CACHE BOOL "")
set(OT_LINK_RAW OFF CACHE BOOL "")
set(OT_UDP_FORWARD OFF CACHE BOOL "")
set(OT_ECDSA ON CACHE BOOL "")
set(OT_SNTP_CLIENT OFF CACHE BOOL "")
set(OT_CHILD_SUPERVISION ON CACHE BOOL "")
set(OT_JAM_DETECTION OFF CACHE BOOL "")
set(OT_LOG_LEVEL_DYNAMIC ON CACHE BOOL "")
set(OT_EXTERNAL_HEAP ON CACHE BOOL "")
set(OT_SRP_CLIENT ON CACHE BOOL "")
set(OT_DUA ON CACHE BOOL "")
set(OT_BACKBONE_ROUTER OFF CACHE BOOL "")
set(OT_MLR ON CACHE BOOL "")
set(OT_CSL_RECEIVER ON CACHE BOOL "")
set(OT_LINK_METRICS_INITIATOR OFF CACHE BOOL "")
set(OT_CHANNEL_MONITOR OFF CACHE BOOL "")
set(OT_CHANNEL_MANAGER OFF CACHE BOOL "")
if(DEFINED ENABLE_RAM_REDUCE AND "${ENABLE_RAM_REDUCE}" STREQUAL "ON")
    set(OT_DHCP6_CLIENT OFF CACHE BOOL "")
    set(OT_COAP OFF CACHE BOOL "")
    set(OT_COAPS OFF CACHE BOOL "")
    set(OT_MAC_FILTER OFF CACHE BOOL "")
    set(OT_SERVICE OFF CACHE BOOL "")
    set(OT_LINK_METRICS_SUBJECT OFF CACHE BOOL "")
    set(OT_MULTIPLE_INSTANCE ON CACHE BOOL "")
else()
    set(OT_DHCP6_CLIENT ON CACHE BOOL "")
    set(OT_COAP ON CACHE BOOL "")
    set(OT_COAPS ON CACHE BOOL "")
    set(OT_MAC_FILTER ON CACHE BOOL "")
    set(OT_SERVICE ON CACHE BOOL "")
    set(OT_LINK_METRICS_SUBJECT ON CACHE BOOL "")
endif()
set(OT_EXTERNAL_MBEDTLS "mbedtls" CACHE STRING "")

# different on Github
include(${OT_REALTEK_ROOT}/Realtek/${BUILD_TYPE}.cmake)
include(${OT_REALTEK_ROOT}/Realtek/openthread.cmake)

get_filename_component(SDK_ROOT ${REALTEK_SDK_ROOT} REALPATH)
get_filename_component(CHIP_ROOT ${bee_matter_root} REALPATH)

if(DEFINED BUILD_USB AND "${BUILD_USB}" STREQUAL "ON")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUILD_USB")
endif()

if(DEFINED BUILD_CERT AND "${BUILD_CERT}" STREQUAL "ON")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUILD_CERT")
endif()

if(DEFINED ENABLE_CFU AND "${ENABLE_CFU}" STREQUAL "ON")
    set (matter_enable_cfu 1)
else()
    set (matter_enable_cfu 0)
endif()

if(DEFINED ENABLE_PERSISTENTSTORAGE_AUDIT AND "${ENABLE_PERSISTENTSTORAGE_AUDIT}" STREQUAL "ON")
    set (matter_enable_persistentstorage_audit 1)
else()
    set (matter_enable_persistentstorage_audit 0)
endif()

if(DEFINED ENABLE_FACTORY_DATA AND "${ENABLE_FACTORY_DATA}" STREQUAL "ON")
    set (matter_enable_factory_data 1)
else()
    set (matter_enable_factory_data 0)
endif()

if(DEFINED ENABLE_FACTORY_DATA_ENCRYPTION AND "${ENABLE_FACTORY_DATA_ENCRYPTION}" STREQUAL "ON")
    set (matter_factory_data_encryption 1)
else()
    set (matter_factory_data_encryption 0)
endif()

if(DEFINED ENABLE_DAC_KEY_ENCRYPTION AND "${ENABLE_DAC_KEY_ENCRYPTION}" STREQUAL "ON")
    set (matter_dac_key_encryption 1)
else()
    set (matter_dac_key_encryption 0)
endif()

if(DEFINED ENABLE_BLE_OTA AND "${ENABLE_BLE_OTA}" STREQUAL "ON")
    set (matter_enable_ble_ota 1)
else()
    set (matter_enable_ble_ota 0)
endif()

if(DEFINED ENABLE_OTA_REQUESTOR AND "${ENABLE_OTA_REQUESTOR}" STREQUAL "ON")
    set (matter_enable_ota_requestor 1)
else()
    set (matter_enable_ota_requestor 0)
endif()

if(DEFINED ENABLE_CLI AND "${ENABLE_CLI}" STREQUAL "ON")
    set (matter_enable_otcli 1)
else()
    set (matter_enable_otcli 0)
endif()

if(DEFINED ENABLE_PW_RPC AND "${ENABLE_PW_RPC}" STREQUAL "ON")
    set (matter_enable_rpc 1)
else()
    set (matter_enable_rpc 0)
endif()

if(DEFINED ENABLE_SHELL AND "${ENABLE_SHELL}" STREQUAL "ON")
    set (matter_enable_shell 1)
else()
    set (matter_enable_shell 0)
endif()

if(DEFINED ENABLE_RAM_REDUCE AND "${ENABLE_RAM_REDUCE}" STREQUAL "ON")
    set (matter_enable_ram_reduce 1)
else()
    set (matter_enable_ram_reduce 0)
endif()

if(${OT_CMAKE_NINJA_TARGET} STREQUAL "matter-cli-ftd")
    set (matter_enable_ftd 1)
endif()

if(${OT_CMAKE_NINJA_TARGET} STREQUAL "matter-cli-mtd")
    set (matter_enable_mtd 1)

    if(DEFINED ENABLE_MATTER_MED AND "${ENABLE_MATTER_MED}" STREQUAL "ON")
        set (matter_enable_med 1)
    else()
        set (matter_enable_med 0)
    endif()

    if(DEFINED ENABLE_ICD_LIT AND "${ENABLE_ICD_LIT}" STREQUAL "ON")
        set (matter_enable_icd_lit 1)
    else()
        set (matter_enable_icd_lit 0)
    endif()

    if(DEFINED ENABLE_DLPS AND "${ENABLE_DLPS}" STREQUAL "ON")
        set (matter_enable_dlps 1)
    else()
        set (matter_enable_dlps 0)
    endif()
endif()

if(${RT_PLATFORM} STREQUAL "bee3plus")
    set(bee_lwip_path ${REALTEK_SDK_ROOT}/sdk/subsys/lwip/for_matter)
else()
    set(bee_lwip_path ${REALTEK_SDK_ROOT}/subsys/lwip/for_matter)
endif()

include(${bee_lwip_path}/lwip.cmake)
include(includepath.cmake)
include(${CHIP_ROOT}/src/app/chip_data_model.cmake)
include($ENV{MATTER_EXAMPLE_PATH}/chip_main.cmake)
include(${CHIP_ROOT}/config/realtek/bee/chip.cmake)

add_dependencies(chip_main chip-gn)

add_subdirectory(${OT_REALTEK_ROOT}/vendor/${RT_PLATFORM} ${CMAKE_CURRENT_BINARY_DIR}/vendor/${RT_PLATFORM})
