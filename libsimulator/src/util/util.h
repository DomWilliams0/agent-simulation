#ifndef SIM_UTIL_H
#define SIM_UTIL_H

#define UNUSED(x) (void)(x)

#define MACRO_CONCAT_IMPL(a, b) a##b
#define MACRO_CONCAT(a, b) MACRO_CONCAT_IMPL(a, b)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// declaration
#define _MOD_DECLARE_FUNCS(name) \
    int name ## _init(struct name *self, void *arg); \
    void name ## _destroy(struct name *self);

#define MOD_DECLARE(name, impl) \
    struct name impl; \
	_MOD_DECLARE_FUNCS(name)

#define MOD_FWD_DECLARE(name) \
    struct name; \
	size_t name ## _sizeof(); \
	_MOD_DECLARE_FUNCS(name)

// implementation
#define MOD_INIT(name, impl) \
    int name ## _init(struct name *self, void *arg) { UNUSED(self); UNUSED(arg); impl } \
	size_t name ## _sizeof() { return sizeof(struct name); }


#define MOD_DESTROY(name, impl) \
    void name ## _destroy(struct name *self) { UNUSED(self); impl }

void random_init();
double random_get(double max);

#ifndef M_PI
// Source: http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406
#endif


#endif
