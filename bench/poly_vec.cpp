#include "Line.hpp"
#include "Vector2.hpp"
#include <array>
#include <benchmark/benchmark.h>
#include <memory>
#include <numeric>
#include <ostream>
#include <vector>

template <typename Number = double>
struct polygon {

    std::vector<Vector2<Number>> vertices;
    std::vector<Line<Number>>    lines;

    explicit polygon(std::vector<Vector2<Number>> verts) : vertices(std::move(verts)) {
        lines.reserve(vertices.size());
        for (auto i = 0UL; i != vertices.size(); ++i) {
            // this is calling Line's copy assignment operator: TOO LATE
            lines.emplace_back(vertices[i], vertices[(i + 1) % vertices.size()]);
        }
    }

    Number perimeter() const {
        return std::accumulate(lines.cbegin(), lines.cend(), Number{},
                               [](Number peri, const Line<Number>& l) { return peri + l.len; });
    }
};

using poly = polygon<double>;

void vec_populate(benchmark::State& state) { // NOLINT complexity
    std::size_t count = static_cast<std::size_t>(state.range(0));

    for (auto _: state) {
        std::vector<poly> polys;
        polys.reserve(count * 3);
        for (auto i = 0UL; i != count; ++i) {
            polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}}));
            polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}, {7, 8}}));
            polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}, {7, 8}, {9, 10}}));
        }
        benchmark::DoNotOptimize(polys);
    }
}
BENCHMARK(vec_populate)->Range(8, 8 << 15); // NOLINT macro

void vec_iterate(benchmark::State& state) { // NOLINT complexity
    std::size_t count = static_cast<std::size_t>(state.range(0));

    std::vector<poly> polys;
    polys.reserve(count * 3);
    for (auto i = 0UL; i != count; ++i) {
        polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}}));
        polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}, {7, 8}}));
        polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}, {7, 8}, {9, 10}}));
    }

    for (auto _: state) {
        double total_perimeter =
            std::accumulate(polys.begin(), polys.end(), 0.0,
                            [](double peri, const auto& p) { return peri + p.perimeter(); });
        benchmark::DoNotOptimize(total_perimeter);
    }
}
BENCHMARK(vec_iterate)->Range(8, 8 << 15); // NOLINT macro

void vec_both(benchmark::State& state) { // NOLINT complexity
    std::size_t count = static_cast<std::size_t>(state.range(0));

    for (auto _: state) {
        std::vector<poly> polys;
        polys.reserve(count * 3);
        for (auto i = 0UL; i != count; ++i) {
            polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}}));
            polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}, {7, 8}}));
            polys.emplace_back(polygon({{1, 2}, {3, 4}, {4, 5}, {7, 8}, {9, 10}}));
        }

        double total_perimeter =
            std::accumulate(polys.begin(), polys.end(), 0.0,
                            [](double peri, const auto& p) { return peri + p.perimeter(); });
        benchmark::DoNotOptimize(total_perimeter);
    }
}
BENCHMARK(vec_both)->Range(8, 8 << 15); // NOLINT macro

BENCHMARK_MAIN();
