#include "unicode.h"

#ifndef UNICODE_UC_H
#define UNICODE_UC_H

#define RETURN_WITH_ERROR_SET(cp) \
	{                         \
		cp.err	= 1;      \
		cp.code = 0;      \
		return cp;        \
	}

struct uc_codepoint_
{
	int	 err;
	uint32_t code;
};

int uc_str_has_error_(const uc_codepoint *);

void uc_set_error_(const char *, ...);
void uc_unset_error_(void);

#endif /* !UNICODE_UC_H */
