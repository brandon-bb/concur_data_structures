#pragma once

#include <vector>
#include <mutex>
#include <type_traits>
#include <iterator>

namespace concurrency
{
template <typename K, typename V>
class Concurrent_HashMap 
{
private:
  std::vector <Bucket <K,V>> buckets;
  std::vector <std::mutex> bucket_mutexes;
};

/*
insert function example
- calculate bucket index
lock specific bucket - std::unique_lock<std::mutex> lock (bucket_mutexes[bucket_index]);
insert pair.
*/


}