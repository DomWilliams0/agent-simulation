#ifndef UTIL_H
#define UTIL_H

#define UNUSED(x) (void)(x)

#define MACRO_CONCAT_IMPL(a, b) a##b
#define MACRO_CONCAT(a, b) MACRO_CONCAT_IMPL(a, b)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// forward declare
// header file init and destroy functions
// source file init { log; declare new_; allocate; ...init...; return new_; }
// source file destroy {log; ...destroy...; safe_free}

// header file
#define MODULE_DECLARE(struct_type, init_func_name, destroy_func_name) \
	/* forward declare */ \
	struct_type; \
	/* init */ \
	struct_type *init_func_name(void *arg); \
	/* destroy */ \
	void destroy_func_name(struct_type *instance);

#define MODULE_INIT_ABORT break

// implementation
// TODO upgrade void* arg to va_args if necessary
#define MODULE_IMPLEMENT(struct_type, friendly_name, \
		init_func_name, init_impl, \
		destroy_func_name, destroy_impl) \
	/* init */ \
	struct_type *init_func_name(void *arg) \
	{ \
		UNUSED(arg); \
		LOG_DEBUG("Creating new " friendly_name); \
		struct_type *new_instance; \
		safe_malloc_struct(struct_type, &new_instance); \
		int great_success = 0; \
		do { init_impl; great_success = 1; } while(0); \
		if (great_success == 0) \
		{ \
			LOG_DEBUG("Aborting creation of " friendly_name); \
			destroy_func_name(new_instance); \
			return NULL; \
		} \
		return new_instance; \
	} \
	/* destroy */ \
	void destroy_func_name(struct_type *instance) \
	{ \
		if (!instance) return; \
		LOG_DEBUG("Destroying " friendly_name); \
		do { destroy_impl } while(0); \
		safe_free(instance); \
	} \

#endif

void random_init();
double random_get(double max);
