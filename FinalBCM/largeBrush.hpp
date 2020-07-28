#ifndef largeBrush_hpp
#define largeBrush_hpp

#include <stdio.h>
#include "linegen.hpp"

class LargeBrush : public LineGen {
    public:
    
    private:
        virtual void draw(int x, int y, ofColor colour);
};


#endif /* largeBrush_hpp */
