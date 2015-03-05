#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>
#include <stdlib.h>
#include <stdint.h>
#include <random>

#include "structures.h"

// Questions 2 and 3
void linearApproximationMatrix(int nbElt) {
    std::vector<std::pair<int,int>> goodKeys ;
    std::cout << "\\[\n\\bordermatrix{\n& " ;
    for(int b = 0 ; b < nbElt-1 ; b++) {
        std::cout << b << " & " ;
    }
    std::cout << nbElt-1 << "\\cr\n" ;
    for(int a = 0 ; a < nbElt ; a++) {
        std::cout << a << " & " ;
        for(int b = 0 ; b < nbElt ; b++) {
            int nbKey = 0 ;
            for(int c = 0 ; c < nbElt ; c++) {
                Block blockA(a) ;
                Block blockB(b) ;
                Block key(c) ;
                blockA.product(key) ;
                key.substitution() ;
                blockB.product(key) ;
                if(blockA.bitsXor() == blockB.bitsXor()) {
                    nbKey ++ ;
                }
            }
            std::cout << nbKey ;
            double proba = ((double)nbKey)/16 ;
            if(std::abs(proba - 0.5) > 0.3) {
                // std::cout << nbKey ;
                goodKeys.push_back(std::pair<int,int>(a, b)) ;
            }
            if(b < nbElt-1)
                std::cout << " & " ;
        }
        std::cout << "\\cr\n" ;
    }
    std::cout << "}\n\\]\n";
    std::cout << "Couples (a, b) with the highest probability:\n\\[" ;
    if(goodKeys.size() > 0)
        std::cout << "(" << goodKeys[0].first << ", " << goodKeys[0].second << ")" ;
    for(unsigned i = 1 ; i < goodKeys.size() ; i++) {
        std::cout << ", (" << goodKeys[i].first << ", " << goodKeys[i].second << ")" ;
    }
    std::cout << "\\]\n" ;
}

// Question 4
void experimentalCheck(uint32_t a=1, uint32_t b=5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, UINT32_MAX);
    Block A(a<<28);
    Block B(b<<28);
    B.permutation() ;
    int nbMess = 160000 ;
    for(int i = 0 ; i < 10 ; i++) {
        Block K0(dis(gen)) ;
        Block K1(dis(gen)) ;
        int nbEqual = 0 ;
        for(int j = 0 ; j < nbMess ; j++) {
            Block m(dis(gen)) ;
            Block x(m) ;
            x.addition(K0) ;
            x.turn(K1) ;
            m.product(A) ;
            x.product(B) ;
            if(m.bitsXor() == x.bitsXor())
                nbEqual ++ ;
        }
        std::cout << nbEqual << "/" << nbMess << std::endl ;
    }
}

int main(int argc, char *argv[]) {
    // if(argc != 2) {
    //     std::cerr << "Syntax:" << argv[0] << " <size of the S box>" << std::endl ;
    //     return 1 ;
    // }
    // int S = atoi(argv[1]) ;
    // int nbElt = 1<<S ;
    // linearApproximationMatrix(nbElt) ;
    if(argc != 3) {
        std::cerr << "Syntax:" << argv[0] << " <a> <b>" << std::endl ;
        return 1 ;
    }
    int a = atoi(argv[1]), b = atoi(argv[2]) ;
    experimentalCheck(a, b) ;
    return 0 ;
}
