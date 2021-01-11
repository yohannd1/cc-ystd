#include <cstdio>

#include "buffer.hh"
#include "string.hh"
#include "variant.hh"
#include "shared.hh"

void buffer_test();
void string_test();
void either_test();
void maybe_test();
void shared_test();

int main() {
    buffer_test();
    string_test();
    either_test();
    maybe_test();
    shared_test();
}

void buffer_test() {
    std::printf("*** BUFFER TEST **********************\n");

    int arr[] = { 10, 20, 30 };
    auto buf = ysl::Buffer<int>::from_sized_array(arr, 3);

    std::printf("Without iterators: ");
    for (size_t i = 0; i < buf.len(); i++) {
        std::printf("%d ", buf[i]);
    }
    std::printf("\n");

    std::printf("With iterators: ");
    for (auto m : buf) {
        std::printf("%d ", m);
    }
    std::printf("\n");

    std::printf("\n");
}

void string_test() {
    std::printf("*** STRING TEST **********************\n");

    auto string = ysl::String::from_str("Hello, world");
    std::printf("String: %s\n", string.as_str());

    std::printf("\n");
}

void either_test() {
    std::printf("*** EITHER TEST **********************\n");

    auto either = ysl::Either<int, float>::left(20);
    std::printf("%d\n", either.unwrap_left());

    std::printf("\n");
}

void maybe_test() {
    std::printf("*** MAYBE TEST **********************\n");

    auto maybe = ysl::Maybe<int>::none();
    if (maybe.is_some()) {
        std::printf("Some: %d\n", maybe.unwrap());
    } else {
        std::printf("None\n");
    }
}

void shared_test() {
    std::printf("*** SHARED TEST *********************\n");

    ysl::Shared<int> my_shared(50);
    ysl::Shared<int> my_same_shared = ysl::own(my_shared);

    std::printf("%d = %d\n", *my_shared, *my_same_shared);
}
