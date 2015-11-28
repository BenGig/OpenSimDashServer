#pragma once

#include "stdafx.h"
#include "../mongoose/mongoose.h"

class LiveItemRegistry;

class LiveItem {
public:
	bool RegisterFor(int id, LiveItemRegistry * registry);
	void Unregister();
	std::string * ValueIfChanged();
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
	std::string * ChangedItemsJSON();
	void Empty();
};

