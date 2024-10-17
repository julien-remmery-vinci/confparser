/*
    Simple C logger header file.
    Author: Julien Remmery
    Date: Tuesday, October 15, 2024
    Last modified: Tuesday, October 15, 2024
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/**
 * Enum:    LogLevel
 * -----------------
 * Enumerate all available log levels.
 * 
 * Each level starts with LOG_ followed by its severity.
 */
enum LogLevel {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};

/**
 * Struct:    LogFiles
 * -------------------
 * Struct containing the output file/s of a logger.
 * 
 * - files_arr : array of the files to log to
 * - file_count : count of files in the array
 */
typedef struct {
    FILE** files_arr;
    int file_count;
} LogFiles;

/**
 * Struct:    Logger
 * -----------------
 * Struct containing the log level and output file/s of a logger.
 * 
 * - level : logger's log level
 * - files : struct containing the output file/s of a logger.
 */
typedef struct {
    enum LogLevel level;
    LogFiles files;
} Logger;

/**
 * Function:    set_log_level
 * --------------------------
 * Set log level of a logger.
 * 
 * - logger : logger to set log level for
 * - level : log level to set logger to
 */
void set_log_level(Logger* logger, enum LogLevel level);

/**
 * Function:    add_logfile
 * ------------------------
 * Add an output file to a logger
 * 
 * - logger : logger to add file to
 * - file : file to add to logger
 */
void add_logfile(Logger* logger, FILE* file);

/**
 * Function:    close_logger_files
 * -------------------------------
 * Close all files associated to a logger.
 */
void close_logger_files(Logger* logger);

/**
 * Function:    logger
 * --------------------
 * Uses flogger function to log using provided logger.
 * 
 * - logger : Logger set to a valid log level
 * - fmt : formated string to log
 * - ... : values to fill string formating
 * 
 * If logger's output file count is 0, default log to stdout.
 */
void logger(Logger* logger, char const * fmt, ...);

/**
 * Function:    flogger
 * --------------------
 * Log to a specific output using provided log level.
 * 
 * - file : output fiel to log to
 * - level : log level to use for logging
 * - fmt : formated string to log
 * - ... : values to fill string formating
 */
void flogger(FILE* file, enum LogLevel level, char const * fmt, va_list ap);


#endif // LOGGER_H