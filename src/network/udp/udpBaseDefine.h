/*!
 * \file udpBaseDefine.h
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#ifndef _UDPBASEDEFINE_H_
#define _UDPBASEDEFINE_H_

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include "BaseDataDefine.h"

typedef std::shared_ptr<ByteVector> pByteVector;
typedef struct tagUdpData{//
	//boost::asio::ip::udp::endpoint fromPoint;
	lp_uint32 sin_addr;
	lp_uint32 sin_port;
	pByteVector _byteData;
}UDPData;

#endif
