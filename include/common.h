//
// Created by Jonas Langner on 27.07.23.
//
#ifndef CHESS_COMMON_H
#define CHESS_COMMON_H

namespace common {

    template<typename T1, typename T2>
    bool instanceof_ptr(T2* t2_ptr) {
        return dynamic_cast<T1>(t2_ptr);
    }

    template<typename T1, typename T2>
    bool instanceof(T2 &t2) {
        return instanceof_ptr<T1*>(&t2);
    }

    template<typename Iterator, typename T>
    bool contains(Iterator const &first, Iterator const &last, T const &t) {
        Iterator iterator{first};
        for (;iterator!=last;++iterator) {
            auto const current{*iterator};

            if (current == t) {
                return true;
            }
        }
        return false;
    }

    template<typename C, typename T>
    bool contains(C const &c, T const &t) {
        return contains(c.begin(), c.end(), t);
    }
}

#endif //CHESS_COMMON_H
