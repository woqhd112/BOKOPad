#pragma once

#include <time.h>

namespace ComplexLibrary
{
	// + 연산은 초단위로 계산됨
	struct ComplexTimeTable
	{
		void Set(tm* t)
		{
			year = t->tm_year + 1900;
			month = t->tm_mon + 1;
			day = t->tm_mday;
			hour = t->tm_hour;
			minute = t->tm_min;
			second = t->tm_sec;
			day_of_week = t->tm_wday;
			am_pm = hour > 12 ? true : false;
		}

		int year = 0;
		int month = 0;
		int day = 0;
		int hour = 0;
		int minute = 0;
		int second = 0;
		/*
		일 = 0
		월 = 1
		화 = 2
		수 = 3
		목 = 4
		금 = 5
		토 = 6
		*/
		int day_of_week = 0;
		/*
		am = false
		pm = true
		*/
		bool am_pm = false;

		bool IsPM(ComplexTimeTable table)
		{
			return table.hour > 12;
		}

		int DayOfYear(ComplexTimeTable table)
		{
			int month = table.month;
			int year_error = 0;
			int return_day = 0;
			switch (table.month)
			{
			case 1:
				return_day = 31;
				break;
			case 2:
				return_day = 28;
				year_error = table.year - 2012;
				if (abs(year_error) % 4 == 0)	// 윤달
				{
					if (table.month == 2)
						return_day = 29;
				}
				break;
			case 3:
				return_day = 31;
				break;
			case 4:
				return_day = 30;
				break;
			case 5:
				return_day = 31;
				break;
			case 6:
				return_day = 30;
				break;
			case 7:
				return_day = 31;
				break;
			case 8:
				return_day = 31;
				break;
			case 9:
				return_day = 30;
				break;
			case 10:
				return_day = 31;
				break;
			case 11:
				return_day = 30;
				break;
			case 12:
				return_day = 31;
				break;
			default:
				break;
			}
			return return_day;
		}

		operator unsigned long long()
		{
			unsigned long long sum = 0;
			sum += this->second;
			sum += (this->minute * 60);
			sum += (this->hour * 60 * 60);
			sum += (this->day * 60 * 60 * 24);
			sum += (this->month * 60 * 60 * 24 * 30);
			sum += (this->year * 60 * 60 * 24 * 30 * 12);

			return sum;
		}

		bool operator == (ComplexTimeTable& timetable)
		{
			return (this->year == timetable.year && this->month == timetable.month && this->day == timetable.day && this->hour == timetable.hour && this->minute == timetable.minute && this->second == timetable.second);
		}

		bool operator != (ComplexTimeTable& timetable)
		{
			return (this->year != timetable.year || this->month != timetable.month || this->day != timetable.day || this->hour != timetable.hour || this->minute != timetable.minute || this->second != timetable.second);
		}

		ComplexTimeTable& operator -= (ComplexTimeTable& timetable)
		{
			this->second -= timetable.second;
			this->minute -= timetable.minute;
			this->hour -= timetable.hour;
			this->day -= timetable.day;
			this->day_of_week -= timetable.day_of_week;
			this->month -= timetable.month;
			this->year -= timetable.year;

			while (this->second <= -60 || this->second < 0)
			{
				this->second += 60;
				this->minute--;
			}

			while (this->minute <= -60 || this->minute < 0)
			{
				this->minute += 60;
				this->hour--;
			}

			while (this->hour <= -24 || this->hour < 0)
			{
				this->hour += 24;
				this->day--;
				this->day_of_week--;
			}

			while (this->day_of_week <= -7 || this->day_of_week < 0)
			{
				this->day_of_week += 7;
			}

			int dayOfYear = DayOfYear(*this);
			while (this->day < -dayOfYear || this->day < 0)
			{
				this->day += dayOfYear;
				this->month--;
			}

			while (this->month < -12 || this->month < 0)
			{
				this->month += 12;
				this->year--;
			}

			if (this->year < 0)
				this->year = 0;

			this->am_pm = IsPM(*this);

			return *this;
		}

		ComplexTimeTable operator - (ComplexTimeTable& timetable)
		{
			ComplexTimeTable tmp = *this;

			tmp.second -= timetable.second;
			tmp.minute -= timetable.minute;
			tmp.hour -= timetable.hour;
			tmp.day -= timetable.day;
			tmp.day_of_week -= timetable.day_of_week;
			tmp.month -= timetable.month;
			tmp.year -= timetable.year;

			while (tmp.second <= -60 || tmp.second < 0)
			{
				tmp.second += 60;
				tmp.minute--;
			}

			while (tmp.minute <= -60 || tmp.minute < 0)
			{
				tmp.minute += 60;
				tmp.hour--;
			}

			while (tmp.hour <= -24 || tmp.hour < 0)
			{
				tmp.hour += 24;
				tmp.day--;
				tmp.day_of_week--;
			}

			while (tmp.day_of_week <= -7 || tmp.day_of_week < 0)
			{
				tmp.day_of_week += 7;
			}

			int dayOfYear = DayOfYear(tmp);
			while (tmp.day < -dayOfYear || tmp.day < 0)
			{
				tmp.day += dayOfYear;
				tmp.month--;
			}

			while (tmp.month < -12 || tmp.month < 0)
			{
				tmp.month += 12;
				tmp.year--;
			}

			if (tmp.year < 0)
				tmp.year = 0;

			tmp.am_pm = IsPM(tmp);

			return tmp;
		}

		ComplexTimeTable& operator += (ComplexTimeTable& timetable)
		{
			this->second += timetable.second;
			this->minute += timetable.minute;
			this->hour += timetable.hour;
			this->day += timetable.day;
			this->day_of_week += timetable.day_of_week;
			this->month += timetable.month;
			this->year += timetable.year;

			while (this->second >= 60)
			{
				this->second -= 60;
				this->minute++;
			}

			while (this->minute >= 60)
			{
				this->minute -= 60;
				this->hour++;
			}

			while (this->hour >= 24)
			{
				this->hour -= 24;
				this->day++;
				this->day_of_week++;
			}

			while (this->day_of_week >= 7)
			{
				this->day_of_week -= 7;
			}

			int dayOfYear = DayOfYear(*this);
			while (this->day > dayOfYear)
			{
				this->day -= dayOfYear;
				this->month++;
			}

			while (this->month > 12)
			{
				this->month -= 12;
				this->year++;
			}

			this->am_pm = IsPM(*this);

			return *this;
		}

		ComplexTimeTable operator + (ComplexTimeTable& timetable)
		{
			ComplexTimeTable tmp;
			tmp = *this;

			tmp.second += timetable.second;
			tmp.minute += timetable.minute;
			tmp.hour += timetable.hour;
			tmp.day += timetable.day;
			tmp.day_of_week += timetable.day_of_week;
			tmp.month += timetable.month;
			tmp.year += timetable.year;

			while (tmp.second >= 60)
			{
				tmp.second -= 60;
				tmp.minute++;
			}

			while (tmp.minute >= 60)
			{
				tmp.minute -= 60;
				tmp.hour++;
			}

			while (tmp.hour >= 24)
			{
				tmp.hour -= 24;
				tmp.day++;
				tmp.day_of_week++;
			}

			while (tmp.day_of_week >= 7)
			{
				tmp.day_of_week -= 7;
			}

			int dayOfYear = DayOfYear(tmp);
			while (tmp.day > dayOfYear)
			{
				tmp.day -= dayOfYear;
				tmp.month++;
			}

			while (tmp.month > 12)
			{
				tmp.month -= 12;
				tmp.year++;
			}

			tmp.am_pm = IsPM(tmp);

			return tmp;
		}

		ComplexTimeTable& operator -= (int second)
		{
			this->second -= second;
			while (this->second <= -60 || this->second < 0)
			{
				this->second += 60;
				this->minute--;
			}

			while (this->minute <= -60 || this->minute < 0)
			{
				this->minute += 60;
				this->hour--;
			}

			while (this->hour <= -24 || this->hour < 0)
			{
				this->hour += 24;
				this->day--;
				this->day_of_week--;
			}

			while (this->day_of_week <= -7 || this->day_of_week < 0)
			{
				this->day_of_week += 7;
			}

			int dayOfYear = DayOfYear(*this);
			while (this->day < -dayOfYear || this->day < 0)
			{
				this->day += dayOfYear;
				this->month--;
			}

			while (this->month < -12 || this->month < 0)
			{
				this->month += 12;
				this->year--;
			}

			if (this->year < 0)
				this->year = 0;

			this->am_pm = IsPM(*this);

			return *this;
		}

		ComplexTimeTable operator - (int second)
		{
			ComplexTimeTable tmp = *this;

			tmp.second -= second;
			while (tmp.second <= -60 || tmp.second < 0)
			{
				tmp.second += 60;
				tmp.minute--;
			}

			while (tmp.minute <= -60 || tmp.minute < 0)
			{
				tmp.minute += 60;
				tmp.hour--;
			}

			while (tmp.hour <= -24 || tmp.hour < 0)
			{
				tmp.hour += 24;
				tmp.day--;
				tmp.day_of_week--;
			}

			while (tmp.day_of_week <= -7 || tmp.day_of_week < 0)
			{
				tmp.day_of_week += 7;
			}

			int dayOfYear = DayOfYear(tmp);
			while (tmp.day < -dayOfYear || tmp.day < 0)
			{
				tmp.day += dayOfYear;
				tmp.month--;
			}

			while (tmp.month < -12 || tmp.month < 0)
			{
				tmp.month += 12;
				tmp.year--;
			}

			if (tmp.year < 0)
				tmp.year = 0;

			tmp.am_pm = IsPM(tmp);

			return tmp;
		}

		ComplexTimeTable& operator += (int second)
		{
			this->second += second;
			while (this->second >= 60)
			{
				this->second -= 60;
				this->minute++;
			}

			while (this->minute >= 60)
			{
				this->minute -= 60;
				this->hour++;
			}

			while (this->hour >= 24)
			{
				this->hour -= 24;
				this->day++;
				this->day_of_week++;
			}

			while (this->day_of_week >= 7)
			{
				this->day_of_week -= 7;
			}

			int dayOfYear = DayOfYear(*this);
			while (this->day > dayOfYear)
			{
				this->day -= dayOfYear;
				this->month++;
			}

			while (this->month > 12)
			{
				this->month -= 12;
				this->year++;
			}

			this->am_pm = IsPM(*this);

			return *this;
		}

		ComplexTimeTable operator + (int second)
		{
			ComplexTimeTable tmp = *this;

			tmp.second += second;
			while (tmp.second >= 60)
			{
				tmp.second -= 60;
				tmp.minute++;
			}

			while (tmp.minute >= 60)
			{
				tmp.minute -= 60;
				tmp.hour++;
			}

			while (tmp.hour >= 24)
			{
				tmp.hour -= 24;
				tmp.day++;
				tmp.day_of_week++;
			}

			while (tmp.day_of_week >= 7)
			{
				tmp.day_of_week -= 7;
			}

			int dayOfYear = DayOfYear(tmp);
			while (tmp.day > dayOfYear)
			{
				tmp.day -= dayOfYear;
				tmp.month++;
			}

			while (tmp.month > 12)
			{
				tmp.month -= 12;
				tmp.year++;
			}

			tmp.am_pm = IsPM(tmp);

			return tmp;
		}
	};

	class ComplexTime
	{
	public:

		

		ComplexTime()
		{

		}

		virtual ~ComplexTime()
		{

		}

		static ComplexTimeTable GetCurrentTimes()
		{
			time_t times = time(NULL);
			tm t;
			localtime_s(&t, &times);
			ComplexTimeTable timeTable;
			timeTable.Set(&t);

			return timeTable;
		}

	private:

	};
}