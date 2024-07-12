#pragma once

class menuItem : public box {
    private:
        std::string placeholder;//Inner text

    public:
        menuItem(point2 min_, point2 max_, point3 boxColor) : box(min_, max_, boxColor) {}
        menuItem(point2 min_, point2 max_, point3 boxColor, int lineSize, std::string placeholder) : box(min_, max_, lineSize, boxColor), placeholder(placeholder) {}
        menuItem(point2 min_, point2 max_, point3 boxColor, int lineSize, std::string placeholder, std::function<void()> onclick) : box(min_, max_, lineSize, boxColor, true, onclick), placeholder(placeholder) {}

        std::string getPlaceholder() const { return this->placeholder; }
        void setPlaceholder(std::string& placeholder) { this->placeholder = placeholder; }
};
