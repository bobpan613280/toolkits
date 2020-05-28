/*!
 * \file BaseDataDefine.h
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note base type define
*/
#ifndef _DATAGRAM_H_
#define _DATAGRAM_H_
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

typedef unsigned char	    lp_uint8; //1bytes
typedef unsigned short		lp_uint16;//2bytes
typedef unsigned int		lp_uint32;//4bytes
//windows
typedef unsigned long  long int	lp_uint64;//8bytes
//ubuntu
//typedef unsigned long long 	lp_uint64;//8bytes
typedef signed char			lp_int8; //1bytes
typedef signed short		lp_int16;//2bytes
typedef signed int			lp_int32;//4bytes

typedef signed long int		lp_int64;//8bytes

typedef float				lp_float32;
typedef double				lp_float64;

/*
typedef std::uint8_t	    lp_uint8; //1bytes
typedef std::uint16_t		lp_uint16;//2bytes
typedef std::uint32_t		lp_uint32;//4bytes

typedef std::uint64_t 		lp_uint64;//8bytes
typedef std::int8_t			lp_int8; //1bytes
typedef std::int16_t		lp_int16;//2bytes
typedef std::int32_t			lp_int32;//4bytes

typedef std::int64_t		lp_int64;//8bytes
typedef float				lp_float32;
typedef double				lp_float64;*/

typedef std::vector<lp_uint8> ByteVector;


#define WINDOWS
//#define ARM
#endif

