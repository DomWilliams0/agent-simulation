#ifndef UTIL_H
#define UTIL_H

#define UNUSED(x) (void)(x)

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
		do { init_impl } while(0); \
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
