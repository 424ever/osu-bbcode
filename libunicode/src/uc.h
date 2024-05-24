#include "unicode.h"

#ifndef UC_H
#define UC_H

struct uc_codepoint_
{
	int	 err;
	uint32_t code;
};

int uc_str_has_error_(const uc_codepoint *s);

#endif /* !UC_H */
