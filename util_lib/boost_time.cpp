#include <iostream>
#include <bitset>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>       // Types and I/O.
#include <boost/date_time/gregorian/gregorian_types.hpp> // Types only.

using namespace boost::gregorian;


void Print(const date& myDate, const std::string& type = "")
{
	using namespace std;
    cout << "\nDate Information..." << type << "\n";
	cout << myDate << endl;
	cout << "Year: " << myDate.year() << ", Month: " << myDate.month() << ", Day: " << myDate.day() << endl;

	// Extra stuff.
	cout << "Day of week: " << myDate.day_of_week() << ", Day of year: " << myDate.day_of_year() << endl;
	cout << "End of month: " << myDate.end_of_month() << endl;
	cout << "ISO 8601 week number: " << myDate.week_number() << ", Day of year: " << myDate.julian_day() << endl;
}


int main(){

    date myDate2(2011, May, 16); 
    Print(myDate2);


    std::string s("2009/1/9");                           // 9 January 2009.
    date myDate8(from_simple_string(s)); 
    Print(myDate8,"from simple string");

    // ISO 8601 extended format CCYY-MM-DD.
    std::string  s2("2009-10-9");                         // 9 October 2009.
    date myDate9(from_simple_string(s2)); 
    Print(myDate9,"from delimited string");
 }

 