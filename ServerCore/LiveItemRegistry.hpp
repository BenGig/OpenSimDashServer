#pragma once

class LiveItem {
public:
	void RegisterFor(int id);
	void UnregisterFrom(int id);


private:
	int SDEMid;

};