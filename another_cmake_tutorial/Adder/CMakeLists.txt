add_library(add adder.cpp)

install(TARGETS add DESTINATION lib)
install(FILES adder.h DESTINATION include)
