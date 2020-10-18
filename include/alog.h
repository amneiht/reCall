/*
 * log.h
 *
 *  Created on: Oct 5, 2020
 *      Author: dccan
 */

#ifndef ALOG_H_
#define ALOG_H_
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//
// Khi không cần in ra log nữa ta đơn giản định nghĩa macro NDEBUG (tức là No Debug) bằng dòng bên dưới.
// Khi biên dich, các đoạn mã trống do {} while(0); sẽ bị xóa hết do tối ưu.
// Vì thế hầu như không ảnh hưởng đến code chính.
//#define NDEBUG (1)

#ifdef NDEBUG
#define log_debug(fmt, ...)  do {} while (0)
#define log_err(fmt, ...)  do {} while (0)
#define log_warn(fmt, ...)  do {} while (0)
#define log_info(fmt, ...)  do {} while (0)
#else // NDEBUG

//
// Dành cho việc in ra log debug
//
#define log_debug(fmt, ...)\
do { fprintf(stderr, "[DEBUG] %10s:%4d:%15s():\t" fmt, __FILE__, __LINE__, __func__,## __VA_ARGS__); \
fprintf(stderr,"\n"); \
} while (0)

//
// Dành cho việc  in ra log lỗi
//
#define log_err(fmt, ...) \
        do { fprintf(stderr, "[ERROR] %10s:%4d:%15s():\t" fmt, __FILE__,  __LINE__, __func__,## __VA_ARGS__);\
fprintf(stderr,"\n"); \
} while (0)

//
// Dành cho việc in ra log lỗi
//
#define log_warn(fmt, ...) \
        do { fprintf(stderr, "[WARN] %10s:%4d:%15s():\t" fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); \
fprintf(stderr,"\n"); \
        } while (0)
//
// Dành cho việc in ra các thông tin khác.
//
#define log_info(fmt, ...) \
        do { \
        	fprintf(stderr, "[INFO] %10s:%4d:%15s():\t" fmt, __FILE__, \
            __LINE__, __func__, ##__VA_ARGS__); \
       fprintf(stderr,"\n"); \
}while (0)
#endif //NDEBUG

#endif /* ALOG_H_ */
