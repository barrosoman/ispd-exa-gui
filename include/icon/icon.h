#pragma once
class QRectF;

class Icon {
public:
    virtual ~Icon()                                        = default;
    virtual void toggleChoosen()                           = 0;
    virtual bool isChosen()                                = 0;
    virtual void toggleChosenIfInside(QRectF area)         = 0;
    virtual void updatePosition()                          = 0;
};
