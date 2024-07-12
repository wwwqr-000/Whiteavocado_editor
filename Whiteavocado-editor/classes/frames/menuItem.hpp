#pragma once

class menuItem : public box {
    private:
        std::string placeholder;//Inner text
        //Click listener here (W.I.P)

    public:
        menuItem(point2 min_, point2 max_, point3 boxColor) : box(min_, max_, boxColor) {}
        menuItem(point2 min_, point2 max_, point3 boxColor, int lineSize, std::string placeholder) : box(min_, max_, lineSize, boxColor), placeholder(placeholder) {}

        std::string getPlaceholder() { return this->placeholder; }
        void setPlaceholder(std::string& placeholder) { this->placeholder = placeholder; }
};
