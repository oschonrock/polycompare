#include "Line.hpp"
#include "Vector2.hpp"
#include <array>
#include <benchmark/benchmark.h>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <ostream>
#include <variant>
#include <vector>

template <unsigned N, typename Number = double>
struct polygon {

    std::array<Vector2<Number>, N> vertices;
    std::array<Line<Number>, N>    lines;

    template <typename... Pack>
    polygon(Pack... vs) : vertices{vs...}, lines{make_lines(std::index_sequence_for<Pack...>{})} {
        static_assert(sizeof...(vs) == N, "incorrect number of vertices passed");
    }

    Number perimeter() const {
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

using poly = std::variant<triangle, quadrilateral, pentagon>;

template <typename... Base>
struct visitor : Base... {
    using Base::operator()...;
};

template <typename... T>
visitor(T...) -> visitor<T...>;

class poly_actions {
  public:
    static double perimeter(const poly& s) { return std::visit(peri, s); }

  private:
    constexpr static visitor peri = {
        [](const triangle& s) { return s.perimeter(); },
        [](const quadrilateral& s) { return s.perimeter(); },
        [](const pentagon& s) { return s.perimeter(); },
    };
};

void var_populate(benchmark::State& state) {
    std::size_t count = static_cast<std::size_t>(state.range(0));
    for (auto _: state) {
        std::vector<poly> polys;
        polys.reserve(count * 3);
        for (auto i = 0UL; i != count; ++i) {
            polys.push_back(triangle(Vec2{1, 2}, Vec2{3, 4}, Vec2{4, 5}));
            polys.push_back(quadrilateral(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}));
            polys.push_back(pentagon(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}, Vec2{9, 10}));
        }
        benchmark::DoNotOptimize(polys);
    }
}

BENCHMARK(var_populate)->Range(8, 8 << 15); // NOLINT macro

void var_iterate(benchmark::State& state) {
    std::size_t       count = static_cast<std::size_t>(state.range(0));
    std::vector<poly> polys;
    polys.reserve(count * 3);
    for (auto i = 0UL; i != count; ++i) {
        polys.push_back(triangle(Vec2{1, 2}, Vec2{3, 4}, Vec2{4, 5}));
        polys.push_back(quadrilateral(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}));
        polys.push_back(pentagon(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}, Vec2{9, 10}));
    }

    for (auto _: state) {
        double total_perimeter =
            std::accumulate(polys.begin(), polys.end(), 0.0, [](double peri, const poly& p) {
                return peri + poly_actions::perimeter(p);
            });
        benchmark::DoNotOptimize(total_perimeter);
    }
}
BENCHMARK(var_iterate)->Range(8, 8 << 15); // NOLINT macro

void var_both(benchmark::State& state) {
    std::size_t count = static_cast<std::size_t>(state.range(0));
    for (auto _: state) {
        std::vector<poly> polys;
        polys.reserve(count * 3);
        for (auto i = 0UL; i != count; ++i) {
            polys.push_back(triangle(Vec2{1, 2}, Vec2{3, 4}, Vec2{4, 5}));
            polys.push_back(quadrilateral(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}));
            polys.push_back(pentagon(Vec2{1, 2}, Vec2{3, 4}, Vec2{5, 6}, Vec2{7, 8}, Vec2{9, 10}));
        }

        double total_perimeter =
            std::accumulate(polys.begin(), polys.end(), 0.0, [](double peri, const poly& p) {
                return peri + poly_actions::perimeter(p);
            });
        benchmark::DoNotOptimize(total_perimeter);
    }
}

BENCHMARK(var_both)->Range(8, 8 << 15); // NOLINT macro

BENCHMARK_MAIN();
