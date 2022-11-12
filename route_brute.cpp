#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <tuple>

#define TASK "15"

// Object Coordinates Struct
struct OC {
    std::string name;
    float x, y, z;
};

// Non-start-or-end objects that can have their order modified
std::vector<OC> Objects = {
    {"Coin 1", -1902.517, 100,      1066.476},
    {"Coin 2",  1529.892, 199.972,  822.343 },
    {"Coin 3",  856.882,  100,     -422.768 },
    {"Coin 4", -887.991,  209.611, -452.45  },
    {"Coin 5",  611.302,  266.972, -1985.706},
    {"Coin 6",  1812.085, 290,     -1760.313}
};
/* Define this here to allow for
compile-time evaluation of possibleRoutes */
const int objVecSize = 6;

// Start and End objects
const OC startObj = {"Start", 0, 317, 1000},
         endObj   = {"End",   0, 0,   3277};

// Specific to this OC naming scheme
inline bool compareOC(OC obj1, OC obj2) {
    return obj1.name[5] < obj2.name[5];
}

// 3D Euclidean distance
inline float euDist(OC a, OC b) {
    return std::sqrt((a.x - b.x)*(a.x - b.x)
                    +(a.y - b.y)*(a.y - b.y)
                    +(a.z - b.z)*(a.z - b.z));
}

/* Number of possible permutations is
equal to n! (n factorial) */
constexpr int factorial(int n) {
    return (n != 0) ? n * factorial(n-1) : 1;
}
constexpr int possibleRoutes = factorial(objVecSize);

std::tuple<std::vector<OC>, float> 
generateRoutes(std::vector<OC> objVec, OC startObj, OC endObj, 
               int maxIterations, float bestRouteScore = 0)
{
    // Keep track of the previous best score (optional argument)
    float ogBestRouteScore = bestRouteScore;
    std::vector<OC> bestRoute(objVecSize);
    int c = 1;
    // Iterate over permutations of Objects vector (up to maxIterations)
    while (std::next_permutation(objVec.begin(), objVec.end(), compareOC)
           && c <= maxIterations) {
        // DEBUG: std::cout << "Permutation " << c << "\n";
        // Sum of euDists for a given route
        float routeScore = 0;
        for (int i = 0; i < objVecSize; i++) {
            // DEBUG: std::cout << objVec[i].name << "\n";
            /* Calculate euDist from start pos to first
            object in route permutation */
            if (i == 0) {
                routeScore += euDist(objVec[0], startObj);
                continue;
            }
            /* Calculate euDist from last object in route
            permutation to end pos */
            else if (i == objVecSize-1) {
                routeScore += euDist(objVec[i], endObj);
                continue;
            }
            /* Calculate euDist from current object
            to next object */
            routeScore += euDist(objVec[i], objVec[i+1]);
        }
        if (routeScore < bestRouteScore || bestRouteScore == 0) {
            bestRoute = objVec;
            bestRouteScore = routeScore;
        }
        // DEBUG: std::cout << "Score: " << routeScore << "\n";
        c++;
    }
    // Print best route
    std::cout << "\nBest Route: " << "\n";
    for (OC obj : bestRoute)
        std::cout << obj.name << "\n";
    std::cout << "Score: " << bestRouteScore;
    /* If a previous bestRouteScore was passed to the function,
    and the new bestRouteScore is an improvement, display the
    improvement in route score */
    if (ogBestRouteScore > 0 && bestRouteScore > ogBestRouteScore)
        std::cout << " (-" << ogBestRouteScore - bestRouteScore << ")\n";
    std::cout << std::endl;

    return {bestRoute, bestRouteScore};
}

int main(int argc, char* argv[]) {
    int possibleRoutes = factorial(objVecSize);

    std::cout << "\n" << possibleRoutes << " possible routes for "
              << Objects.size() << " objects\n";

    /* By default, the program will check all possible permutations of the Objects
    vector, but you can swap possibleRoutes out for any number of max iterations you like */
    std::tuple<std::vector<OC>, float> best = generateRoutes(Objects, startObj, 
                                                             endObj, possibleRoutes);
    // Unpack tuple
    std::vector<OC> bestRoute = std::get<0>(best);
    float bestRouteScore = std::get<1>(best);

    /* Write best route to file if "-f" argument
    is passed on the command line */
    if (argc > 1 && argv[1] == "-f") {
        std::ofstream outFile;
        outFile.open(std::string("Task_") + TASK + "_Best_Route.txt");

        if (!outFile.is_open()) {
            std::cout << "Error: Could not open/create output file" << std::endl;
            return 1;
        }

        outFile << "Best Route:\n";
        for (OC obj : bestRoute)
            outFile << obj.name << "\n";
        outFile << "Score: " << bestRouteScore;
        outFile.close();
    }

    return 0;
}