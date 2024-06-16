#pragma once
#include "node.h"
struct CompareNode {
    bool operator()(const node* a, const node* b) const {
        return a->frecuencia > b->frecuencia;
    }
};
