#ifndef ARG_H

#include "str.h"

/* types of arguments
 *
 * */

struct Arg;
struct ArgX;
struct TArgX;
struct ArgXGroup;

typedef int (*ArgFunction)(void *);

struct Arg *arg_new(void);
void arg_init(struct Arg *arg, const int argc, const char **argv, RStr program, RStr description, RStr epilog, unsigned char prefix, bool show_help, VrStr *rest);
void arg_free(struct Arg **arg);

struct ArgXGroup *arg_pos(struct Arg *arg);
struct ArgXGroup *arg_opt(struct Arg *arg);
struct ArgXGroup *arg_env(struct Arg *arg);

struct ArgX *argx_init(struct ArgXGroup *group, size_t index, const unsigned char c, const RStr optX, const RStr descX);
void argx_str(struct ArgX *x, RStr *val, RStr *ref);
void argx_int(struct ArgX *x, ssize_t *val, ssize_t *ref);
void argx_dbl(struct ArgX *x, double *val, double *ref);
void argx_bool(struct ArgX *x, bool *val, bool *ref);
void argx_none(struct ArgX *x);
struct ArgXGroup *argx_opt(struct ArgX *x, void *val, void *ref);
void argx_opt_enum(struct ArgX *x, int val);
void argx_func(struct ArgX *x, void *func, void *data, bool quit_early);
void argx_help(struct ArgX *x, struct Arg *arg);

struct ArgX *argx_new(struct TArgX *group, const unsigned char c, const RStr opt, const RStr desc);

int arg_help(struct Arg *arg);

#define ERR_arg_parse(...) "failed parsing arguments"
ErrDecl arg_parse(struct Arg *arg, bool *quit_early);

#define ARG_H
#endif

