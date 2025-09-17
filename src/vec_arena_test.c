#include "minishell.h"

void print_vec(t_vec *v)
{
    printf("[");
    for (size_t i = 0; i < v->len; i++)
    {
        char *c = (char *)vec_get(v, i);
        printf("'%c'", *c);
        if (i + 1 < v->len)
            printf(", ");
    }
    printf("]\n");
}

int vec_test(void)
{
    t_vec v1, v2;
    char tmp;

    // --- vec_new ---
    vec_new(&v1, 2);
    printf("Initial v1: ");
    print_vec(&v1);

    // --- vec_push ---
    char a = 'A', b = 'B', c = 'C', d = 'D';
    vec_push(&v1, &a);
    vec_push(&v1, &b);
    vec_push(&v1, &c);
    vec_push(&v1, &d);
    printf("After push A,B,C,D: ");
    print_vec(&v1);

    // --- vec_get ---
    char *second = (char *)vec_get(&v1, 1);
    printf("Second element: %c\n", *second);

    // --- vec_insert ---
    char x = 'X';
    vec_insert(&v1, &x, 2);
    printf("After insert 'X' at index 2: ");
    print_vec(&v1);

    // --- vec_remove ---
    vec_remove(&v1, 2);
    printf("After remove index 2: ");
    print_vec(&v1);

    // --- vec_pop ---
    vec_pop(&tmp, &v1);
    printf("Popped value: %c\n", tmp);
    printf("After pop: ");
    print_vec(&v1);

    // --- vec_from ---
    vec_from(&v2, "HELLO", 5);
    printf("v2 from 'HELLO': ");
    print_vec(&v2);

    // --- vec_append ---
    vec_append(&v1, &v2);
    printf("After append v2 to v1: ");
    print_vec(&v1);

    // --- vec_prepend ---
    vec_prepend(&v1, &v2);
    printf("After prepend v2 to v1: ");
    print_vec(&v1);

    // --- free ---
    vec_free(&v1);
    vec_free(&v2);

    return 0;
}

int arena_vec_test_(void)
{
    t_arena arena;
    t_vec   tokens;

    if (!arena_init(&arena, 1024))
    {
        printf("arena init failed\n");
        return (1);
    }
    if (vec_new(&tokens, 4) == -1)
    {
        printf("vec init failed\n");
        return (1);
    }

    char *s1 = arena_push(&arena, "echo");
    char *s2 = arena_push(&arena, "hello");
    char *s3 = arena_push(&arena, "world");

    vec_push(&tokens, s1);
    vec_push(&tokens, s2);
    vec_push(&tokens, s3);

    size_t i = 0;
    while (i < tokens.len)
    {
        char *tok = vec_get(&tokens, i);
        printf("token[%zu] = %s\n", i, tok);
        i++;
    }

	vec_free(&tokens);
    arena_free(&arena);
    return (0);
}
