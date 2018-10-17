#include <iostream>
#include <immintrin.h>
#include <random>

union v4 {
    __m128 v;
    float a[4];
};

// randomly generate vector values
void generate_data(v4 *data, size_t n) {
    // create random engine
    std::random_device r;
    std::default_random_engine e(r());
    // fill data
    for(size_t i = 0; i < n; ++i)
        for(size_t j = 0; j < 4; ++j)
            data[i].a[j] = e();
}

// normalises a vector
void normalise_vectors(v4 *data, v4 *result, size_t n) {
    for(size_t i = 0; i < n; ++i) {
        // square each component
        result[i].v = _mm_mul_ps(data[i].v, data[i].v);
        // calculate sum of the components
        result[i].v = _mm_hadd_ps(result[i].v, result[i].v);
        result[i].v = _mm_hadd_ps(result[i].v, result[i].v);
        // calculate reciprocal square root of values
        // i.e. 1.f / sqrt(value), or the length of the vector
        result[i].v = _mm_rsqrt_ps(result[i].v);
        // multiply result by original data
        result[i].v = _mm_mul_ps(data[i].v, result[i].v);
    }
    // vectors normalised
}

void check_results(v4 *data, v4* result) {
    // check first 100 values
    for(size_t i = 0; i < 100; ++i) {
        // calculate length
        float l = 0.0f;
        // square components and add to l
        for(size_t j = 0; j < 4; ++j)
            l += powf(data[i].a[j], 2.0f);
        // get sqrt of length
        l = sqrtf(l);
        // check individual results
        for(size_t j = 0; j < 4; ++j)
            std::cout << data[i].a[j] / l << " : " << result[i].a[j] << std::endl;
    }
}

int main() {
    auto data = (v4*)_aligned_malloc(sizeof(v4) * 1000000, 16);
    auto result = (v4*)_aligned_malloc(sizeof(v4) * 1000000, 16);
    generate_data(data, 1000000);
    normalise_vectors(data, result, 1000000);
    check_results(data, result);
    _aligned_free(data);
    _aligned_free(result);

    return 0;
}

