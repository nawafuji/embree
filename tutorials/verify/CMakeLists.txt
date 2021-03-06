## ======================================================================== ##
## Copyright 2009-2016 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

ADD_EXECUTABLE(verify verify.cpp ../common/tutorial/application.cpp)
TARGET_LINK_LIBRARIES(verify sys scenegraph embree)
SET_PROPERTY(TARGET verify PROPERTY FOLDER tutorials)
INSTALL(TARGETS verify DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT examples)

ADD_TEST(NAME verify COMMAND verify --no-colors)
IF (EMBREE_TESTING_MEMCHECK AND EMBREE_MEMORYCHECK_COMMAND)
  ADD_MEMCHECK_TEST(verify_memcheck verify 
     --no-colors --intensity 0.1 
     --skip .*AVX.*               # valgrind does not support AVX
     --skip .*AVX2.*              # valgrind does not support AVX2
     --skip .*AVX512KNL.*         # valgrind does not support AVX512KNL
     --skip .*AVX512SKX.*         # valgrind does not support AVX512SKX
  )
  set_tests_properties(verify_memcheck PROPERTIES TIMEOUT 3000)
ENDIF()

IF (EMBREE_TESTING_BENCHMARK)
  ADD_TEST(NAME verify_benchmarks COMMAND verify
    --no-colors --cdash
    --benchmark-tolerance 0.05
    --database "${EMBREE_TESTING_BENCHMARK_DATABASE}"
    --run .*benchmarks.*
    --skip .*_120.* --skip .*_1k.* --skip .*_10k.* --skip .*100k.*  # skip all smaller build benchmarks
  )
  set_tests_properties(verify_benchmarks PROPERTIES TIMEOUT 3000)
ENDIF()

SET(CPACK_NSIS_MENU_LINKS ${CPACK_NSIS_MENU_LINKS} "${CMAKE_INSTALL_BINDIR}/verify" "verify")
SET(CPACK_NSIS_MENU_LINKS ${CPACK_NSIS_MENU_LINKS} PARENT_SCOPE)
