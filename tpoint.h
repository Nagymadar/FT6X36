#pragma once


struct TPoint
{
	public:
		#define maxDeviation 5

		int16_t x;
		int16_t y;

		TPoint snap(const uint8_t& grid) {
			int16_t new_x = x + grid / 2;
			int16_t new_y = y + grid / 2;
			
			TPoint ret;
			ret.x = new_x - new_x % grid;
			ret.y = new_y - new_y % grid;

			return ret;
		};

		/* something wrong
		TPoint within(const int16_t& x0, const int16_t& x1, const int16_t& y0, const int16_t& y1) {
			TPoint ret;
			ret.x = x;
			ret.y = y;

			if (ret.x < x0) ret.x = x0;
			if (ret.y < y0) ret.y = y0;
			if (ret.x > x1) ret.x = x1;
			if (ret.y > y1) ret.y = y1;

			return ret;
		};*/

		friend bool operator==(const TPoint& value1, const TPoint& value2) {
			return abs(value1.x - value2.x) <= maxDeviation && abs(value1.y - value2.y) <= maxDeviation;  // remark: Tpoint == operator has tolerance 
		}

		friend bool operator!=(const TPoint& value1, const TPoint& value2) {
			return !(value1 == value2);
		}

		friend TPoint operator+(const TPoint& value1, const TPoint& value2) {
			TPoint sum;
			sum.x = value1.x + value2.x; 
			sum.y = value1.y + value2.y; 
			return sum;
		}

		friend TPoint& operator+=(TPoint& value1, const TPoint& value2) {
			value1.x += value2.x; 
			value1.y += value2.y; 
			return value1;
		}

		friend TPoint operator-(const TPoint& value1, const TPoint& value2) {
			TPoint sub;
			sub.x = value1.x - value2.x; 
			sub.y = value1.y - value2.y; 
			return sub;
		}

		friend TPoint& operator-=(TPoint& value1, const TPoint& value2) {
			value1.x -= value2.x; 
			value1.y -= value2.y; 
			return value1;
		}


};

