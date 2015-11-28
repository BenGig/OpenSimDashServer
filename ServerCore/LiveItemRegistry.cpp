#include "stdafx.h"
#include "LiveItemRegistry.hpp"

#define PUSHSLEEP 10  // milliseconds

static HANDLE pRunMutex;

bool LiveItem::RegisterFor(int id, LiveItemRegistry * registry)
{
	source = sdem.Lookup(id);
	if (source)
	{
		sdem_id = id;
		container = registry;
		storedJSON = source->Json();
		live_id = (int)registry->size();
		registry->push_back(this);
		return true;
	}
	return false;
}

void LiveItem::Unregister()
{
	sdem_id = -1;
	source = nullptr;
	container->erase(container->begin() + live_id);
	live_id = -1;
}

std::string * LiveItem::ValueIfChanged()
{
	if (source)
	{
		std::string * response = new std::string("");

		if (hasChanged())
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
			*response = cv.to_bytes(source->Json());
		}
		return response;
	}
	else
		return nullptr;
}

bool LiveItem::hasChanged()
{
	if (source)
	{
		if (storedJSON.length() == source->Json().length() && storedJSON == source->Json())
			return false;
		else
		{
			storedJSON = source->Json();
			return true;
		}
	}
	return false;
}

std::string * LiveItemRegistry::ChangedItemsJSON()
{
	std::string * response = new std::string();
	for (int i = 0; i < size(); i++)
	{
		LiveItem * li = this[i];
		response->append(this[i].ValueIfChanged())
	}
	return nullptr;
}
