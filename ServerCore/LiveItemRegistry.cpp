#include "stdafx.h"
#include "LiveItemRegistry.hpp"

bool LiveItem::RegisterFor(int id, LiveItemRegistry * registry)
{
	source = sdem.Lookup(id);
	if (source)
	{
		sdem_id = id;
		container = registry;
		storedJSON = std::wstring(L"");
		live_id = (int)registry->items.size();
		registry->items.push_back(this);
		return true;
	}
	return false;
}

void LiveItem::Unregister()
{
	sdem_id = -1;
	source = nullptr;
	container->items.erase(container->items.begin() + live_id);
	live_id = -1;
}

std::string * LiveItem::ValueIfChanged()
{
	if (source)
	{
		if (hasChanged())
		{
			std::string * response = new std::string();
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
			*response = cv.to_bytes(source->Json());
			return response;
		}
	}
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
	if (items.size() > 0)
	{
		std::string * response = new std::string("{");
		for (int i = 0; i < items.size(); i++)
		{
			LiveItem * li = items.at(i);
			std::string * str = li->ValueIfChanged();
			if (str != nullptr)
			{
				response->append(*str);
				delete str;
				response->append(",");
			}
		}
		response->pop_back();
		response->append("}");
		if (response->length() > 1)
			return response;
		delete response;
		return nullptr;
	}
	else {
		return nullptr;
	}
}

void LiveItemRegistry::Empty()
{
	while (items.size() > 0)
	{
		LiveItem * li = items.at(0);
		items.erase(items.begin());
		delete li;
	}
	items.clear();
}