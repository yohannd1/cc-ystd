#include <stdio.h>
#include "buffer.hh"

int main() {
    Buffer<int> my_buffer;

    my_buffer.push(10);
    my_buffer.push(20);
    my_buffer.push(30);

    printf("Without iterators: ");
    for (size_t i = 0; i < my_buffer.len(); i++) {
        printf("%d ", my_buffer[i]);
    }
    printf("\n");

    printf("With iterators: ");
    for (auto m : my_buffer) {
        printf("%d ", m);
    }
    printf("\n");
}
