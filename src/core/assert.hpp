#pragma once

#include <cstdio>
#include <iostream>

#include <godot_cpp/core/error_macros.hpp>

#ifndef NDEBUG
  //
  // In debug mode, checks the passed in condition and outputs
  // detailed information to stederr, including a custom error
  // message when the condition evaluates to false.
  //
  #define assertion(condition, message)                                             \
      do                                                                            \
      {                                                                             \
          if (!(condition)) [[unlikely]]                                            \
          {                                                                         \
              ::godot::_err_print_error(__FUNCTION__, __FILE__, __LINE__,           \
                                        message " => condition: (" #condition ")"); \
              ::godot::_err_flush_stdout();                                         \
              GENERATE_TRAP();                                                      \
          }                                                                         \
      }                                                                             \
      while (false)

  #define error_msg(message)                                                    \
      do                                                                        \
      {                                                                         \
          ::godot::_err_print_error(__FUNCTION__, __FILE__, __LINE__, message); \
          ::godot::_err_flush_stdout();                                         \
          GENERATE_TRAP();                                                      \
      }                                                                         \
      while (false)

  #define runtime_assert(condition) assertion(condition, "validation check failed")

#else
//
// In release mode the macro does nothing ((void)0), including
// the execution of the condition so don't define the expression
// as anything that would be considered program logis.
//
  #define assertion(condition, message) static_cast<void>(0)
  #define error_msg(message)            static_cast<void>(0)
  #define runtime_assert(condition)     static_cast<void>(0)
#endif
