#ifndef ERROR_H
#define ERROR_H
const char *get_error(void);
int	    error_occured(void);
void	    report_error(const char *fmt, ...);
void	    unset_error(void);
#endif
