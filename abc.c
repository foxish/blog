#include <stdio.h>

#pragma pack(0)
typedef struct finder {
	char a;
	char b;
};

int main(void) {
	struct finder f;
	int* ptr = (int*)&f;
	*ptr = 1;
    return (int) f.a;
}
