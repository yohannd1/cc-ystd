#include <stdio.h>
#include "buffer.hh"

int main() {
    Buffer<int> my_buffer;

    my_buffer.push(10);
    my_buffer.push(20);
    my_buffer.push(30);

    for (size_t i = 0; i < my_buffer.len(); i++) {
        printf("%d\n", my_buffer[i]);
    }
}
