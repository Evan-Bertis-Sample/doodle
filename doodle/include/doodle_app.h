#ifndef __DOODLE_APP_H__
#define __DOODLE_APP_H__

typedef doodle_app_desc_t {
    const char *name;
    const char *description;
    const char *version;
    const char *author;
    const char *url;
    const char *license;
}

typedef doodle_app {
    doodle_app_desc_t desc;
    void (*setup)(void);
    void (*loop)(void);
    void (*teardown)(void);
}


#ifdef DOODLE_MAIN
doodle_app_desc_t doodle_main(int32_t argc, char *argv[]);
#endif // DOODLE_MAIN

#endif // __DOODLE_APP_H__