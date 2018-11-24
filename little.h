//
// Created by Piotr Dabkowski on 24/11/2018.
//

#ifndef STOCKSNAKE_LITTLE_H
#define STOCKSNAKE_LITTLE_H
#include <string>


class Little {
public:
    Little();
    const std::string fen() const;

private:
    std::string payload_;
};
#endif //STOCKSNAKE_LITTLE_H
