

#ifndef TOUCHGFX_GUI_INCLUDE_UNITCONVERTER_HPP_
#define TOUCHGFX_GUI_INCLUDE_UNITCONVERTER_HPP_

enum TemperatureUnit{
	CELSIUS,
	FAHRENHEIT
};

typedef float DegreeCelcius;

float celsiusToUnit(DegreeCelcius degreeCelsius, TemperatureUnit targetUnit);
DegreeCelcius unitToCelsius(float value, TemperatureUnit inputUnit);


#endif /* TOUCHGFX_GUI_INCLUDE_UNITCONVERTER_HPP_ */
