#ifndef STOCKPILEITEM_H
#define STOCKPILEITEM_H
#include <iostream>

using namespace std;

class StockpileItem {
public:
	string name;
	int value;
	StockpileItem(string newName, int newValue) {
		name = newName;
		value = newValue;
	}
	StockpileItem() {}
	~StockpileItem() {}
};

#endif