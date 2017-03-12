## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
##
## # The following are required to submit to the CDash dashboard:
##   ENABLE_TESTING()
##   INCLUDE(CTest)

set(CTEST_PROJECT_NAME "Test")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "192.168.0.102")
set(CTEST_DROP_LOCATION "/CDash/public/submit.php?project=Freeflow")
set(CTEST_DROP_SITE_CDASH TRUE)