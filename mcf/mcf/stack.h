#ifndef MCF_STACK
#define MCF_STACK

#include <ccan/compiler/compiler.h>
#include <ccan/tal/tal.h>

/* Generic and efficient stack based on ccan arrays.
 * FIXME: Reserve grows linearly, maybe it should double its size avoid the copy
 * overhead, but maybe tal_resize already does that.
 * ~1.2 operations/nsec */

#define STACK_CACHE_SIZE 64

#define STACK_DEFINE_TYPE(type, name)                                          \
	struct name {                                                          \
		size_t reserved;                                               \
		size_t len;                                                    \
		type *data;                                                    \
	};                                                                     \
	static inline UNNEEDED bool name##_empty(const struct name *s)         \
	{                                                                      \
		return s->len == 0;                                            \
	}                                                                      \
	static inline UNNEEDED type name##_top(const struct name *s)           \
	{                                                                      \
		return s->data[s->len - 1];                                    \
	}                                                                      \
	static inline UNNEEDED type name##_pop(struct name *s)                 \
	{                                                                      \
		type r;                                                        \
		if (s->len) {                                                  \
			r = s->data[s->len - 1];                               \
			s->len--;                                              \
			if (s->len + 2 * STACK_CACHE_SIZE <= s->reserved) {    \
				s->reserved -= STACK_CACHE_SIZE;               \
				tal_resize(&s->data, s->reserved);             \
			}                                                      \
		} else                                                         \
			r = s->data[0];                                        \
		return r;                                                      \
	}                                                                      \
	static inline UNNEEDED void name##_init(struct name *s,                \
						const tal_t *ctx)              \
	{                                                                      \
		s->reserved = STACK_CACHE_SIZE;                                \
		s->len = 0;                                                    \
		s->data = tal_arr(ctx, type, s->reserved);                     \
	}                                                                      \
	static inline UNNEEDED void name##_insert(struct name *s,              \
						  type element)                \
	{                                                                      \
		s->len++;                                                      \
		if (s->len >= s->reserved) {                                   \
			s->reserved += STACK_CACHE_SIZE;                       \
			tal_resize(&s->data, s->reserved);                     \
		}                                                              \
		s->data[s->len - 1] = element;                                 \
	}                                                                      \
	/* STACK_DEFINE_TYPE */

#endif /* MCF_STACK */

