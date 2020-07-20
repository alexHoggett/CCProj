#ifndef mistyBrush_hpp
#define mistyBrush_hpp

#include <stdio.h>
#include "linegen.hpp"

class MistyBrush : public LineGen {
    public:
    
    private:
        virtual void draw(int x, int y, ofColor colour);
    
};

#endif /* mistyBrush_hpp */
