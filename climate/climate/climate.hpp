// Developer note stack size needs to be increased to allow the allocation of memory for this double = 8 bytes
// 8 * 24 * 60 * 60 * 2 arrays of doubles = 1382400 bytes base allocation is 1MB 
// In VS2017 go to project > properties > System > and set Stack Reserve Size and Stack Commit Size to 2000000
// This prevents a stack overflow

#define maximum_readings 24 * 60 * 60
#define invalid_reading  -1000.0
#include <vector>;


#define fakedata 1	 // NOTE:  Set to 1 use fake sensor values instead of the online values 0

#include <chrono>  // System time
#include <stdexcept>


#if fakedata == 1

#include "fakesensor.hpp"

#else

#include "http.h"

#endif


using namespace std;
using namespace chrono;


class Climate {

private:

#if fakedata == 1	
	FakeSensor sensorDevice;
#else
	Http sensorDevice;
#endif

protected:
	//sampleCount method
	int samples, totalSamples;
	//declaring temperature and humidity arrays
	float tempArray[maximum_readings];
	float humidArray[maximum_readings];
public:

	// Constructors
	Climate();

	// Utility
	void	clearSamples();

	// Sensor related
	long	readSensor();

	long sampleCount(long secs);

	long sampleTotal();

	double getHumidity(long sec);

	double averageHumidity(long lookBack);

	double maximumHumidity(long lookBack);

	double minimumHumidity(long lookBack);

	double getTemperature(long sec);

	double averageTemperature(long lookBack);

	double maximumTemperature(long lookBack);
	
	double minimumTemperature(long lookBack);

	system_clock::time_point StartTime;
};

// Constructor
Climate::Climate() {
	//get app start time
	StartTime = std::chrono::system_clock::now();
	
}


void	Climate::clearSamples() {
	for (int i = 0; i < maximum_readings; i++) {
		tempArray[i] = 0;
		humidArray[i] = 0;
	}
}


long	Climate::readSensor() {

	//get sensor read method launch time
	system_clock::time_point EndTime = std::chrono::system_clock::now();
	sensorDevice.read_data();

	//calculate time passed since launch of app
	std::chrono::duration<double> currentSecond = EndTime - StartTime;

	//store sensor readings in arrays
	tempArray[(long)currentSecond.count()] = sensorDevice.get_temperature_in_c();
	humidArray[(long)currentSecond.count()] = sensorDevice.get_humidity();
	// This line is purely for your debugging and can be removes/commented out in the final code.
		//cout << endl << "Debugging information : " << "Temperature is " << tempArray[(long)currentSecond.count()] << " in degrees C " << sensorDevice.get_humidity() << "% humidity" << endl;
		//cout << " Element at index: " << (long)currentSecond.count() << " Temp: "<<tempArray[(long)currentSecond.count()]<< endl;	
	return (long)currentSecond.count();

	
}

long Climate::sampleCount(long secs) {

	for (int n = (int)secs; n >= 1 ; n--) {
		if (tempArray[n] != NULL) {
			samples++;
		}
		if (humidArray[n] != NULL) {
			samples++;
		}
	}
	
	return samples;
}

long Climate::sampleTotal() {
	for (auto it = begin(tempArray); it != end(tempArray); ++it) {
		totalSamples++;
	}
	return totalSamples;
}

double Climate::getHumidity(long sec) {
	float value = humidArray[sec];
	return value;
}

double Climate::averageHumidity(long lookBack) {
	double avg;
	double sum;
	for (int i = lookBack; i >= 1; i--) {		
	}
	avg = sum / lookBack;
	return avg;

}

double Climate::maximumHumidity(long lookBack) {
	double max = humidArray[lookBack];
	for (int i = lookBack; i >= 1; i--)
	{
		if (humidArray[i] > max) {
			max = humidArray[i];
		}
	}
	return max;
}

double Climate::minimumHumidity(long lookBack) {
	double min = humidArray[lookBack];
	for (int i = lookBack; i >= 1; i--)
	{
		if (humidArray[i] < min) {
			min = humidArray[i];
		}
	}
	return min;
}

double Climate::getTemperature(long sec) {
	float value = tempArray[sec];
	return value;
}

double Climate::averageTemperature(long lookBack) {
	double avg;
	double sum;
	for (int i = lookBack; i >= 1; i--) {
		sum += tempArray[i];
	}
	avg = sum / lookBack;
	return avg;

}

double Climate::maximumTemperature(long lookBack) {
	double max = tempArray[lookBack];
	for (int i = lookBack; i >= 1; i--)
	{
		if (tempArray[i] > max) {
			max = tempArray[i];
		}
	}
	return max;
}

double Climate::minimumTemperature(long lookBack) {
	double min = tempArray[lookBack];
	for (int i = lookBack; i >= 1; i--)
	{
		if (tempArray[i] < min) {
			min = tempArray[i];
		}
	}
	return min;
}


/*Number of element store in vector

const int _a[] = { 1, 2, 3, 4, 5 };
vector <int> a(_a, _a + (sizeof(_a) / sizeof(_a[0])));

	vector <string> args(argv, argv + argc);

	cout << "The array a[] = ";
	for (unsigned n = 0; n < a.size(); n++)
		cout << a[n] << ' ';
	cout << endl;

	cout << "The command line arguments:\n";
	for (unsigned n = 0; n < args.size(); n++)
		cout << n << ": " << args[n] << endl;

	return 0;

*/




