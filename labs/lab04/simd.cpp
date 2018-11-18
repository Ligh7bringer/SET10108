#include <immintrin.h>
#include <iostream>

union v4 {
    __m128 v; // SSE 4 x float vector
    float a[4]; // scalar array of 4 floats
};

int main() {
    // a single 128 bit value aligned to 16 bytes (size of 128 bits)
    alignas(16) v4 x{};
    // x can be treated as a collection of 4 floats
    // or other combinations of values for 128 bits
    x.a[0] = 10.f;
    x.a[1] = 20.f;
    x.a[2] = 30.f;
    x.a[3] = 40.f;
    // print original values
    std::cout << "Original values: " << std::endl;
    for (auto i : x.a)
        std::cout << i << std::endl;

    // declare a second 128 bit value
    alignas(16) v4 y{};
    y.a[0] = 10.0f;
    y.a[1] = 20.0f;
    y.a[2] = 30.0f;
    y.a[3] = 40.0f;
    // add y to x
    x.v = _mm_add_ps(x.v, y.v);
    // print out values
    std::cout << "New values: " << std::endl;
    for(auto i : x.a)
        std::cout << i << std::endl;

    // create array of 100 floats aligned to 4 bytes
    auto data = (float*)_aligned_malloc(100 * sizeof(float), 4);
    // access like an array
    data[0] = 2.3f;
    std::cout << data[0] << std::endl;

    // create an array of 100 128-bit values aligned to 16 bytes
    auto big_data = (v4*)_aligned_malloc(100 * sizeof(v4), 16);

    // access like an array of __m128
    big_data[0].a[0] = 1.2f;
    std::cout << big_data[0].a[0] << std::endl;

    // FREE DATA
    _aligned_free(data);
    _aligned_free(big_data);

    return 0;
}