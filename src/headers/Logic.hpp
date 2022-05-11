#pragma once

#include "Const.hpp"

class Logic {
   private:
    std::vector<int> objIdxs;
    LineRep* lineRep;

    int beginSize;
    int nID;

   public:
    Logic();
    ~Logic();

    void defineLimit(Scan* data);
    void descriptor(Scan* data);
    void matching(Scan* data);
    void predDist(Scan* data);

    inline LineRep* getLineRep() {
        return lineRep;
    };
};
