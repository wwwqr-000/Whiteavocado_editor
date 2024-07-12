#pragma once

class box {
    private:
        point2 min_, max_;//Min x, y Max x, y
        int lineSize;
        point3 boxColor;//RGB format

    public:
        box(int lineSize) : lineSize(lineSize) { this->boxColor = {255, 255, 255}; }
        box(int lineSize, point3& boxColor) : lineSize(lineSize), boxColor(boxColor) {}
        box(point2& min_, point2& max_) : min_(min_), max_(max_) { this->boxColor = {255, 255, 255}; this->lineSize = 1; }
        box(point2& min_, point2& max_, point3& boxColor) : min_(min_), max_(max_), boxColor(boxColor) {}
        box(point2& min_, point2& max_, int lineSize) : min_(min_), max_(max_), lineSize(lineSize) { this->lineSize = 1; }
        box(point2& min_, point2& max_, int lineSize, point3& boxColor) : min_(min_), max_(max_), lineSize(lineSize), boxColor(boxColor) {}

        point3 getColor() const { return this->boxColor; }
        bool setColor(point3& rgb) {
            if (rgb.x_i > 255 || rgb.x_i < 0 || rgb.y_i > 255 || rgb.y_i < 0 || rgb.z_i > 255 || rgb.z_i < 0) { return false; }
            this->boxColor = rgb;
            return true;
        }

        int getLineSize() const { return this->lineSize; }
        bool setLineSize(int lSize) {
            if (lSize > 10 || lSize <= 0) { return false; }
            this->lineSize = lSize;
            return true;
        }

        point2 getMax() const { return this->max_; }
        point2 getMin() const { return this->min_; }
        void setMax(point2& max_) { this->max_ = max_; }
        void setMin(point2& min_) { this->min_ = min_; }

};
