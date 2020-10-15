#include <cstdio>
#include "buffer.hh"
#include "string.hh"
#include "variant.hh"

void string_test() {
    auto string = String::from_str("Hello, world");
    std::printf("String: %s\n", string.as_str());
}

void buffer_test() {
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

void either_test() {
    auto foo = Either<int, float>::left(20);
    std::printf("%d\n", foo.unwrap_left());
}

int main() {
    try {
        buffer_test();
        string_test();
        either_test();
    } catch (const char *string) {
        std::printf("Failed: %s\n", string);
    }
}
