#pragma once
#include <iostream>

#define VERIFY(x) if (!(x)) { std::cout << "Verification failed!" << std::endl;  exit(-1); }
#define VERIFY_NOT_REACHED() VERIFY(0)