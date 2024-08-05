/****************************************************************************************
  > File Name: smart_unordered_set.h
  > Author: Liu Hua Jun
  > Mail: wojiaoliuhuajun.com 
  > Note: Use of this source code is governed by The GNU Affero General Public License
          which can be found in the LICENSE file
 ***************************************************************************************/
#ifndef SMART_STL_UNORDERED_SET
#define SMART_STL_UNORDERED_SET

#include <unordered_set>
#include <cassert>

namespace smart_stl
{

template<typename VALUE_T, typename HASH_T = std::hash<VALUE_T>, typename PRED_T = std::equal_to<VALUE_T>,
    typename ALLOC_T = std::allocator<VALUE_T>>
class unordered_set : public std::unordered_set<VALUE_T, HASH_T, PRED_T, ALLOC_T>
{
public:
    unordered_set() {
        update_threshold();
    }
    ~unordered_set() {}
    using BASE = std::unordered_set<VALUE_T, HASH_T, PRED_T, ALLOC_T>;
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
    insert(const VALUE_T& __x)
    //insert(const typename BASE::value_type& __x)
    {
        if (! need_rehash()) {
            return BASE::insert(__x);
        }
        //realloc_insert(this->size(), ret_, std::forward<typename BASE::value_type>(__x));
        realloc_insert(this->size(), ret_, __x);
        return ret_;
    }

    inline std::pair<typename BASE::iterator, bool>
    insert(const typename BASE::value_type&& __x)
    {
        if (! need_rehash()) {
            return BASE::insert(std::move(__x));
        }
        realloc_insert(this->size(), ret_, std::move(__x));
        return ret_;
    }

    inline void reserve(typename BASE::size_type __n)
    {
        BASE::reserve(__n);
        update_threshold();
    }

    inline void rehash(typename BASE::size_type __n)
    {
        BASE::rehash(__n);
        update_threshold();
    }

    inline void max_load_factor(float __z)
    {
        BASE::max_load_factor(__z);
        update_threshold();
    }
private:
    inline bool need_rehash()
    {
        return (this->size() + 1 >= realloc_threshold_);
    }
    inline void update_threshold()
    {
        //std::cout << "before realloc_threshold_ = " << realloc_threshold_ << std::endl;
        realloc_threshold_ = BASE::bucket_count() * BASE::max_load_factor();
        //std::cout << "end realloc_threshold_ = " << realloc_threshold_ << std::endl;
    }
    bool __attribute__ ((noinline)) realloc_insert(volatile size_t current_size, std::pair<typename BASE::iterator, bool>& ret, const typename BASE::value_type& value)
    {
        assert(current_size == this->size());
        ret = BASE::insert(value);
        if (ret.second) {
            update_threshold();
            //std::cout << this << ", current_size = " << current_size << std::endl;
        }
        return ret.second;
    }

    bool __attribute__ ((noinline)) realloc_insert(volatile size_t current_size, std::pair<typename BASE::iterator, bool>& ret, const typename BASE::value_type&& value)
    {
        assert(current_size == this->size());
        ret = BASE::insert(value);
        if (ret.second) {
            update_threshold();
            //std::cout << this << ", current_size = " << current_size << std::endl;
        }
        return ret.second;
    }
    template<typename... _Args>
        bool __attribute__ ((noinline)) realloc_emplace(volatile size_t current_size, std::pair<typename BASE::iterator, bool>& ret, _Args&&... __args) {
            assert(current_size == this->size());
            ret = BASE::emplace(std::forward<_Args>(__args)...);
            if (ret.second) {
                update_threshold();
                //std::cout << this << ", current_size = " << current_size << std::endl;
            }
            return ret.second;
        }
private:
    std::pair<typename BASE::iterator, bool> ret_{};
    size_t realloc_threshold_{ 0 };
};
}
#endif
