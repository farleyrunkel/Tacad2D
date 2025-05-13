// Copyright [2024] SunCAD

#ifndef _SIGNAL_Header_File_
#define _SIGNAL_Header_File_

#include <boost/signals2.hpp>

template<typename T>
using Signal = boost::signals2::signal<T>;

template<typename T>
using SignalConnection = boost::signals2::connection;

#endif  // _SIGNAL_Header_File_
