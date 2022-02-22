#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h>
#include <valgrind/valgrind.h>

#define STACK_SIZE   (8*1024)

int n_ucs = 1;
int max_switchs = 10;
int n_switchs = 0;
int tid = 0;

ucontext_t *ucs;
static ucontext_t engine_uc;

 void func(int arg)
{
    while (n_switchs < max_switchs) {
        int c_tid = tid;
        int n_tid = (tid + 1) % n_ucs;
        n_switchs++;
        tid = n_tid;
        swapcontext(&ucs[c_tid], &ucs[n_tid]);

    }
}

int main(int argc, char **argv)
{
    if (argc > 1)
        n_ucs = atoi(argv[1]);
    if (argc > 2)
        max_switchs = atoi(argv[2]);

    ucs = (ucontext_t *) malloc(sizeof(ucontext_t) * n_ucs);

    //store the VALGRIND_STACK_REGISTER return values
    int* valgrind_ret = (int*) malloc(n_ucs*sizeof(int));

    int i;
    for (i = 0; i < n_ucs; i++) {
        /* Create initial ucontext_t, including stack */
        getcontext(&ucs[i]);

        //pass stack to valgrind
        void* mystack = malloc(STACK_SIZE);
        //VALGRIND_STACK_REGISTER(mystack, ( ( u_int64_t ) mystack + STACK_SIZE  ));

        ucs[i].uc_stack.ss_sp = mystack;
        ucs[i].uc_stack.ss_size = STACK_SIZE;
        ucs[i].uc_stack.ss_flags = 0;
        ucs[i].uc_link = &engine_uc;
        makecontext(&ucs[i], (void (*)())func, 1, i);
    }

    /* jump to the first uc */
    swapcontext(&engine_uc, &ucs[tid]);

    /* destroy stacks */
    for (i = 0; i < n_ucs; i++) {
        //valgrind stack deregister
        //VALGRIND_STACK_DEREGISTER(valgrind_ret[i]);

        free(ucs[i].uc_stack.ss_sp);
    }
    free(ucs);
    return 0;
}
