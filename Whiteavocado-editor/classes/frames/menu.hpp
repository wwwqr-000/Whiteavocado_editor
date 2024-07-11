#pragma once

class menu : public box {
    private:
        std::vector<menuItem> items;
        std::string name;

    public:
        menu(point2 min_, point2 max_, point3 boxColor) : box(min_, max_, boxColor) {}
        menu(point2 min_, point2 max_, point3 boxColor, std::vector<menuItem>& items) : box(min_, max_, boxColor), items(items) {}
        menu(point2 min_, point2 max_, point3 boxColor, std::vector<menuItem> items, std::string name) : box(min_, max_, boxColor), items(items), name(name) {}

        std::vector<menuItem> getItems() { return this->items; }
        void addItem(menuItem& item) { this->items.emplace_back(item); }
        std::string getName() { return this->name; }
        void setName(std::string& name) { this->name = name; }
};
