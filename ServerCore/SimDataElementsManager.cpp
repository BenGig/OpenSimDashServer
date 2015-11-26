#include "stdafx.h"

// Manager
void SimDataElementsManager::Register(SimDataElement * sde)
{
	if (sde->isRegistered)
		return;

	elements[counter] = sde;
	sde->SetId(counter);
	counter++;
	_ASSERTE(counter < MAX_ELEMENTS);
	sde->isRegistered = true;
}

std::wstring * SimDataElementsManager::JsonElements(std::wstring * str)
{
	str->append(L"{\"dataDictionary\":[");
	for (int i = 0; i < counter; i++)
	{
		str->append(L"{\"label\":\"");
		str->append(elements[i]->label);
		str->append(L"\", \"id\":");
		str->append(std::to_wstring(i));
		str->append(L"},");
	}
	str->pop_back();
	str->append(L"]}");
	return str;
}

SimDataElement * SimDataElementsManager::Lookup(int i)
{
	return elements[i];
}

