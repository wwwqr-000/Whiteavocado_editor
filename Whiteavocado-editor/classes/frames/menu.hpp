#pragma once

class menu : public frameObj, public box {
    private:
        std::vector<menuItem> items;
        std::string name;
        int margin;
        FlexDirection direction;

    public:
        menu(point2 min_, point2 max_, point3 boxColor) : box(min_, max_, 1, boxColor), direction(ROW), frameObj() {}
        menu(point2 min_, point2 max_, point3 boxColor, std::vector<menuItem>& items) : box(min_, max_, 1, boxColor), items(items), direction(ROW), frameObj() {}
        menu(point2 min_, point2 max_, point3 boxColor, std::vector<menuItem> items, std::string name, FlexDirection direction) : box(min_, max_, 1, boxColor), items(items), name(name), direction(direction), frameObj() {}

        std::vector<menuItem> getItems() const { return this->items; }
        void addItem(menuItem& item) { this->items.emplace_back(item); }
        std::string getName() { return this->name; }
        void setName(std::string& name) { this->name = name; }
};
