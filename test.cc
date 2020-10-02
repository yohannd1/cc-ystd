#include <cstdio>
#include "buffer.hh"

int main() {
    Buffer<int> my_buffer;

    my_buffer.push(10);
    my_buffer.push(20);
    my_buffer.push(30);

    std::printf("Without iterators: ");
    for (size_t i = 0; i < my_buffer.len(); i++) {
        std::printf("%d ", my_buffer[i]);
    }
    std::printf("\n");

    std::printf("With iterators: ");
    for (auto m : my_buffer) {
        std::printf("%d ", m);
    }
    std::printf("\n");
}
