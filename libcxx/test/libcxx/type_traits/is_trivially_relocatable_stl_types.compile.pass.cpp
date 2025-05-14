//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++98, c++03, c++11, c++14
//
// <type_traits>
//
// Test that many standard types are trivially relocatable.

#include "test_macros.h"

#include <any>
#include <array>
#include <atomic>
#include <barrier>
#include <bitset>
#include <chrono>
#include <condition_variable>
#include <compare>
#include <deque>
#include <exception>
#include <expected>
//#include <flat_map>
//#include <flat_set>
#include <forward_list>
#include <functional>
#include <future>
//#include <generator>
#include <initializer_list>
#include <ios>
#include <iterator>
#include <latch>
#include <list>
#include <locale>
#include <map>
#include <mdspan>
#include <memory>
#include <memory_resource>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <ranges>
#include <ratio>
#include <regex>
#include <semaphore>
#include <set>
#include <shared_mutex>
#include <source_location>
#include <span>
#include <stack>
//#include <stacktrace>
#include <stop_token>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>


struct T { T(); T(const T&); ~T(); };
struct R { void operator()() const; };
struct SM { void lock_shared(); void unlock_shared(); ~SM(); };

struct K {
    T t;
    bool operator==(const K&) const;
    bool operator< (const K&) const;
    struct Cmp {
        Cmp(); Cmp(const Cmp&); ~Cmp();
        bool operator()(const K&, const K&) const;
    };
};
template<> struct std::hash<K> { size_t operator()(const K&) const; };

static_assert(std::is_trivially_relocatable_v<R>);
static_assert(!std::is_trivially_relocatable_v<T>);
static_assert(std::is_trivially_relocatable_v<T*>);
static_assert(std::is_trivially_relocatable_v<std::reference_wrapper<T>>);
static_assert(std::is_trivially_relocatable_v<std::initializer_list<T>>);
static_assert(std::is_trivially_relocatable_v<std::allocator<T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::memory_resource>);  // virtual destructor
static_assert(std::is_trivially_relocatable_v<std::pmr::polymorphic_allocator<T>>);
static_assert(std::is_trivially_relocatable_v<std::default_delete<T>>);
static_assert(std::is_trivially_relocatable_v<std::hash<std::string>>);
static_assert(std::is_trivially_relocatable_v<std::less<T>>);
static_assert(std::is_trivially_relocatable_v<std::less<void>>);
static_assert(!std::is_trivially_relocatable_v<std::unique_ptr<T>>);
static_assert(!std::is_trivially_relocatable_v<std::shared_ptr<T>>);
static_assert(!std::is_trivially_relocatable_v<std::weak_ptr<T>>);
static_assert(!std::is_trivially_relocatable_v<std::pair<R,R>>);
static_assert(!std::is_trivially_relocatable_v<std::pair<R,T>>);
static_assert(!std::is_trivially_relocatable_v<std::tuple<R,R,R>>);
static_assert(!std::is_trivially_relocatable_v<std::tuple<R,T,R>>);
static_assert(std::is_trivially_relocatable_v<std::variant<R,R,R>>);
static_assert(!std::is_trivially_relocatable_v<std::variant<R,T,R>>);
static_assert(std::is_trivially_relocatable_v<std::optional<R>>);
static_assert(!std::is_trivially_relocatable_v<std::optional<T>>);
static_assert(!std::is_trivially_relocatable_v<std::any>);
static_assert(!std::is_trivially_relocatable_v<std::locale>);
static_assert(!std::is_trivially_relocatable_v<std::exception_ptr>);
static_assert(!std::is_trivially_relocatable_v<std::exception>);  // virtual destructor
static_assert(!std::is_trivially_relocatable_v<std::error_category>);
static_assert(std::is_trivially_relocatable_v<std::error_code>);
static_assert(std::is_trivially_relocatable_v<std::error_condition>);
static_assert(std::is_trivially_relocatable_v<std::errc>);
static_assert(std::is_trivially_relocatable_v<std::type_index>);
static_assert(std::is_trivially_relocatable_v<std::array<R, 5>>);
static_assert(!std::is_trivially_relocatable_v<std::array<T, 5>>);
static_assert(!std::is_trivially_relocatable_v<std::deque<T>>);
static_assert(!std::is_trivially_relocatable_v<std::forward_list<T>>);
static_assert(!std::is_trivially_relocatable_v<std::list<T>>);
static_assert(!std::is_trivially_relocatable_v<std::map<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::multimap<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::multiset<K>>);
static_assert(!std::is_trivially_relocatable_v<std::set<K>>);
static_assert(!std::is_trivially_relocatable_v<std::unordered_map<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::unordered_multimap<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::unordered_multiset<K>>);
static_assert(!std::is_trivially_relocatable_v<std::unordered_set<K>>);
static_assert(!std::is_trivially_relocatable_v<std::vector<T>>);
static_assert(!std::is_trivially_relocatable_v<std::vector<bool>>);
static_assert(!std::is_trivially_relocatable_v<std::stack<T>>);
static_assert(!std::is_trivially_relocatable_v<std::stack<T, std::list<T>>>);
static_assert(!std::is_trivially_relocatable_v<std::queue<T>>);
static_assert(!std::is_trivially_relocatable_v<std::queue<T, std::list<T>>>);
static_assert(!std::is_trivially_relocatable_v<std::priority_queue<K>>);
static_assert(!std::is_trivially_relocatable_v<std::priority_queue<K, std::deque<K>>>);
static_assert(!std::is_trivially_relocatable_v<std::priority_queue<K, std::deque<K>, K::Cmp>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::deque<T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::forward_list<T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::list<T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::map<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::multimap<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::multiset<K>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::set<K>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::unordered_map<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::unordered_multimap<K,T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::unordered_multiset<K>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::unordered_set<K>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::vector<T>>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::vector<bool>>);
static_assert(std::is_trivially_relocatable_v<std::array<R, 5>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::array<T, 5>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::deque<T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::forward_list<T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::list<T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::map<K,T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::multimap<K,T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::multiset<K>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::set<K>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_map<K,T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_multimap<K,T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_multiset<K>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_set<K>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_map<K,T>::local_iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_multimap<K,T>::local_iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_multiset<K>::local_iterator>);
static_assert(std::is_trivially_relocatable_v<std::unordered_set<K>::local_iterator>);
static_assert(std::is_trivially_relocatable_v<std::vector<T>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::vector<T>::const_iterator>);
static_assert(!std::is_trivially_relocatable_v<std::vector<bool>::iterator>);
static_assert(std::is_trivially_relocatable_v<std::vector<bool>::const_iterator>);
static_assert(std::is_trivially_relocatable_v<std::reverse_iterator<int*>>);
static_assert(std::is_trivially_relocatable_v<std::back_insert_iterator<std::deque<int>>>);
static_assert(std::is_trivially_relocatable_v<std::front_insert_iterator<std::list<int>>>);
static_assert(std::is_trivially_relocatable_v<std::insert_iterator<std::vector<int>>>);
static_assert(!std::is_trivially_relocatable_v<std::insert_iterator<std::vector<bool>>>);
static_assert(std::is_trivially_relocatable_v<std::istream_iterator<R>>);
static_assert(!std::is_trivially_relocatable_v<std::istream_iterator<T>>);
static_assert(std::is_trivially_relocatable_v<std::ostream_iterator<R>>);
static_assert(std::is_trivially_relocatable_v<std::ostream_iterator<T>>);
static_assert(std::is_trivially_relocatable_v<std::char_traits<char>>);
static_assert(!std::is_trivially_relocatable_v<std::regex_traits<char>>);  // contains a locale
static_assert(!std::is_trivially_relocatable_v<std::regex>);  // contains a regex_traits
static_assert(!std::is_trivially_relocatable_v<std::cmatch>);  // contains a vector
static_assert(!std::is_trivially_relocatable_v<std::smatch>);  // contains a vector
static_assert(!std::is_trivially_relocatable_v<std::string>);
static_assert(std::is_trivially_relocatable_v<std::string::iterator>);
static_assert(!std::is_trivially_relocatable_v<std::wstring>);
static_assert(std::is_trivially_relocatable_v<std::wstring::iterator>);
static_assert(std::is_trivially_relocatable_v<std::string_view>);
static_assert(std::is_trivially_relocatable_v<std::string_view::iterator>);
static_assert(std::is_trivially_relocatable_v<std::wstring_view>);
static_assert(std::is_trivially_relocatable_v<std::wstring_view::iterator>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::cmatch>);  // contains a vector
static_assert(!std::is_trivially_relocatable_v<std::pmr::smatch>);  // contains a vector
static_assert(!std::is_trivially_relocatable_v<std::pmr::string>);
static_assert(std::is_trivially_relocatable_v<std::pmr::string::iterator>);
static_assert(!std::is_trivially_relocatable_v<std::pmr::wstring>);
static_assert(std::is_trivially_relocatable_v<std::pmr::wstring::iterator>);
static_assert(std::is_trivially_relocatable_v<std::integral_constant<int, 42>>);
static_assert(std::is_trivially_relocatable_v<std::index_sequence<1,2,3,4>>);
static_assert(std::is_trivially_relocatable_v<std::ratio<1, 2>>);
static_assert(std::is_trivially_relocatable_v<std::chrono::system_clock::duration>);
static_assert(std::is_trivially_relocatable_v<std::chrono::system_clock::time_point>);
static_assert(std::is_trivially_relocatable_v<std::streampos>);
static_assert(std::is_trivially_relocatable_v<std::bitset<42>>);
static_assert(std::is_trivially_relocatable_v<std::bitset<420>>);
static_assert(!std::is_trivially_relocatable_v<std::unique_lock<std::mutex>>);
static_assert(!std::is_trivially_relocatable_v<std::shared_lock<SM>>);
static_assert(!std::is_trivially_relocatable_v<std::thread>);
static_assert(std::is_trivially_relocatable_v<std::thread::id>);
static_assert(!std::is_trivially_relocatable_v<std::promise<T>>);
static_assert(!std::is_trivially_relocatable_v<std::promise<T&>>);
static_assert(!std::is_trivially_relocatable_v<std::promise<void>>);
static_assert(!std::is_trivially_relocatable_v<std::future<T>>);
static_assert(!std::is_trivially_relocatable_v<std::future<T&>>);
static_assert(!std::is_trivially_relocatable_v<std::future<void>>);
static_assert(!std::is_trivially_relocatable_v<std::shared_future<T>>);
static_assert(!std::is_trivially_relocatable_v<std::shared_future<T&>>);
static_assert(!std::is_trivially_relocatable_v<std::shared_future<void>>);
static_assert(!std::is_trivially_relocatable_v<std::function<void()>>);  // contains pointer into self
static_assert(!std::is_trivially_relocatable_v<std::packaged_task<T(T)>>);  // contains pointer into self
static_assert(!std::is_trivially_relocatable_v<std::packaged_task<T&(T)>>);  // contains pointer into self
static_assert(!std::is_trivially_relocatable_v<std::packaged_task<void(T)>>);  // contains pointer into self

// The following types are not move-constructible at all.
// Since P1144R7, that doesn't stop their being trivially relocatable.
// But let's not bother to test them.
static_assert(!std::is_relocatable_v<std::atomic<int>>);
static_assert(!std::is_relocatable_v<std::atomic_flag>);
static_assert(!std::is_relocatable_v<std::condition_variable>);
static_assert(!std::is_relocatable_v<std::condition_variable_any>);
static_assert(!std::is_relocatable_v<std::lock_guard<std::mutex>>);
static_assert(!std::is_relocatable_v<std::mutex>);
static_assert(!std::is_relocatable_v<std::scoped_lock<std::mutex>>);
static_assert(!std::is_relocatable_v<std::seed_seq>);
static_assert(!std::is_relocatable_v<std::shared_mutex>);

#if TEST_STD_VER >= 20
static_assert(!std::is_relocatable_v<std::barrier<>>);
static_assert(!std::is_relocatable_v<std::binary_semaphore>);
static_assert(!std::is_relocatable_v<std::counting_semaphore<3>>);
static_assert(!std::is_trivially_relocatable_v<std::jthread>);
static_assert(!std::is_relocatable_v<std::latch>);
static_assert(std::is_trivially_relocatable_v<std::partial_ordering>);
static_assert(std::is_trivially_relocatable_v<std::ranges::subrange<T*>>);
static_assert(std::is_trivially_relocatable_v<std::ranges::subrange<std::string::iterator>>);
static_assert(std::is_trivially_relocatable_v<std::source_location>);
static_assert(std::is_trivially_relocatable_v<std::span<T>>);
static_assert(std::is_trivially_relocatable_v<std::span<T, 3>>);
//static_assert(!std::is_relocatable_v<std::stop_callback<R>>);
//static_assert(std::is_trivially_relocatable_v<std::stop_source>);
//static_assert(std::is_trivially_relocatable_v<std::stop_token>);
static_assert(std::is_trivially_relocatable_v<std::strong_ordering>);
static_assert(std::is_trivially_relocatable_v<std::weak_ordering>);
#endif

#if TEST_STD_VER >= 23
static_assert(!std::is_trivially_relocatable_v<std::expected<T,R>>);
static_assert(!std::is_trivially_relocatable_v<std::expected<R,T>>);
static_assert(!std::is_trivially_relocatable_v<std::expected<R,R>>);
static_assert(!std::is_trivially_relocatable_v<std::expected<void,R>>);
static_assert(!std::is_trivially_relocatable_v<std::expected<void,T>>);
static_assert(!std::is_trivially_relocatable_v<std::unexpected<T>>);
static_assert(std::is_trivially_relocatable_v<std::unexpected<R>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_map<K,T>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_map<K,bool>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_map<bool,T>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_map<bool,bool>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_multimap<K,T>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_multimap<K,bool>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_multimap<bool,T>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_multimap<bool,bool>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_multiset<K>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_multiset<bool>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_set<K>>);
//static_assert(std::is_trivially_relocatable_v<std::flat_set<bool>>);
static_assert(!std::is_trivially_relocatable_v<std::move_only_function<T(T)>>);
static_assert(!std::is_trivially_relocatable_v<std::move_only_function<void()>>);
#endif
