
#ifndef _UTILITIES_GLOBE_ETC_
#define _UTILITIES_GLOBE_ETC_

#include <vector>

// For template instantiations, see Utilities.cc!

// Definitions.
template <typename H_FUNC_TYPE, typename H_FUNC, typename D_FUNC, typename N_FUNC>
std::vector<std::vector<int>> _AStar_FULL(std::vector<int> A, std::vector<int> B, H_FUNC Heuristic_F = nullptr, 
                                          D_FUNC Distance_F = nullptr, N_FUNC Neighbour_F = nullptr);


// Local typedefs.
typedef double (*AStar_Heuristic_Func_Type_A)(const std::vector<int> &, const std::vector<int> &);
typedef double (*AStar_Distance_Func_Type_A)(const std::vector<int> &, const std::vector<int> &);
typedef void (*AStar_Neighbour_Func_Type_A)(const std::vector<int> &, std::vector<std::vector<int>> &);


// Convenience Bindings. ("Curryings.")
auto AStar = [](std::vector<int> a, std::vector<int> b) {
    return _AStar_FULL<double, AStar_Heuristic_Func_Type_A, AStar_Distance_Func_Type_A, AStar_Neighbour_Func_Type_A>(
        a, b, nullptr, nullptr, nullptr);
};

auto AStar_Custom_heur = [](std::vector<int> a, std::vector<int> b, AStar_Heuristic_Func_Type_A Heur_Func) {
    return _AStar_FULL<double, AStar_Heuristic_Func_Type_A, AStar_Distance_Func_Type_A, AStar_Neighbour_Func_Type_A>(
        a, b, Heur_Func, nullptr, nullptr);
};

auto AStar_Custom_dist = [](std::vector<int> a, std::vector<int> b, AStar_Distance_Func_Type_A Dist_Func) {
    return _AStar_FULL<double, AStar_Heuristic_Func_Type_A, AStar_Distance_Func_Type_A, AStar_Neighbour_Func_Type_A>(
        a, b, nullptr, Dist_Func, nullptr);
};

auto AStar_Custom_nhbr = [](std::vector<int> a, std::vector<int> b, AStar_Neighbour_Func_Type_A Nghbr_Func) {
    return _AStar_FULL<double, AStar_Heuristic_Func_Type_A, AStar_Distance_Func_Type_A, AStar_Neighbour_Func_Type_A>(
        a, b, nullptr, nullptr, Nghbr_Func);
};

#endif
