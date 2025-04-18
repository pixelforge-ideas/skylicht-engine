/*
!@
MIT License

Copyright (c) 2024 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/

#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

#ifdef _SKYLICHT_STATIC_LIB_
	#define SKYLICHT_API
	#define SKYLICHT_EXPORT
#else

#ifdef _MSC_VER 
	// Visual studio
	#define SKYLICHT_API
	#ifdef SKYLICHT_EXPORTS
	#define SKYLICHT_EXPORT __declspec(dllexport)
	#else
	#define SKYLICHT_EXPORT __declspec(dllimport)
	#endif

#else
	// Cygwin & MinGW
	#ifdef SKYLICHT_EXPORTS
		#define SKYLICHT_API __declspec(dllexport)
		#define SKYLICHT_EXPORT __declspec(dllexport)
	#else
		#define SKYLICHT_API __declspec(dllimport)
		#define SKYLICHT_EXPORT __declspec(dllimport)
	#endif
#endif

#endif

#else

// GCC or OTHER
// Force symbol export in shared libraries built with gcc.
#if (__GNUC__ >= 4) && !defined(_SKYLICHT_STATIC_LIB_) && defined(SKYLICHT_EXPORTS)
	#define SKYLICHT_API __attribute__ ((visibility("default")))
	#define SKYLICHT_EXPORT __attribute__ ((visibility("default")))
#else
	#define SKYLICHT_API
	#define SKYLICHT_EXPORT
#endif

#endif