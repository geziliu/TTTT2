cmake_minimum_required(VERSION 3.4.1)
include_directories(C:/Users/eh/AndroidStudioProjects/TTTT/app/src/main/include)
#加载第三方的so头文件
add_library(openssl SHARED IMPORTED )
#add_library(ssl SHARED IMPORTED )
set_target_properties(openssl PROPERTIES IMPORTED_LOCATION C:/Users/eh/AndroidStudioProjects/TTTT/app/libs/${ANDROID_ABI}/libcrypto.so)
#set_target_properties(ssl PROPERTIES IMPORTED_LOCATION C:/Users/eh/AndroidStudioProjects/TTTT/app/libs/${ANDROID_ABI}/libssl.so)
#前面4行中的openssl ssl是自定义名，可以随便取，相当于给加载的第三方so取个名字 。
#如果只用MD5的话其实加载libcrypto.so就行了，我这里只是在尝试着同时加载多个第三方so时格式要怎么写
add_library(native-lib SHARED native-lib.cpp)
#自定义native-lib路径（和CMakeLists同目录，所以直接写的文件名）
find_library(log-lib log)
target_link_libraries(native-lib openssl ${log-lib})
#target_link_libraries(native-lib openssl ssl ${log-lib})
