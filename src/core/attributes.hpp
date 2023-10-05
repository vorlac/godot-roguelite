#pragma once

namespace rl::inline utils
{

#if defined(_MSC_VER)
    // MSVC doesn't have a clean way to ignore custom
    // attributes within a namespace like clang or gcc
  #define property
  #define signal_slot
#elif defined(__GNUG__) || defined(__clang__)
    // these macros are used to define custom attributes to label godot signal callbacks
    // and node properties. they don't do anything other than making it easier to spot
    // these functions when reading the code or searching for them in an IDE.
  #define property    __attribute__((rl("property")))
  #define signal_slot __attribute__((rl("signal_slot")))
#endif

}
