//Utility functions, in a similar vein as the things in "Misc.cc" and "Misc.h", but in a more
// involved scope. These functions are implementations of handy (hopefully generic) routines 
// which might be useful to have lying around sometime.
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

#include "Utilities.h"

#include <cmath>      //Used if taxicab distance is used (fabs.)
#include <iostream>

//H_FUNC   ---> A* Heuristic function. Takes two std::vector<int>'s and returns H_FUNC_TYPE (like, a double, or something.)
//D_FUNC   ---> A* Distance function.  Takes two std::vector<int>'s and returns H_FUNC_TYPE (like, a double, or something.)
//N_FUNC   ---> A* Neighbour function. Takes one std::vector<int>, a vector of std::vector<int>'s, and returns void. Fills the latter with neighbouring nodes.
//

template<typename H_FUNC_TYPE, typename H_FUNC, typename D_FUNC, typename N_FUNC > 
std::vector< std::vector<int> >  _AStar_FULL( std::vector<int>  A,  std::vector<int>  B, \
                                                      H_FUNC Heuristic_F = nullptr,      \
                                                      D_FUNC Distance_F  = nullptr,      \
                                                      N_FUNC Neighbour_F = nullptr  ){
    //A is the starting point and B is the end point.

    std::vector< std::vector<int> > result;

    //Avoid doing anything if there is no need to.
    if((A == B) || (A.size() != B.size())){ 
        result.push_back(A);
        result.push_back(B);
    }

    std::vector< std::vector<int> > closedset;  //A collection of previously-visited nodes.
    std::vector< std::vector<int> > openset;    //The set of all nodes to be evaluated. Initially holds the starting point.
    //openset.push_back(A);
    std::map< std::vector<int>, std::vector<int> > came_from;  //A collection of navigated nodes. Keeps track of where each node branched from.

    //Score storage.
    std::map< std::vector<int>, H_FUNC_TYPE > g_score;  //The path *cost* from starting point to current point.
    //g_score[A] = static_cast<H_FUNC_TYPE>( 0 );
    std::map< std::vector<int>, H_FUNC_TYPE > h_score;  //The heuristic cost from a point to the goal point.
    //h_score[A] = static_cast<H_FUNC_TYPE>( HeuristicFunc(A,B) );
    std::map< std::vector<int>, H_FUNC_TYPE > f_score;  //g + h  --> This is the metric we want to minimize.
    //f_score[A] = h_score[A] + g_score[A];

    //------------------------------------------------------------------------------------
    //---------------------------- Lambdas for use locally. ------------------------------
    //------------------------------------------------------------------------------------
    auto _sorter = [&f_score](const std::vector<int> & a, const std::vector<int> & b ){
                      //Sort the lowest to the top (so we can pop it off cheaply.)
                      return f_score[a] > f_score[b];
    };

    std::function< void (const std::vector<int> &) > _reconstruct_path;
    _reconstruct_path = [&came_from, &result, &_reconstruct_path](const std::vector<int> & node)->void {
                      //Check to see if the previous node exists.
                      if( came_from.find(node) != came_from.end() ){
                          _reconstruct_path( came_from[node] );
                          result.push_back( came_from[node] );
                          return;
                      }else{
                          return;
                      }
    };

    //------------------------------------------------------------------------------------
    //-------Lambdas which can be overridden by sending in suitable replacements.---------
    //------------------------------------------------------------------------------------
    std::function< H_FUNC_TYPE (const std::vector<int> & , const std::vector<int> & ) >   _Distance;
    _Distance = [](const std::vector<int> & a, const std::vector<int> & b)->H_FUNC_TYPE {
                      H_FUNC_TYPE res = static_cast<H_FUNC_TYPE>(0);
                   
                      //Polar distance.
                      for(unsigned int i=0; i<a.size(); ++i) res += static_cast<H_FUNC_TYPE>((b[i] - a[i])*(b[i] - a[i]));
                      res = sqrt(res);
                      return static_cast<H_FUNC_TYPE>(res);

                     /*
                      //Taxicab distance.
                      for(unsigned int i=0; i<a.size(); ++i) res += static_cast<H_FUNC_TYPE>(fabs(b[i] - a[i]));
                      return static_cast<H_FUNC_TYPE>(res);
                     */
    };

    if(Distance_F != nullptr){
        _Distance = Distance_F;
    }
        

    std::function< void (const std::vector<int> & , std::vector< std::vector<int> > & ) >  _Neighbour;
    _Neighbour = [](const std::vector<int> & p, std::vector< std::vector<int> > & nhbrs)->void {
                      //This lambda returns nothing, but sends back a vector of neighbour nodes (nhbrs.)
                      //
                      // NOTE: that this lambda assumes that neighbouring points are +- 1 the coordinates
                      // of the node which was sent in.
                      //
                      // NOTE: this function makes no attempt to determine if the neighbours exist or are
                      // disallowed. Send in your own function to do that!
                     
                   //   if(p.size() != p.size()) return;
                      std::vector<int> up,dn;
/*
                      //Non-diagonals only. (Any dimension.)
                      for(unsigned int i=0; i<p.size(); ++i){
                          up.clear(); dn.clear();
                          for(unsigned int j=0; j<p.size(); ++j){
                              if(i==j){
                                  up.push_back( p[j] +  static_cast<int>(1));
                                  dn.push_back( p[j] -  static_cast<int>(1));
                              }else{
                                  up.push_back( p[j] );
                                  dn.push_back( p[j] );
                              }
                          }
                          nhbrs.push_back(up);  nhbrs.push_back(dn);
                      }
*/

                      //Non-diagonals and diagonals. (Any dimension.)
                      unsigned int index, max = 1;   //(max = 3^dimension. Saves including cmath header..)
                      for(unsigned int j=0; j<p.size(); ++j){ max *= 3; }
                      for(unsigned int i=0; i<max; ++i){
                          up.clear();
                          index = 0;
                          for(unsigned int j=1; j<max; j*=3){
                              up.push_back( (((i/(j)) % 3) - 1 ) + p[index]);
                              ++index;
                          }
                          if( up != p ) nhbrs.push_back(up);
                      }

                      return;
    };

    if(Neighbour_F != nullptr){
        _Neighbour = Neighbour_F;
    }

    std::function< H_FUNC_TYPE (const std::vector<int> &, const std::vector<int> &) >  _Heuristic;
    _Heuristic = [&_Distance](const std::vector<int> & somepoint, const std::vector<int> & endpoint)->H_FUNC_TYPE {
        return static_cast<H_FUNC_TYPE>( _Distance(somepoint, endpoint) );
    };

    if(Heuristic_F != nullptr){
        _Heuristic = Heuristic_F;
    }

    //------------------------------------------------------------------------------------
    //------------------------------------ Data prep -------------------------------------
    //------------------------------------------------------------------------------------
    openset.push_back(A);

    g_score[A] = static_cast<H_FUNC_TYPE>( 0 );
    h_score[A] = static_cast<H_FUNC_TYPE>( _Heuristic(A,B) );
    f_score[A] = h_score[A] + g_score[A];

    //------------------------------------------------------------------------------------
    //---------------------------------- Temp storage ------------------------------------
    //------------------------------------------------------------------------------------
    std::vector< std::vector<int> > neighbours;
    std::vector<int> up,dn;
    std::vector<int> current;
    std::vector< std::vector<int> >::iterator vvit;

    bool tentatv_is_better;
    H_FUNC_TYPE tentatv_g_score;



    //------------------------------------------------------------------------------------
    //---------------------------- Entry into the Algorithm ------------------------------
    //------------------------------------------------------------------------------------

    while(openset.size() != 0){
        //Determine which node from openset has the lowest f_score.
        //Sorting is NlogN usually, N*N at worst, and hopefully slightly lower in this case
        // because the sort happens on each pass. NOTE: It was extremely slow to use a full 
        // sort, so now we just use nth_element to grab the lowest f_score'd value.
        //
        //After sort, lowest f_Score'd node is in openset.last().
//        std::sort( openset.begin(), openset.end(), _sorter );
        std::nth_element( openset.begin(), openset.end()-1, openset.end(), _sorter );

        //If the best node is the target node, we reconstruct the final path and exit.
        if(openset[ openset.size()-1] == B){
            _reconstruct_path(came_from[B]);

            result.push_back(came_from[B]);
            result.push_back( B );

            return result;
        }

        //Remove this node from the openset and put it into the closedset.
        closedset.push_back( openset[openset.size()-1] );
        openset.pop_back();

        current = closedset[closedset.size() - 1];

        //Create the list of neighbours.
        neighbours.clear();
        //neighbours =  <DECIDE IF YOU WANT THIS FUNCTIONALITY OR NOT>
        _Neighbour( current , neighbours);

        //Cycle through all neighbour nodes to potentially add to the openset. 
        for(vvit = neighbours.begin(); vvit != neighbours.end(); ++vvit){

            //If this neighbour node is in closedset already, skip this iteration.
            if( find(closedset.begin(), closedset.end(), (*vvit)) != closedset.end()){
                continue;  //Not 'break'!! FML!
            }

            //Update the move-by-move distance. Since it is the total distance travelled, 
            // we can just sum piece-by-piece.
            tentatv_g_score = g_score[current] + _Distance(current, (*vvit)); 
     
            //If this neighbour node is not in openset, add it.
            if( find(openset.begin(), openset.end(), (*vvit)) == openset.end()){
                openset.push_back( (*vvit) );
                tentatv_is_better = true;
 
            //If this neighbour node is present, check if the tentative score is better.
            }else if(tentatv_g_score <= g_score[*vvit]){
                tentatv_is_better = true;
            
            //Otherwise, this node is not useful to us at the moment.
            }else{
                tentatv_is_better = false;
            }

            //If we are going to keep this node, we record the parent node and some
            // relevant stats.
            if(tentatv_is_better == true){
                came_from[*vvit] = current;
                g_score[*vvit]   = tentatv_g_score;
                h_score[*vvit]   = _Heuristic(*vvit, B);
                f_score[*vvit]   = g_score[*vvit] + h_score[*vvit];
            }

        }

    }//while(openset.size() !+ 0)


    //We will only get here if a failure occurs. In this case, we will output a SINGLE 
    // node. Therefore, if we call this routine and get only a single point, we should 
    // check what went wrong. 
    //
    // NOTE: that the case where the beginning point = the end point will also return 
    // a single point: If we get more than one point from this routine, it is 
    // guaranteed to be a solution.
    result.clear();
    result.push_back( B );
    return result; 
}



//AStar template instantiations. 

//Type A:
//   Data (point) type- -->   vector of ints.
//   Return type---------->   vector of vectors of ints. 
//   Heuristic function--->   double (vector of ints, vector of ints) 
//   Distance function---->   (same as Heuristic function...)
//   Neighbour function--->   void (vector of ints, vector of vectors of ints).

/*
typedef double(*_Utilities_Globe_etc__Heuristic_Func_Type_A)(std::vector<int>, std::vector<int>);
template std::vector< std::vector<int> > 
_AStar_FULL<double,_Utilities_Globe_etc__Heuristic_Func_Type_A>(std::vector<int>,  std::vector<int>, _Utilities_Globe_etc__Heuristic_Func_Type_A);
*/



typedef double(*_Utilities_Globe_etc__Heuristic_Func_Type_A)(const std::vector<int> &, const std::vector<int> &);
typedef  double(*_Utilities_Globe_etc__Distance_Func_Type_A)(const std::vector<int> &, const std::vector<int> &);
typedef   void(*_Utilities_Globe_etc__Neighbour_Func_Type_A)(const std::vector<int> &, std::vector< std::vector<int> > &);

template std::vector< std::vector<int> >  _AStar_FULL<double,   _Utilities_Globe_etc__Heuristic_Func_Type_A,
                                                                _Utilities_Globe_etc__Distance_Func_Type_A,
                                                                _Utilities_Globe_etc__Neighbour_Func_Type_A  
                                                     >( std::vector<int>  A,  std::vector<int>  B, 
                                                                _Utilities_Globe_etc__Heuristic_Func_Type_A,
                                                                _Utilities_Globe_etc__Distance_Func_Type_A,
                                                                _Utilities_Globe_etc__Neighbour_Func_Type_A   );



