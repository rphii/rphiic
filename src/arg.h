#ifndef ARG_H

#include "str.h"

#define ARG_INIT_DEFAULT_WIDTH      80
#define ARG_INIT_DEFAULT_PREFIX     '-'
#define ARG_INIT_DEFAULT_SHOW_HELP  true
#define ARG_INIT_DEFAULT_FLAG_SEP   ','
#define ARG_TYPE_F      FG_CY

/* types of arguments
 *
 * */

struct Arg;
struct ArgX;
struct TArgX;
struct ArgXGroup;

typedef int (*ArgFunction)(void *);

struct Arg *arg_new(void);
void arg_init(struct Arg *arg, Str program, Str description, Str epilog);
void arg_init_width(struct Arg *arg, int width, int percent);
void arg_init_show_help(struct Arg *arg, bool show_help);
void arg_init_prefix(struct Arg *arg, unsigned char prefix);
void arg_init_rest(struct Arg *arg, Str description, VStr *rest);
void arg_init_fmt(struct Arg *arg);

void arg_free(struct Arg **arg);

struct ArgXGroup *argx_group(struct Arg *arg, Str desc, bool explicit_help);
struct ArgX *argx_pos(struct Arg *arg, Str opt, Str desc);
struct ArgX *argx_init(struct ArgXGroup *group, const unsigned char c, const Str optX, const Str descX);
struct ArgX *argx_env(struct ArgXGroup *group, Str opt, Str desc, bool hide_value);

void argx_str(struct ArgX *x, Str *val, Str *ref);
void argx_col(struct ArgX *x, Color *val, Color *ref);
void argx_ssz(struct ArgX *x, ssize_t *val, ssize_t *ref);
void argx_int(struct ArgX *x, int *val, int *ref);
void argx_dbl(struct ArgX *x, double *val, double *ref);
void argx_bool(struct ArgX *x, bool *val, bool *ref);
void argx_bool_require_tf(struct ArgX *x, bool require_tf);
void argx_none(struct ArgX *x);
void argx_vstr(struct ArgX *x, VStr *val, VStr *ref);
struct ArgXGroup *argx_flag(struct ArgX *x);
struct ArgXGroup *argx_opt(struct ArgX *x, int *val, int *ref);
void argx_opt_enum(struct ArgX *x, int val);
void argx_flag_set(struct ArgX *x, bool *val, bool *ref);
void argx_type(struct ArgX *x, Str type);
void argx_func(struct ArgX *x, ssize_t priority, void *func, void *data, bool allow_compgen, bool quit_early);
void argx_hide_value(struct ArgX *x, bool hide_value);

struct ArgX *argx_get(struct ArgXGroup *group, Str opt);
//struct ArgX *argx_new(struct TArgX *group, const unsigned char c, const Str opt, const Str desc);

void arg_help_set(struct Arg *arg, struct ArgX *x);
int arg_help(struct Arg *arg);
void arg_config(struct Arg *arg, Str conf);

void argx_builtin_env_compgen(struct ArgXGroup *group);
void argx_builtin_opt_help(struct ArgXGroup *group);
struct ArgXGroup *argx_builtin_opt_rice(struct ArgXGroup *group);
void argx_builtin_opt_source(struct ArgXGroup *group, Str source);
void argx_builtin_opt_fmtx(struct ArgX *x, StrFmtX *fmt, StrFmtX *ref);


#if 0
Str arg_info_opt(struct Arg *arg, void *x);
#endif

#define ERR_arg_parse(...) "failed parsing arguments"
ErrDecl arg_parse(struct Arg *arg, const unsigned int argc, const char **argv, bool *quit_early);

#define ARG_H
#endif

