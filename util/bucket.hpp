#include <vector>
#include <type_traits>

namespace concurrency_utils
{

/*
- this particicular instance of bucket is going to use mutexes for thread safety.
- the plan is to expand this project into implementing a lock-free vector as referenced
below:
https://www.stroustrup.com/lock-free-vector.pdf

this is just a pet-project that I am using to learn.
*/


/*
T is currently a struct as we are inserting a K,V pair as defined in
keyutils.hpp
*/

template <typename K, typename V>
class Bucket
{
private:
  size_t bucket_size = 0;
  const size_t default_bucket_size = 128;
  const double max_fill_ratio = 0.75;
  const double min_fill_ratio = 0.3;

  std::vector <KV_Pair> bucket;

  double check_load_factor ();
  void rehash (size_t new_size);
  size_t generate_hash (const K& key, size_t bucket_size); //return concurrency_utils::atomic_hash
  static std::size_t get_precomputed_hash (const K& key);




public:
  Bucket ();
  Bucket (std::size_t size);
  
  /*

  */
  template <typename T>
  void insert (const T& pair);
  void insert (const K& key, const V& value);
  void remove (const K& key);
  bool find (const K& key);
  const KV_Pair& retrieve_pair (const K& key);
};

/*
BUCKET IMPLEMENTATION
*/
template <typename K, typename V>
Bucket <K,V>::Bucket () : bucket (default_bucket_size) {}


template <typename K, typename V>
template <typename T>
void Bucket <K,V>::insert (const T& pair)
{
  if (check_load_factor() >= max_fill_ratio) { rehash (bucket.size() * 2); }

  std::size_t hash = concurrency_utils::KeyUtils <K>::generic_hash (pair.key, bucket.size ());
  std::size_t initial_index = hash;

  while (bucket[hash].key != K{})
  {
    hash = (hash + 1 ) % bucket.size();

    //if full
    if (hash == initial_index)
    {
      rehash (bucket.size() * 2);
      hash = concurrency_utils::KeyUtils <K>::generic_hash (pair.key, bucket.size ());
    }
  }

  bucket[hash].key = pair.key;
  bucket[hash].value = pair.value;
  bucket_size++;
}

template <typename K, typename V>
void Bucket <K,V>::remove (const K& key)
{
  if (check_load_factor() > min_fill_ratio) { rehash (bucket.size() / 2); }

  std::size_t index = concurrency_utils::KeyUtils <K>::generic_hash (key, bucket.size ());

  while (bucket[index] != K{})
  {
    if (bucket[index] == key)
    {
      bucket[index].key = K{};
      bucket[index].value = V{};
      return;
    }
    index = (index + 1) % bucket.size();
  }
}

template <typename K, typename V>
bool Bucket <K,V>::find (const K& key)
{
  if (!bucket.empty())
  {
    std::size_t index = concurrency_utils::KeyUtils <K>::generic_hash (key, bucket.size());
  }
}

};