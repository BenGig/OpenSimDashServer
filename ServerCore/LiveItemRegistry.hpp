#pragma once

#include "stdafx.h"
#include "../mongoose/mongoose.h"

class LiveItemRegistry;

class LiveItem {
public:
	bool registerFor(int id, LiveItemRegistry * registry);
	void unregister();
	std::string * valueIfChanged();
	std::string * value();
	bool hasChanged();

private:
	SimDataElement * source;
	int sdem_id;
	int live_id;
	std::wstring storedJSON;
	LiveItemRegistry * container;
};

class LiveItemRegistry {
public:
	std::vector<LiveItem*> items;
	std::string * changedItemsJSON();
	std::string * allItemsJSON();
	void clear();
};

