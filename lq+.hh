#include <vector>
#include <iostream>

namespace lq {
    template<typename T, typename NF, typename CF>
    class enumerable;
    
    template<typename T, typename NF, typename CF>
    enumerable<T, NF, CF> _make_enumerable(NF nf, CF cf) {
        return enumerable<T, NF, CF>(nf, cf);
    }
    
    template<typename T>
    class enumerable_base {
    public:
        virtual bool next() = 0;
        virtual T current() = 0;
        
        enumerable_base() {
            throw 1;
        }
        
        class iterator {
            bool end_iterator;
            bool ended;
            enumerable_base<T>& e;
        public:
            iterator(enumerable_base<T>& e, bool end_iterator = false) : e(e), ended(false), end_iterator(end_iterator) { }
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
    };
    
    template<typename T, typename NF, typename CF>
    class enumerable : public enumerable_base<T> {
        NF nf;
        CF cf;
    public:
        enumerable(NF nf, CF cf) : nf(nf), cf(cf) {
            std::cout << "ctor for enumerable" << std::endl;
        }
        
        virtual bool next() {
            return nf();
        }
        virtual T current() {
            return cf();
        }
        
        template<typename U>
        enumerable<T, NF, CF> where(U pred) {
            return _make_enumerable<T>([this, pred]() -> bool {
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
    enumerable_base<T> _from_iterable(U xs) {
        typename U::iterator iter = xs.begin();
        return _make_enumerable<T>([xs, &iter]() {
            return ++iter != xs.end();
        }, [xs, iter]() {
            return *iter;
        });
    }
    
    template<typename T>
    enumerable_base<T> from(std::initializer_list<T> xs) {
        return _from_iterable<T>(xs);
    }
};