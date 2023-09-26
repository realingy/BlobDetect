cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

# Qt查询路径
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${ACTIVE_QT_DIR}")
set(INSTALL_PREFIX_INITIALIZED ${CMAKE_CURRENT_SOURCE_DIR}/build/install CACHE STRINGS "change prefix")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)
# set(RESOURCE_DIR resources/resources.qrc)

set(Qt5_DIR /opt/Qt5.13.2/5.13.2/gcc_64/lib/cmake/Qt5)
find_package(Qt5 REQUIRED Widgets Core)
# qt5_wrap_cpp( MOC widget.h)
# qt5_wrap_ui( UIC widget.ui)
# qt5_add_resources(RCC resources.qrc)

# opencv库查询
set(OpenCV_Root /thirdparty/opencv401)
set(OpenCV_DIR /usr/local/lib)

find_package(OpenCV REQUIRED)

# message("Opencv Libs: " ${OpenCV_LIBS})

# 包含目录
include_directories(${OpenCV_Root}/include)
include_directories(${OpenCV_Root}/include/opencv)
include_directories(${OpenCV_Root}/include/opencv2)

# message("XXXX: " ${OpenCV_Root}/include)
# message("ZZZZ: " ${OPENCV401_LIBRARIES})

# 查找源文件
aux_source_directory(. DIR_SRCS)

# 查找头文件
file(GLOB HEADER ./*.h)

set(EXE_NAME blobtest)
add_executable(${EXE_NAME} ${DIR_SRCS} ${HEADER})
target_link_libraries(${EXE_NAME} Qt5::Widgets ${OpenCV_LIBS})
# target_link_libraries(${EXE_NAME} Qt5::Widgets ${OPENCV401_LIBRARIES})