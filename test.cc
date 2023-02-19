#include "buffer.hh"
#include "shared.hh"
#include "string.hh"
#include "variant.hh"

#include <cstdio>

void buffer_test();
void string_test();
void either_test();
void maybe_test();
void shared_test();

int main() {
    std::printf("*** BUFFER TEST **********************\n");
    {
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
    }

    std::printf("\n");

    std::printf("*** STRING TEST **********************\n");
    {
        auto string = ysl::String::from_str("Hello, world");
        std::printf("String: %s\n", string.as_str());
    }

    std::printf("\n");

    std::printf("*** EITHER TEST **********************\n");
    {
        auto either = ysl::Either<int, float>::left(20);
        std::printf("%d\n", either.unwrap_left());
    }

    std::printf("\n");

    std::printf("*** MAYBE TEST **********************\n");
    {
        auto maybe = ysl::Maybe<int>::none();

        if (maybe.is_some()) {
            std::printf("Some: %d\n", maybe.unwrap());
        } else {
            std::printf("None\n");
        }

        std::printf("Hello, there! Your code is %d\n", maybe.unwrap_or(5));
    }

    std::printf("\n");

    std::printf("*** SHARED TEST *********************\n");
    {
        auto my_shared = ysl::Shared<int>::from(50);
        ysl::Shared<int> my_same_shared = my_shared.to_share();

        auto weak = my_same_shared.to_weak();

        std::printf("%d = %d\n", *my_shared, *my_same_shared);
    }
}
