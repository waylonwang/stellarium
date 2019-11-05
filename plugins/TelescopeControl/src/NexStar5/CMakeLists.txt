set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_library(TelescopeControl_NexStar5 STATIC
    NexStar5Command.hpp
    NexStar5Command.cpp
    NexStar5Connection.hpp
    NexStar5Connection.cpp
    TelescopeClientDirectNexStar5.hpp
    TelescopeClientDirectNexStar5.cpp
    )

target_link_libraries(TelescopeControl_NexStar5
    Qt5::Core
    Qt5::Network
    Qt5::Gui
    TelescopeControl_common
    )

SET_TARGET_PROPERTIES(TelescopeControl_NexStar5 PROPERTIES FOLDER "plugins/TelescopeControl")
