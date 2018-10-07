#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>
#include <random>
#include <atomic>
#include <memory>

void monte_carlo_pi(size_t iterations, std::shared_ptr<std::atomic<double>>& pi) {
    // set up random number generator
    std::random_device r;
    std::default_random_engine e(r());
    // doubles between 0 and 1
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // keep track of points within the circle
    unsigned int in_circle = 0;
    for (size_t i = 0; i < iterations; ++i) {
        auto x = distribution(e);
        auto y = distribution(e);
        auto length = sqrt((x * x) + (y * y));
        if (length < 1.0)
            ++in_circle;
    }

    pi->store((4.0 * in_circle) / static_cast<double>(iterations));
}

int main() {
    std::ofstream data("montecarlo-atomic.csv", std::ofstream::out);

   auto pi = std::make_shared<std::atomic<double>>(0.0);

    for (size_t num_threads = 0; num_threads <= 6; ++num_threads) {
        auto total_threads = static_cast<unsigned int>(pow(2.0, num_threads));
        std::cout << "Number of threads is " << total_threads << std::endl;
        data << "num_threads_" << total_threads << std::endl;

        for (size_t iters = 0; iters < 100; ++iters) {
            // get start time
            auto start = std::chrono::system_clock::now();

            std::vector<std::thread> threads;
            for (size_t n = 0; n < total_threads; ++n) {
                threads.emplace_back(monte_carlo_pi, pow(2.0, 24 - num_threads), std::ref(pi));
            }

            // wait for threads to finish
            for (auto &t : threads)
                t.join();

            auto pi_calc = pi->load();
            pi_calc /= static_cast<double>(total_threads);

            auto end = std::chrono::system_clock::now();
            auto total = end - start;
            auto result = std::chrono::duration_cast<std::chrono::milliseconds>(total).count();
            std::cout.precision(std::numeric_limits<double>::digits10);
            std::cout << "pi = " << pi_calc << " calculated in " << std::chrono::duration_cast<std::chrono::milliseconds>(total).count() << " ms" << std::endl;
            data << ", " << result;
        }
        data << std::endl;
    }

    data.close();
    return 0;
}