#include "../src/arg.h"
#include "../src/file.h"
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

typedef enum {
    CONFIG_NONE,
    CONFIG_PRINT,
    CONFIG_BROWSER,
    CONFIG_LMAO,
} ConfigList;

typedef enum {
    CONFIG_MODE_NONE,
    CONFIG_MODE_HELLO,
    CONFIG_MODE_INT,
    CONFIG_MODE_FLOAT,
    CONFIG_MODE_STRING,
    CONFIG_MODE_BOOL,
    CONFIG_MODE_STRINGS,
} ConfigModeList;

typedef struct Config {
    ssize_t whole;
    bool boolean;
    double number;
    Str2 config;
    Str2 string;
    VStr2 strings;
    ConfigList id;
    ConfigList id2;
    ConfigList id3;
    struct {
        ConfigModeList id;
        ssize_t z;
        Str2 s;
        double f;
        bool b;
        VrStr v;
    } mode;
    struct {
        bool safe;
        bool unsafe;
        bool other;
    } flags;
} Config;

#define TEST(msg)   do { \
        printf(F("=== %s ===", BOLD FG_BL_B) "\n", #msg); \
        arg_free(&arg); \
        TRYC(main_arg(&arg, argc, argv)); \
    } while(0)

#define TEST_ALL        0
#if TEST_ALL
#define TEST_EMPTY_LONG 1
#define TEST_TWIN_LONG  1
#define TEST_TWIN_SHORT 1
#endif

int hello_world(int *n) {
    printf("Hello, %i worlds!\n", *n);
    return 0;
}

#include <sys/ioctl.h>
#include <poll.h> // int poll(struct pollfd *fds, nfds_t nfds, int timeout);

#include <errno.h>
#include <unistd.h>

int main(const int argc, const char **argv) {

    int err = 0;
    size_t n_arg = 0;
    Config config = {0};
    Config preset = {0};
    Str2 *rest2 = {0};
    Str2 *files = {0};
    Str2 configuration = {0};
    struct Arg *arg = arg_new();
    struct ArgX *x;
    struct ArgXGroup *g;
    ssize_t nfuck = 0;
    bool quit_early = false;

    preset.flags.other = true;
    preset.flags.safe = true;
    preset.id = CONFIG_LMAO;
    preset.config = str2("path/to/config/that-is-very-long-and-unnecessary");

    /* set up arguments {{{*/

    arg_init(arg, str2("test_arg"), str2("this is a test program to verify the functionality of an argument parser. also, this is a very very long and boring description, just so I can check whether or not it wraps and end correctly! isn't that fascinating..."), str2("github: https://github.com/rphii"));
    arg_init_rest(arg, str2("files"), &files);
    arg_init_width(arg, 40, 45);
    //arg_init_width(arg, 0, 45);

    x=argx_init(arg_opt(arg), 'h', str2("help"), str2("print this help"));
      argx_help(x, arg);
    x=argx_init(arg_opt(arg), 0, str2("xyz"), str2("nothing"));
    x=argx_init(arg_opt(arg), 'b', str2("bool"), str2("boolean value and a long description that is"));
      argx_bool(x, &config.boolean, &preset.boolean);
    x=argx_init(arg_opt(arg), 'f', str2("double"), str2("double value"));
      argx_dbl(x, &config.number, &preset.number);
    x=argx_init(arg_opt(arg), 's', str2("string"), str2("string value"));
      argx_str(x, &config.string, &preset.string);
    x=argx_init(arg_opt(arg), 'i', str2("integer"), str2("integer value"));
      argx_int(x, (int *)&config.whole, (int *)&preset.whole);
    x=argx_init(arg_opt(arg), 'o', str2("option"), str2("select one option"));
      g=argx_opt(x, (int *)&config.id, (int *)&preset.id);
        x=argx_init(g, 0, str2("none"), str2("do nothing"));
          argx_opt_enum(x, CONFIG_NONE);
        x=argx_init(g, 0, str2("print"), str2("print stuff"));
          argx_opt_enum(x, CONFIG_PRINT);
        x=argx_init(g, 0, str2("browser"), str2("browse stuff"));
          argx_opt_enum(x, CONFIG_BROWSER);
        x=argx_init(g, 0, str2("lmao"), str2("what the fuck"));
          argx_opt_enum(x, CONFIG_LMAO);
          argx_ssz(x, &nfuck, 0);
          argx_func(x, ++n_arg, hello_world, &nfuck, false);
        x=argx_init(g, 0, str2("test"), str2("what the fuck"));
          argx_opt_enum(x, 6);
        x=argx_init(g, 0, str2("useless"), str2("what the fuck"));
          argx_opt_enum(x, 7);
        x=argx_init(g, 0, str2("verbose"), str2("what the fuck"));
          argx_opt_enum(x, 8);
    //x=argx_init(arg_opt(arg), n_arg++, 0, str2("very-long-option-that-is-very-important-and-cool-but-serves-no-purpose-whatsoever-anyways-how-are-you-doing-today"), str2("select another option"));
    x=argx_init(arg_opt(arg), 'F', str2("flags"), str2("set different flags"));
      g=argx_flag(x);
        x=argx_init(g, 0, str2("safe"), str2("enable safe operation"));
          argx_flag_set(x, &config.flags.safe, &preset.flags.safe);
        x=argx_init(g, 0, str2("unsafe"), str2("enable unsafe operation"));
          argx_flag_set(x, &config.flags.unsafe, &preset.flags.unsafe);
        x=argx_init(g, 0, str2("other"), str2("enable other operation"));
          argx_flag_set(x, &config.flags.other, &preset.flags.other);

    x=argx_pos(arg, str2("mode"), str2("the main mode"));
      g=argx_opt(x, (int *)&config.mode.id, (int *)&preset.mode.id);
        x=argx_init(g, 0, str2("none"), str2("do nothing"));
          argx_opt_enum(x, CONFIG_MODE_NONE);
        x=argx_init(g, 0, str2("hello"), str2("print hello"));
          argx_func(x, ++n_arg, hello_world, &nfuck, true);
          argx_opt_enum(x, CONFIG_MODE_HELLO);
        x=argx_init(g, 0, str2("int"), str2("set int"));
          argx_ssz(x, &config.mode.z, &preset.mode.z);
          argx_opt_enum(x, CONFIG_MODE_INT);
        x=argx_init(g, 0, str2("float"), str2("set float"));
          argx_dbl(x, &config.mode.f, &preset.mode.f);
          argx_opt_enum(x, CONFIG_MODE_FLOAT);
        x=argx_init(g, 0, str2("string"), str2("set string"));
          argx_str(x, &config.mode.s, &preset.mode.s);
          argx_opt_enum(x, CONFIG_MODE_STRING);
        x=argx_init(g, 0, str2("bool"), str2("set bool"));
          argx_bool(x, &config.mode.b, &preset.mode.b);
          argx_opt_enum(x, CONFIG_MODE_BOOL);
        x=argx_init(g, 0, str2("strings"), str2("set strings"));
          argx_vstr(x, &rest2, 0);
          argx_opt_enum(x, CONFIG_MODE_STRINGS);

    x=argx_init(arg_opt(arg), 'I', str2("input"), str2("input files"));
      argx_vstr(x, &config.strings, &preset.strings);
      argx_type(x, str2("input-files"));

    argx_env(arg, str2("ARG_CONFIG_PATH"), str2("config path"), &config.config, &preset.config, false);
    /*}}}*/

    /* load config {{{ */
    Str2 filename = str2("test_arg.conf");
    TRYC(file_str_read(filename, &configuration));
    arg_config(arg, str2_ll(configuration.str, str2_len(configuration)));
    /*}}}*/

#if 0
    sleep(1);
    Str input = {0};
    int n = 0;
    while (ioctl(0, FIONREAD, &n) ? 0 : n > 0) {
    //while(true) {
        str_clear(&input);
        str_get_str(&input);
        if(!str_length(input)) continue;
        printff(F(">>> %.*s", BG_YL_B FG_BK), STR_F(input));
    }
    return 0;
#endif

    TRYC(arg_parse(arg, argc, argv, &quit_early));
    if(quit_early) goto clean;


    /* post arg parse {{{ */
    printf("quit? %s\n", quit_early ? "yes" : "no");
    printf("INPUT-FILES:\n");
    for(size_t i = 0; i < array_len(config.strings); ++i) {
        printf(" %.*s\n", STR2_F(array_at(config.strings, i)));
    }
    printf("FILES:\n");
    for(size_t i = 0; i < array_len(files); ++i) {
        printf(" %.*s\n", STR2_F(array_at(files, i)));
    }
    printf("STRINGS:\n");
    for(size_t i = 0; i < array_len(rest2); ++i) {
        printf(" %.*s\n", STR2_F(array_at(rest2, i)));
    }
    /*}}}*/

clean:
    str2_free(&configuration);
    arg_free(&arg);
    return err;

error:
    ERR_CLEAN;
}

