/*
 *  Copyright 2018 Digital Media Professionals Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#ifndef UTIL_TIME_H
#define UTIL_TIME_H

#ifdef DMP_MEASURE_TIME

#define TVAL_START(name)  name ## _start_tv
#define TVAL_END(name)    name ## _end_tv

#define DECLARE_TVAL(name) \
  struct timeval TVAL_START(name); \
  struct timeval TVAL_END(name); \

#define GET_TVAL_START(name)  gettimeofday(&TVAL_START(name), NULL);
#define GET_TVAL_END(name)    gettimeofday(&TVAL_END(name), NULL);

#define SHOW_TIME(name) \
{ \
  struct timeval tv_sub; \
  timersub(&TVAL_END(name), &TVAL_START(name), &tv_sub); \
  printf(#name " time : %ld.%06ld (sec)\n", tv_sub.tv_sec, tv_sub.tv_usec); \
}
#define GET_SHOW_TVAL_END(name) GET_TVAL_END(name); SHOW_TIME(name)

#else

#define TVAL_START(name)
#define TVAL_END(name)
#define DECLARE_TVAL(name)
#define GET_TVAL_START(name)
#define GET_TVAL_END(name)
#define SHOW_TIME(name)
#define GET_SHOW_TVAL_END(name)

#endif // DMP_MEASURE_TIME
#endif // include guard
