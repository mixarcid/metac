#pragma once

#include <vector>

namespace mc {

  template <typename T>
  using Array = typename std::vector<T>;

  template <typename T>
  String to_string(Array<T> arr) {

    String ret = "{\n";
    for (auto& elem : arr) {
      ret += " " + to_string(elem) + "\n";
    }
    ret += "}\n";

    return ret;
  }

}
