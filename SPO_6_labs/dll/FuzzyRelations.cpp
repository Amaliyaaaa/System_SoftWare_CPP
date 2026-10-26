
#include "FuzzyRelations.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
    void compose(const double* a, const double* b,
        double* result, int m, int n, int p, int mode) {

        if (!a || !b || !result || m <= 0 || n <= 0 || p <= 0)
            throw invalid_argument("Ќекорректные параметры матриц.");

        for (int i = 0; i < m * n; ++i)
            if (!(a[i] >= 0.0 && a[i] <= 1.0))
                throw invalid_argument("Ёлементы A должны быть от 0 до 1.");

        for (int i = 0; i < n * p; ++i)
            if (!(b[i] >= 0.0 && b[i] <= 1.0))
                throw invalid_argument("Ёлементы B должны быть от 0 до 1.");

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < p; ++j) {
                double value = mode == 2 ? 1.0 : 0.0;

                for (int k = 0; k < n; ++k) {
                    double x = a[i * n + k];
                    double y = b[k * p + j];
                    double current;

                    if (mode == 1)
                        current = min(x, y);
                    else if (mode == 2)
                        current = max(x, y);
                    else
                        current = x * y;

                    if (mode == 2)
                        value = min(value, current);
                    else
                        value = max(value, current);
                }

                result[i * p + j] = value;
            }
        }
    }
}

namespace Fuzzy {
    void Relations::MaxMin(const double* a, const double* b,
        double* result, int m, int n, int p) {
        compose(a, b, result, m, n, p, 1);
    }

    void Relations::MinMax(const double* a, const double* b,
        double* result, int m, int n, int p) {
        compose(a, b, result, m, n, p, 2);
    }

    void Relations::MaxProduct(const double* a, const double* b,
        double* result, int m, int n, int p) {
        compose(a, b, result, m, n, p, 3);
    }
}
