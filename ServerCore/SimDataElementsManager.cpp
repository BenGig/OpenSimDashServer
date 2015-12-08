#include "stdafx.h"

// Manager
void SimDataElementsManager::registerElement(SimDataElement * sde)
{
	if (sde->isRegistered)
		return;

	elements[counter] = sde;
	sde->setId(counter);
	counter++;
	_ASSERTE(counter < MAX_ELEMENTS);
	sde->isRegistered = true;
}

std::wstring * SimDataElementsManager::jsonElements()
{
	std::wstring * str = new std::wstring(L"{\"dataDictionary\":[");
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

SimDataElement * SimDataElementsManager::lookup(int i)
{
	return elements[i];
}

