#include <stdarg.h>
#include <stdio.h>

#include "unicode.h"

#define UC_MAX_ERR_LEN 255

static char last_error[UC_MAX_ERR_LEN] = {0};

const char *uc_last_error(void)
{
	return last_error;
}

void uc_set_error_(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(last_error, UC_MAX_ERR_LEN, fmt, ap);
	va_end(ap);
}

void uc_unset_error_(void)
{
	last_error[0] = '\0';
}
