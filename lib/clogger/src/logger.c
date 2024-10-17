/*
    Simple C logger source file.
    Author: Julien Remmery
    Date: Tuesday, October 15, 2024
    Last modified: Tuesday, October 15, 2024
*/

#include "../include/logger.h"

/**
 * Log levels array used for logging.
 * Each level should match its index in LogLevel enum.
 * 
 * Example:
 *      enum LogLevel {
 *          INFO,
 *          WARN,
 *          ERROR
 *      };
 * 
 *      const char* log_levels[] = {"INFO", "WARN", "ERROR"};
 */
const char* log_levels[] = {"INFO", "WARN", "ERROR"};

void set_log_level(Logger* logger, enum LogLevel level) {
    logger->level = level;
}

void add_logfile(Logger* logger, FILE* file) {
    if(logger->files.file_count == 0) {
        logger->files.files_arr = malloc(sizeof(FILE*));
    } else {
        logger->files.files_arr = realloc(logger->files.files_arr, (logger->files.file_count + 1) * sizeof(FILE*));
    }
    logger->files.files_arr[logger->files.file_count] = file;
    logger->files.file_count++;
}

void close_logger_files(Logger* logger) {
    if (logger->files.files_arr != NULL) {
        for (int i = 0; i < logger->files.file_count; i++) {
            if (logger->files.files_arr[i] != NULL) {
                fclose(logger->files.files_arr[i]);
                logger->files.files_arr[i] = NULL;
            }
        }
        free(logger->files.files_arr);
        logger->files.files_arr = NULL;
    }
    logger->files.file_count = 0;
}

void logger(Logger* logger, char const * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if(logger->files.file_count == 0) {
        flogger(stdout, logger->level, fmt, ap);
    } else {
        for (int i = 0; i < logger->files.file_count; i++){
            va_list ap_copy;
            va_copy(ap_copy, ap);
            flogger(logger->files.files_arr[i], logger->level, fmt, ap_copy);
            va_end(ap_copy);
        }
    }
    va_end(ap);
}

void flogger(FILE* file, enum LogLevel level, char const * fmt, va_list ap) {
    fprintf(file, "[ %s ] ", log_levels[level]);
    vfprintf(file, fmt, ap);
    fprintf(file, "\n");
}