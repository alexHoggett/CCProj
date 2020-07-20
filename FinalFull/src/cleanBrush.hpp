#ifndef cleanBrush_hpp
#define cleanBrush_hpp

#include <stdio.h>
#include "linegen.hpp"

class CleanBrush : public LineGen {
    public:
        void begin();
    private:
        virtual void draw(int x, int y, ofColor colour);
    
};

#endif /* cleanBrush_hpp */
