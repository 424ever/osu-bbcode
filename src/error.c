#include <stdarg.h>
#include <stdio.h>

#include "unicode.h"

#define UC_MAX_ERR_LEN 255

static char last_error[UC_MAX_ERR_LEN] = {0};

const char *get_error(void)
{
	return last_error;
}

int error_occured(void)
{
	return last_error[0] != '\0';
}

void report_error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(last_error, UC_MAX_ERR_LEN, fmt, ap);
	va_end(ap);
}

void unset_error(void)
{
	last_error[0] = '\0';
}
