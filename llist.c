/*
    ███    █▄      ███      ▄█   ▄█        ▄█      ███      ▄█     ▄████████    ▄████████ 
    ███    ███ ▀█████████▄ ███  ███       ███  ▀█████████▄ ███    ███    ███   ███    ███ 
    ███    ███    ▀███▀▀██ ███▌ ███       ███▌    ▀███▀▀██ ███▌   ███    █▀    ███    █▀  
    ███    ███     ███   ▀ ███▌ ███       ███▌     ███   ▀ ███▌  ▄███▄▄▄       ███        
    ███    ███     ███     ███▌ ███       ███▌     ███     ███▌ ▀▀███▀▀▀     ▀███████████ 
    ███    ███     ███     ███  ███       ███      ███     ███    ███    █▄           ███ 
    ███    ███     ███     ███  ███▌    ▄ ███      ███     ███    ███    ███    ▄█    ███ 
    ████████▀     ▄████▀   █▀   █████▄▄██ █▀      ▄████▀   █▀     ██████████  ▄████████▀  
                            ▀                                                         
    Niccolò Boano                                                               2020 - 2023
    ╔═════════════════════════════════════════════════════════════════════════════════════╗
    ║                                      LLIST.C                                        ║
    ║   Gestione di una linked list di tipo qualunque.                                    ║
    ║   Implementazione inserimento, cancellazione, ricerca, ordinamento, iterazione.     ║
    ║                                                                                     ║
    ║   LLIST_DECLTYPE(Tipo);        // (FUORI DAL MAIN)                                  ║
    ║   LLIST_INIT(mialst, Tipo);    // (FUORI DAL MAIN)                                  ║
    ╚═════════════════════════════════════════════════════════════════════════════════════╝
*/


#define LLIST_DECLTYPE(T)\
struct llitem_##T {T fields; struct llitem_##T* next;};                 \
typedef struct t_LList_##T {                                 \
    struct llitem_##T* Head;                                    \
    void(*Prepend)(T new);            \
    void(*Append)(T new);\
    int(*Count)();                              \
    void(*Each)(void(*)(T*));   \
    T* (*At)(int index);\
    T* (*Find)(int(*checkfn)(T));\
    int (*FindIndex)(int(*checkfn)(T));\
    void (*RemoveAt)(int index);\
    void (*InsertAt)(int index, T element);\
    void (*Sort)(int(*comparefn)(T a, T b));\
} LinkedList_##T;\

#define LLIST_INIT(listname, T)                    \
LinkedList_##T listname;                                              \
                                                \
void PREPEND_##listname(T new) {            \
    struct llitem_##T* newhead = (struct llitem_##T*)malloc(sizeof(struct llitem_##T));         \
    newhead->fields = new;                              \
    newhead->next = listname.Head;                      \
    listname.Head = newhead;                            \
}                                               \
void APPEND_##listname(T new) {\
    struct llitem_##T* newptr = (struct llitem_##T*)malloc(sizeof(struct llitem_##T));\
    newptr->fields = new;                              \
    newptr->next = NULL;                      \
    struct llitem_##T* ptr = listname.Head;                         \
    if(ptr == NULL) {listname.Head = newptr; return;}\
    while(ptr->next != NULL) {                        \
        ptr = ptr->next;                                 \
    }                                           \
    ptr->next = newptr;\
}\
int COUNT_##listname() {                              \
    int n = 0;                                  \
    struct llitem_##T* ptr = listname.Head;                         \
    while(ptr != NULL) {                        \
        ptr = ptr->next;                        \
        n++;                                    \
    }                                           \
    return n;                                   \
}                                               \
void EACH_##listname(void(*handler)(T*)) {                      \
    struct llitem_##T* ptr = listname.Head;\
    while(ptr != NULL) {                        \
        handler(&ptr->fields);\
        ptr = ptr->next;    \
    };\
}\
struct llitem_##T* C_AT_##listname(int index) {\
    struct llitem_##T* ptr = listname.Head; \
    for(int i = 0; i < index; i++) ptr = ptr->next; \
    return ptr;\
}\
T* AT_##listname(int index) {\
    struct llitem_##T* ptr = C_AT_##listname(index);\
    return &ptr->fields;\
};\
T* FIND_##listname(int(*checkfn)(T)) {\
    struct llitem_##T* ptr = listname.Head;\
    while(ptr != NULL) {\
        if(checkfn(ptr->fields) == 0) return ptr;\
        ptr = ptr->next;\
    };\
    return NULL;\
}\
int FINDINDEX_##listname(int(*checkfn)(T)) {\
    struct llitem_##T* ptr = listname.Head;\
    int n = 0;\
    while(ptr != NULL) {\
        if(checkfn(ptr->fields) == 0) return n;\
        ptr = ptr->next;\
        n++;\
    };\
    return -1;\
}\
void REMOVEAT_##listname(int index) {\
    struct llitem_##T* tmp_elm = C_AT_##listname(index);\
    struct llitem_##T* tmp_next = tmp_elm->next;\
    free((void*)tmp_elm);\
    if(index < 0) return;\
    if(index == 0) {listname.Head = tmp_next; return; }\
    struct llitem_##T* tmp_prev = C_AT_##listname(index - 1);\
    tmp_prev->next = tmp_next;\
}\
void INSERTAT_##listname(int index, T element) {\
    if(index == 0) {PREPEND_##listname(element);return;}\
    struct llitem_##T* ptr = listname.Head;\
    struct llitem_##T* tmp_prec = NULL;\
    for(int i = 0; i < index; i++) {tmp_prec = ptr;ptr = ptr->next;}\
    struct llitem_##T* tmp_next = ptr;\
    ptr = (struct llitem_##T*)malloc(sizeof(struct llitem_##T));\
    tmp_prec->next = ptr;\
    ptr->fields = element;\
    ptr->next = tmp_next;\
}\
void SORT_##listname(int(*comparefn)(T a, T b)) {\
    int contascambi = 0;\
    do {\
        contascambi = 0;\
        int n = COUNT_##listname();\
        for(int i = 0; i < n - 1; i++) {\
            if(comparefn(*listname.At(i), *listname.At(i + 1)) > 0) {\
                T aus = C_AT_##listname(i)->fields;\
                C_AT_##listname(i)->fields = C_AT_##listname(i + 1)->fields;\
                C_AT_##listname(i + 1)->fields = aus;\
                contascambi++;\
            }\
        }\
    } while(contascambi != 0);\
}\
                                                \
LinkedList_##T listname = {                          \
    NULL,                                       \
    PREPEND_##listname,                               \
    APPEND_##listname,                              \
    COUNT_##listname,                                \
    EACH_##listname,                                   \
    AT_##listname, \
    FIND_##listname,\
    FINDINDEX_##listname,\
    REMOVEAT_##listname,\
    INSERTAT_##listname,\
    SORT_##listname,\
};