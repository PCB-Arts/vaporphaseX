#include "unitConverter.hpp"


float celsiusToUnit(DegreeCelcius degreeCelsius, TemperatureUnit targetUnit){
	if(targetUnit == CELSIUS){
		return degreeCelsius;
	}else if(targetUnit == FAHRENHEIT){
		return degreeCelsius * 1.8 + 32.0;
	}else{
		return -1;
	}
}

DegreeCelcius unitToCelsius(float value, TemperatureUnit inputUnit){
	if(inputUnit == CELSIUS){
		return value;
	}else if(inputUnit == FAHRENHEIT){
		return (value - 32.0) * 1.8;
	}else{
		return -1;
	}
}
