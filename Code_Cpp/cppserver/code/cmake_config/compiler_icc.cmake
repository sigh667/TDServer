CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
# Set build-directive (used in core to tell which buildtype we used)
### 设置构建-指令

# 设置编译类型宏
add_definitions(-D_BUILD_DIRECTIVE="${CMAKE_BUILD_TYPE}")

if( USE_SFMT)
  if(PLATFORM EQUAL 32)
	# 32位系统
    add_definitions(-axSSE2)
  else()
    add_definitions(-xSSE2)
  endif()
  message(STATUS "ICC: SFMT enabled, SSE2 flags forced")
endif()

# 警告
if( WITH_WARNINGS )
  add_definitions(-w1)
  message(STATUS "ICC All warnings enabled")
else()
  add_definitions(-w)
  message(STATUS "ICC: All warnings disabled")
endif()

# 调试
if( WITH_COREDEBUG )
  add_definitions(-g)
  message(STATUS "ICC: Debug-flag set (-g)")
endif()
