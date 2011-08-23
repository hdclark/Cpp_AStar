//LICENSE: LGPLv3.
//
// Code written by haley clark in 2011. Based on Pseudo code implementation on Wikipedia article for A*.
//
// This is an implementation of the A* algorithm. It is templated to accept passing in your own Heuristic function
// and defining the data type which 'distance' (and thus also the heuristic function) is measured in.
//
// Lambda functions are used as a default:
//    - neighbour grabbing function,
//    - heuristic function,
//    - distance function.
//
//   The plan is to let one pass in their own data types and function definitions, but this will happen down the road.
//

#ifndef _UTILITIES_GLOBE_ETC_
#define _UTILITIES_GLOBE_ETC_

#include <vector>
//#include <functional>   //std::bind.


//For template instantiations, see Utilities.cc!

//Definitions.
template<typename H_FUNC_VAL, typename FUNC> 
std::vector< std::vector<int> >  _AStar_FULL( std::vector<int> ,  std::vector<int> ,  FUNC Heuristic );


//Local typedefs.
typedef double(*AStar_Heuristic_Func_Type_A)(std::vector<int>, std::vector<int>);


//Convenience Bindings. ("Curryings.")
auto AStar = [](std::vector<int> a,  std::vector<int> b){
               return _AStar_FULL<double, AStar_Heuristic_Func_Type_A>( a, b, nullptr );
};




/*   CURRENT EXAMPLE CODE SNIPPETS:

  #include "Utilities.h"

  some_local = AStar(beginning, end);
  //some_local = _AStar_FULL<double, Myloc_Heuristic_Func_Type>(beginning, end, NULL);
  //some_local = _AStar_FULL<double, Myloc_Heuristic_Func_Type>(beginning, end, SomeHeuristicFunc);

*/

#endif
