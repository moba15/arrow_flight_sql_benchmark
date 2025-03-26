#include <iostream>
// For EXIT_SUCCESS

#include <cstdlib>

// For strerror

#include <cstring>

#include <iostream>


#include <arrow-adbc/adbc.h>

#include <nanoarrow.h>


#define CHECK_ADBC(EXPR)                                          \

  if (AdbcStatusCode status = (EXPR); status != ADBC_STATUS_OK) { \

      if (error.message != nullptr) {                               \

          std::cerr << error.message << std::endl;                    \

      }                                                             \

      return EXIT_FAILURE;                                          \

  }


// Error-checking helper for ArrowArrayStream.

#define CHECK_STREAM(STREAM, EXPR)                            \

  if (int status = (EXPR); status != 0) {                     \

      std::cerr << "(" << std::strerror(status) << "): ";       \

      const char* message = (STREAM).get_last_error(&(STREAM)); \

      if (message != nullptr) {                                 \

          std::cerr << message << std::endl;                      \

      } else {                                                  \

          std::cerr << "(no error message)" << std::endl;         \

      }                                                         \

      return EXIT_FAILURE;                                      \

  }


// Error-checking helper for Nanoarrow.

#define CHECK_NANOARROW(EXPR)                                              \

  if (int status = (EXPR); status != 0) {                                  \

      std::cerr << "(" << std::strerror(status) << "): failed" << std::endl; \

      return EXIT_FAILURE;                                                   \

  }



int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}