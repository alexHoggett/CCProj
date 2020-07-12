//
//  cleanBrush.hpp
//  DrawingExamples
//
//  Created by Alex Hoggett on 31/07/2020.
//

#ifndef cleanBrush_hpp
#define cleanBrush_hpp

#include <stdio.h>
#include "linegen.hpp"

class CleanBrush : public LineGen {
    public:
        void begin();
    private:
        virtual void draw(int x, int y);
    
};

#endif /* cleanBrush_hpp */
