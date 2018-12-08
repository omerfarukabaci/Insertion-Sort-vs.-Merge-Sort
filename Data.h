#include <iostream>
#include <string>
using namespace std;

class Data {
	string line;
	string timeStamp;
	float lastPrice;
public:
	const string& get_line() const { return line; };
	void set_line(string inputLine);

	const string& get_timeStamp() const { return timeStamp; };
	void set_timeStamp(string inputTimeStamp);

	const float& get_lastPrice() const { return lastPrice; };
	void set_lastPrice(float inputLastPrice);
};
