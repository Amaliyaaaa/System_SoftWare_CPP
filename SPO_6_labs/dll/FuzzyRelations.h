
#pragma once

#ifdef FUZZY_DLL_EXPORTS
#define FUZZY_API __declspec(dllexport)
#else
#define FUZZY_API __declspec(dllimport)
#endif

namespace Fuzzy {
    class Relations {
    public:
        static FUZZY_API void MaxMin(
            const double* a, const double* b,
            double* result, int m, int n, int p);

        static FUZZY_API void MinMax(
            const double* a, const double* b,
            double* result, int m, int n, int p);

        static FUZZY_API void MaxProduct(
            const double* a, const double* b,
            double* result, int m, int n, int p);
    };
}
