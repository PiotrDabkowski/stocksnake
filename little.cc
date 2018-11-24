//
// Created by Piotr Dabkowski on 24/11/2018.
//

#include "stockfish/little.h"


Little::Little() {
    payload_ = "hello";
}
const std::string Little::fen() const {
    return payload_;
}