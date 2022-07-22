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
    void reverseLine(Scan* data, int begin, int end, int& objID);

    inline LineRep* getLineRep() {
        return lineRep;
    };
};
