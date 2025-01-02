include(FetchContent)

FetchContent_Declare(
  check
  GIT_REPOSITORY https://github.com/libcheck/check
  GIT_TAG        origin/master
)

FetchContent_MakeAvailable(check)