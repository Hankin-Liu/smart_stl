/****************************************************************************************
  > File Name: smart_unordered_map.h
  > Author: Liu Hua Jun
  > Mail: wojiaoliuhuajun.com 
  > Note: Use of this source code is governed by The GNU Affero General Public License
          which can be found in the LICENSE file
 ***************************************************************************************/
#ifndef SMART_STL_UNORDERED_MAP
#define SMART_STL_UNORDERED_MAP

#include <unordered_map>

namespace smart_stl
{

template<typename KEY_T, typename VALUE_T, typename HASH_T = std::hash<KEY_T>,
    typename PRED_T = std::equal_to<KEY_T>, typename ALLOC_T = std::allocator<std::pair<const KEY_T, VALUE_T>>>
class unordered_map : public std::unordered_map<KEY_T, VALUE_T, HASH_T, PRED_T, ALLOC_T>
{
public:
    unordered_map() {
        update_threshold();
    }
    ~unordered_map() {}
    using BASE = std::unordered_map<KEY_T, VALUE_T, HASH_T, PRED_T, ALLOC_T>;
public:
    template<typename... _Args>
	inline std::pair<typename BASE::iterator, bool>
    emplace(_Args&&... __args)
	{
        if (! need_rehash()) {
            return BASE::emplace(std::forward<_Args>(__args)...);
        }
        realloc_emplace(this->size(), ret_, std::forward<_Args>(__args)...);
        return ret_;
    }

    inline std::pair<typename BASE::iterator, bool>
    insert(const typename BASE::value_type& __x)
    {
        if (! need_rehash()) {
            return BASE::insert(__x);
        }
        realloc_insert(BASE::size(), ret_, __x);
        return ret_;
    }
private:
    inline bool need_rehash()
    {
        return (BASE::size() + 1 >= realloc_threshold_);
    }
    inline void update_threshold()
    {
        //std::cout << "before realloc_threshold_ = " << realloc_threshold_ << std::endl;
        realloc_threshold_ = BASE::bucket_count() * BASE::max_load_factor();
        //std::cout << "end realloc_threshold_ = " << realloc_threshold_ << std::endl;
    }
    bool __attribute__ ((noinline)) realloc_insert(volatile size_t current_size, std::pair<typename BASE::iterator, bool>& ret, const typename BASE::value_type& value)
    {
        ret = BASE::insert(value);
        if (ret.second) {
            update_threshold();
        }
        return ret.second;
    }
    template<typename... _Args>
    bool __attribute__ ((noinline)) realloc_emplace(volatile size_t current_size, std::pair<typename BASE::iterator, bool>& ret, _Args&&... __args) {
        ret = BASE::emplace(std::forward<_Args>(__args)...);
        if (ret.second) {
            update_threshold();
        }
        return ret.second;
    }
private:
    std::pair<typename BASE::iterator, bool> ret_{};
    size_t realloc_threshold_{ 0 };
};
}
#endif
