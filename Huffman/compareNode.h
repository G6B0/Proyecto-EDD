#pragma once
#include "node.h"
/**Operador de comparador de frecuencias */
struct CompareNode {
    bool operator()(const node* a, const node* b) const {
        return a->frecuencia > b->frecuencia;
    }
};
