#include <vector>
#include <functional>

namespace lq {
    template<typename T>
    class enumerable {
        std::function<bool()> next_func;
        std::function<T()> current_func;
    public:
        bool next() {
            return next_func();
        }
        T current() {
            return current_func();
        }
        
        enumerable(std::function<bool()> next_func, std::function<T()> current_func)
            : next_func(next_func), current_func(current_func)
        { }
        
        class iterator {
            bool end_iterator;
            bool ended;
            enumerable<T>& e;
        public:
            iterator(enumerable<T>& e, bool end_iterator = false) : e(e), ended(false), end_iterator(end_iterator) { }
            iterator& operator++() {
                if(!ended) {
                    ended = e.next();
                }
                return *this;
            }
            T operator*() const {
                return e.current();
            }
            bool operator==(iterator& iter) {
                if(end_iterator) {
                    return ended;
                } else {
                    return false;
                }
            }
            bool operator!=(iterator& iter) {
                return !(*this == iter);
            }
        };
        
        iterator begin() {
            return iterator(*this);
        }
        iterator end() {
            return iterator(*this, true);
        }
        
        template<typename U>
        enumerable<T> where(U pred) {
            return enumerable<T>([this, pred]() -> bool {
                if(!next()) return false;
                while(!pred(current())) {
                    if(!next()) return false;
                }
                return true;
            }, [this]() {
                return current();
            });
        }
    };
    
    template<typename T, typename U>
    enumerable<T> _from_iterable(U xs) {
        typename U::iterator iter = xs.begin();
        return enumerable<T>([xs, &iter]() {
            return ++iter != xs.end();
        }, [xs, &iter]() {
            return *iter;
        });
    }
    
    template<typename T>
    enumerable<T> from(std::initializer_list<T> xs) {
        return _from_iterable<T>(xs);
    }
};