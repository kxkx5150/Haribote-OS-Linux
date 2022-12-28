GLOBAL  _alloca


SECTION .text

_alloca:
    ADD     EAX,-4
    SUB     ESP,EAX
    JMP     DWORD [ESP+EAX]     ; RETの代わり
