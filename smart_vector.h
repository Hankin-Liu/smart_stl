/****************************************************************************************
  > File Name: smart_vector.h
  > Author: Liu Hua Jun
  > Mail: wojiaoliuhuajun.com 
  > Note: Use of this source code is governed by The GNU Affero General Public License
          which can be found in the LICENSE file
 ***************************************************************************************/
#ifndef SMART_STL_VECTOR
#define SMART_STL_VECTOR

#include <vector>
#include <cassert>

namespace smart_stl
{

template<typename T, typename Allocator = std::allocator<T>>
class vector : public std::vector<T, Allocator>
{
public:
    vector() {}
    ~vector() {}
public:
    inline void push_back(const T& value) {
        if (this->size() != this->capacity()) {
            std::vector<T, Allocator>::push_back(value);
            return;
        }
        realloc_insert(this->size(), value);
    }
#if __cplusplus >= 201103L
    inline void push_back(const T&& value) {
        emplace_back(std::move(value));
    }
#endif

#if __cplusplus >= 201103L
    template<typename... _Args>
    inline 
#if __cplusplus > 201402L
      typename vector<T, Allocator>::reference
#else
      void
#endif
      emplace_back(_Args&&... __args)
      {
        if (this->size() != this->capacity()) {
            std::vector<T, Allocator>::emplace_back(std::forward<_Args>(__args)...);
        } else {
            realloc_emplace(this->size(), std::forward<_Args>(__args)...);
        }
#if __cplusplus > 201402L
        return back();
#endif
      }
#endif

private:
    void __attribute__ ((noinline)) realloc_insert(volatile size_t current_size, const T& value)
    {
        assert(current_size == this->size());
        std::vector<T, Allocator>::push_back(value);
    }
#if __cplusplus >= 201103L
    template<typename... _Args>
    void __attribute__ ((noinline)) realloc_emplace(volatile size_t current_size, _Args&&... __args)
    {
        assert(current_size == this->size());
        std::vector<T, Allocator>::emplace_back(std::forward<_Args>(__args)...);
    }
#endif
};

}
#endif
