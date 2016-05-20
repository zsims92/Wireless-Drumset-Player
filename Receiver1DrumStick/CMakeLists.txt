CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
# Your project name
PROJECT(receiver)

#######################################
# Toolchain setup
#######################################
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER msp430-gcc)
SET(CMAKE_AR msp430-ar)
SET(CMAKE_RANLIB msp430-ranlib)
# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  /usr/local/msp430/)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CMAKE_C_FLAGS "-Wall -Wextra -Wno-missing-field-initializers -Wno-missing-braces -Wno-unused-parameter -Wno-main -Wno-pointer-sign -Wreturn-type -mmcu=cc430f6137 -fdata-sections -ffunction-sections -Os -O2")
#SET(CMAKE_C_FLAGS "-W -Wreturn-type -mmcu=cc430f6137 -fdata-sections -ffunction-sections -g")
SET(CMAKE_SHARED_LINKER_FLAGS "-mmcu=cc430f6137 -Wl,--gc-sections -Wl,--print-gc-sections")
SET(CMAKE_EXE_LINKER_FLAGS "-mmcu=cc430f6137 -Wl,--gc-sections")

#######################################
# Wizzikit setup
# The absolute path of Wizzikit
#######################################
SET(WIZZIKIT_ROOT_PATH /home/mfay/UNL/advembbedded/wizzikit/)

######################################
# Where to find the .h files
######################################
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/hal/include)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/hal/src)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/rac/include)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/kal/include)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/drv/include)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/drv/tempa_mcp9700)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/drv/magsw_ah180)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/drv/accel_mma7660)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/drv/light_tept4400)
INCLUDE_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/at/include)
# Add your own folders of .h files


#####################################
# Where to find the libraries
#####################################
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/rac/lib)
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/build/at/src)
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/build/kal/src)
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/build/hal/src)
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/build/drv/tempa_mcp9700)
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/build/drv/magsw_ah180)
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/build/drv/accel_mma7660)
LINK_DIRECTORIES (${WIZZIKIT_ROOT_PATH}/build/drv/light_tept4400)
# Add your won folder of library files
LINK_DIRECTORIES(${WIZZIKIT_ROOT_PATH}/dash7/stack/framework/hal/chips/cc1101)

#####################################
# How to build the exectutable
#####################################
ADD_EXECUTABLE(receiver receive.c )
# You need to edit this part according to your needs

TARGET_LINK_LIBRARIES(receiver
    KAL_LIB
    HAL_LIB
    KAL_LIB
    RAC_LIB
    AT_LIB
    DRV_TMPA
    DRV_MMA
    DRV_MAGSW
    DRV_LIGHT)



