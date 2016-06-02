#pragma once

#include <unordered_map>

namespace mc {

  template <typename Key,
	    typename Val,
	    typename Hash = std::hash<Key>,
	    typename KeyEqual = std::equal_to<Key>,
	    typename Allocator = std::allocator< std::pair<const Key, Val>>>
    using HashMap = typename std::unordered_map<Key, Val>;
  template <typename A, typename B>
  using Pair = typename std::pair<A, B>;

}
