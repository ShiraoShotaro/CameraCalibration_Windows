cmake_minimum_required(VERSION 3.0)

# ソリューションファイルの名前を指定する
project(CameraCalibration)

file(GLOB_RECURSE proj_cpp_files src/*.cpp)
file(GLOB_RECURSE proj_header_files src/*.hpp)

add_executable(CameraCalibration ${proj_cpp_files} ${proj_header_files})

# 次の行を追加するとMinSizeRelとRelWithDebInfoの選択肢を生成することが抑制できます。
#SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)

#次の行を追加すると-DWITH_SOMELIBをコンパイルオプション指定できる。
#add_definitions(-DWITH_SOMELIB)

# OpenCVのパッケージを探す
find_package(OpenCV REQUIRED)

# OpenCVが見つかった場合のみ設定を行う
if(OpenCV_FOUND)
  # インクルードパスを指定
  include_directories(${OpenCV_INCLUDE_DIRS} src)

  # リンクするライブラリ指定
  target_link_libraries(CameraCalibration ${OpenCV_LIBS})
endif(OpenCV_FOUND)
