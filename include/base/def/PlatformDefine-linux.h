/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once


#ifndef STRAW_FRAMEWORK_API

#define STRAW_FRAMEWORK_API
#define STRAW_TEMPLATE_API


#else
#undef STRAW_FRAMEWORK_API

#define STRAW_FRAMEWORK_API __attribute__((visibility("default")))
#define STRAW_TEMPLATE_API STRAW_FRAMEWORK_API

#endif // STRAW_FRAMEWORK_API
