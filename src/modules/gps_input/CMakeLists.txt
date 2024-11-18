px4_add_module(
	MODULE modules__gps_input
    MAIN gps_input
    STACK_MAIN 2500
    STACK_MAX 4000
    SRCS
        gps_input.cpp
    DEPENDS
	)
