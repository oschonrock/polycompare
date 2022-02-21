#include "Line.hpp"
#include "Vector2.hpp"
#include <array>
#include <benchmark/benchmark.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <ostream>
#include <variant>
#include <vector>

struct poly {
    virtual ~poly()                  = default;
    virtual double perimeter() const = 0;
};

template <unsigned N, typename Number = double>
struct polygon : poly {

    std::array<Vector2<Number>, N> vertices;
    std::array<Line<Number>, N>    lines;

    template <typename... Pack>
    polygon(Pack... vs) : vertices{vs...}, lines{make_lines(std::index_sequence_for<Pack...>{})} {
        static_assert(sizeof...(vs) == N, "incorrect number of vertices passed");
    }

    Number perimeter() const override {
        return std::accumulate(lines.cbegin(), lines.cend(), Number{},
                               [](Number peri, const Line<Number>& l) { return peri + l.len; });
    }

  private:
    template <std::size_t... Is>
    auto make_lines(std::index_sequence<Is...>) {
        return std::array<Line<Number>, N>{
            Line(vertices[Is], vertices[(Is + 1) % vertices.size()])...};
    }
};

using triangle      = polygon<3>;
using quadrilateral = polygon<4>;
using pentagon      = polygon<5>;

void virt_populate(benchmark::State& state) { // NOLINT complexity
    std::size_t count = static_cast<std::size_t>(state.range(0));
    for (auto _: state) {
        std::vector<std::unique_ptr<poly>> polys;
        polys.reserve(count * 3);
        for (auto i = 0UL; i != count; ++i) {
            polys.push_back(std::make_unique<triangle>(Vec2{1, 2}, Vec2{3, 4}, Vec2{4, 5}));

            polys.push_back(std::make_unique<quadrilateral>(
                quadrilateral(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8})));

            polys.push_back(std::make_unique<pentagon>(
                pentagon(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}, Vec2{9, 10})));
        }
        benchmark::DoNotOptimize(polys);
    }
}
BENCHMARK(virt_populate)->Range(8, 8 << 15); // NOLINT macro

void virt_iterate(benchmark::State& state) { // NOLINT complexity
    std::size_t                        count = static_cast<std::size_t>(state.range(0));
    std::vector<std::unique_ptr<poly>> polys;
    polys.reserve(count * 3);
    for (auto i = 0UL; i != count; ++i) {
        polys.push_back(std::make_unique<triangle>(Vec2{1, 2}, Vec2{3, 4}, Vec2{4, 5}));

        polys.push_back(std::make_unique<quadrilateral>(
            quadrilateral(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8})));

        polys.push_back(std::make_unique<pentagon>(
            pentagon(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}, Vec2{9, 10})));
    }
    for (auto _: state) {

        double total_perimeter =
            std::accumulate(polys.begin(), polys.end(), 0.0,
                            [](double peri, auto& p) { return peri + p->perimeter(); });
        benchmark::DoNotOptimize(total_perimeter);
    }
}
BENCHMARK(virt_iterate)->Range(8, 8 << 15); // NOLINT macro

void virt_both(benchmark::State& state) { // NOLINT complexity
    std::size_t count = static_cast<std::size_t>(state.range(0));
    for (auto _: state) {
        std::vector<std::unique_ptr<poly>> polys;
        polys.reserve(count * 3);
        for (auto i = 0UL; i != count; ++i) {
            polys.push_back(std::make_unique<triangle>(Vec2{1, 2}, Vec2{3, 4}, Vec2{4, 5}));

            polys.push_back(std::make_unique<quadrilateral>(
                quadrilateral(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8})));

            polys.push_back(std::make_unique<pentagon>(
                pentagon(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}, Vec2{9, 10})));
        }
        double total_perimeter =
            std::accumulate(polys.begin(), polys.end(), 0.0,
                            [](double peri, auto& p) { return peri + p->perimeter(); });
        benchmark::DoNotOptimize(total_perimeter);
    }
}
BENCHMARK(virt_both)->Range(8, 8 << 15); // NOLINT macro

BENCHMARK_MAIN();
