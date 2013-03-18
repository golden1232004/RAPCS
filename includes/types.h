//
//  types.h
//  
//
//  Created by Phillip Heels on 18/09/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <inttypes.h>
#ifndef _types_h
#define _types_h

template<unsigned int N> struct IntN;

template <> struct IntN< 8> {
    typedef uint8_t type;
};

template <> struct IntN<16> {
    typedef uint16_t type;
};

template <> struct IntN<32> {
    typedef uint32_t type;
};

template <> struct IntN<64> {

    typedef uint64_t type;
};


#endif
