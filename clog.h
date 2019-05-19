#ifndef CLOG_H
#define CLOG_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

enum clog_level { CLOG_INFO, CLOG_WARN, CLOG_ERROR, CLOG_FATAL };

typedef struct {
  enum clog_level level;
  const char *tag;
  const char *msg;
} clog_item;

void clog_enable();

void clog_disable();

/*!
 * \brief - Change the destination for log messages
 * \param output_file - The FILE * to where the output will go to
 */
void clog_out(FILE *);

/*!
 * \brief - Log message as information
 * \param tag - The tag or context of the log
 * \param msg - The message to be printed
 */
void log_inf(const char *tag, const char *, ...);

/*!
 * \brief - Log message as warning
 * \param tag - The tag or context of the log
 * \param msg - The message to be printed
 */
void log_war(const char *, const char *, ...);

/*!
 * \brief - Log message as error
 * \param tag - The tag or context of the log
 * \param msg - The message to be printed
 */
void log_err(const char *, const char *, ...);

/*!
 * \brief - Log message as error but get error message from errno
 * \param tag - The tag or context of the log
 * \param msg - The message to be printed
 */
void log_per(const char *, const char *, ...);

/*!
 * \brief - Log message as fatal error
 * \param tag - The tag or context of the log
 * \param msg - The message to be printed
 */
void log_fat(const char *, const char *, ...);

#ifdef __cplusplus
}
#endif
#endif
