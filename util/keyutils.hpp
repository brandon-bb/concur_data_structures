#include <atomic>
#include <mutex>
#include <type_traits>
#include <string>
#include <unordered_map>

namespace concurrency_utils
{

template <typename K, typename V>
class KeyUtils
{
private:
  std::unordered_map <K, std::size_t> precomputed_hashes;

  struct KV_Pair
  {
    K key;
    V value;
  };

public:
  static std::size_t generic_hash (const K& key, std::size_t table_size);
  static std::size_t get_precomputed_hash (const K& key);
  static bool is_hashable (const K& key);
  static bool equal_keys (const K& key1, const K& key2);
  static bool is_string (const K& key);
  static std::string to_string (const K& key);
  
  static KV_Pair create_pair (const K& key, const V& value);
  static const K& get_key (const KV_Pair& pair);
  static const V& get_value (const KV_Pair& pair);
};




/*
- store generated hashcode to reduce latency
*/
template <typename K, typename V>
bool KeyUtils <K, V>::is_hashable (const K& key)
{
  return std::is_pod <K>::value;
}

template <typename K, typename V>
bool KeyUtils <K, V>::is_string (const K& key)
{
  return std::is_same <decltype (key), std::string>::value;
}


template <typename K, typename V>
bool KeyUtils <K, V>::equal_keys(const K& key1, const K& key2)
{
  return key1 == key2;
}

template <typename K, typename V>
std::string KeyUtils <K, V>::to_string (const K& key)
{
  return std::to_string (key);
}


template <typename K, typename V>
const K& KeyUtils <K, V>::get_key (const KV_Pair& pair)
{
  return pair.key;
}

template <typename K, typename V>
const V& KeyUtils <K, V>::get_value (const KV_Pair& pair)
{
  return pair.value;
}

template <typename K, typename V>
std::size_t KeyUtils <K, V>::generic_hash (const K& key, std::size_t table_size)
{
  if (!is_hashable (key))
  {
    //return an error sa
  }
  
  std::size_t hash = 0;
  const char* key_data = reinterpret_cast <const char*> (&key);
  std::size_t key_size = sizeof (K);

  for (std::size_t i = 0; i < key_size; i++)
  {
    hash = (hash * 31) + key_data[i];
  }

  return hash % table_size;
}

template <typename K, typename V>
std::size_t KeyUtils <K,V>::get_precomputed_hash (const K& key)
{
  if (!is_hashable (key))
  {
    // generate error stating key is not hashable
  }

  if (precomputed_hashes.contains (key))
  {
    return precomputed_hashes [key];
  }

  else
  {
    //throw an error stating the hash does not exist
    //suggest the user actions the find function to check if they key exists
  }
}

template <typename K, typename V>
typename KeyUtils <K, V>::KV_Pair KeyUtils <K, V>::create_pair (const K& key, const V& value)
{
  static_assert (KeyUtils <K, V>::is_hashable (key), "Key must be an integral type");

  KV_Pair pair;
  pair.key = key;
  pair.value = value;
  
  return pair;
}



}
