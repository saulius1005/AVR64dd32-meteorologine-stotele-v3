/*
 * Windows.c
 *
 * Created: 2024-12-11 21:50:34
 * Author: Saulius
 *
 * This file provides functions for displaying and managing time, date, location (latitude, longitude),
 * altitude, and timezone on the screen. It includes extraction and formatting of specific data for display.
 */

#include "Settings.h"

/**
 * @brief Displays the current date, time, timezone, altitude, latitude, and longitude on the screen.
 * 
 * This function uses the `newTimeAndPlace` array for displaying time and location data. It also uses the
 * `arrow` array to visually highlight the selected field for editing, based on the `step`.
 * 
 * @param newTimeAndPlace Array containing the digits of time and place data.
 * @param arrow Array to display arrows ('^' or 's') indicating the current field to be edited.
 * @param step The index of the current field being edited.
 */
void displayDateTimeAndLocation(uint8_t *newTimeAndPlace, char *arrow, uint8_t *step) {
    uint8_t simbol = 0, simbol2 = 0;

    // Display date and time information in the format: t:YYYY-MM-DD HH:MM:SS
    screen_write_formatted_text("t:%d%d%d%d-%d%d-%d%d %d%d:%d%d:%d%d", 0, ALIGN_CENTER,
        newTimeAndPlace[0], newTimeAndPlace[1], newTimeAndPlace[2], newTimeAndPlace[3],
        newTimeAndPlace[4], newTimeAndPlace[5], newTimeAndPlace[6], newTimeAndPlace[7],
        newTimeAndPlace[8], newTimeAndPlace[9], newTimeAndPlace[10], newTimeAndPlace[11],
        newTimeAndPlace[12], newTimeAndPlace[13], 0); //last 0 for hunderts of seconds

    // Display arrows indicating current step for time and date
    screen_write_formatted_text("  %c%c%c%c %c%c %c%c %c%c %c%c %c%c", 1, ALIGN_CENTER,
        arrow[0], arrow[1], arrow[2], arrow[3], arrow[4], arrow[5], arrow[6], arrow[7],
        arrow[8], arrow[9], arrow[10], arrow[11], arrow[12], arrow[13]);

    // Timezone sign (positive or negative)
    simbol = (newTimeAndPlace[14] == 0) ? '+' : '-';
    simbol2 = (newTimeAndPlace[17] == 0) ? '+' : '-';

    // Display timezone and altitude information
    // screen_write_formatted_text("l.z: %c%d%d   alt: %c%d%d%d%d", 2, ALIGN_CENTER, simbol, newTimeAndPlace[15], newTimeAndPlace[16], simbol2, newTimeAndPlace[18], newTimeAndPlace[19], newTimeAndPlace[20], newTimeAndPlace[21]); //laiko zona -12:+12 //Lithuanian
    screen_write_formatted_text("t.z: %c%d%d   alt: %c%d%d%d%d", 2, ALIGN_CENTER,
        simbol, newTimeAndPlace[15], newTimeAndPlace[16], simbol2, newTimeAndPlace[18],
        newTimeAndPlace[19], newTimeAndPlace[20], newTimeAndPlace[21]); //English

    // Display arrows for timezone and altitude
    screen_write_formatted_text("     %c%c%c        %c%c%c%c%c", 3, ALIGN_CENTER,
        arrow[14], arrow[15], arrow[16], arrow[17], arrow[18], arrow[19], arrow[20], arrow[21]);

    // Latitude information with symbol (positive or negative)
    simbol = (newTimeAndPlace[22] == 0) ? '+' : '-';
    screen_write_formatted_text("lat:        %c%d%d.%d%d%d%d°", 4, ALIGN_RIGHT,
        simbol, newTimeAndPlace[23], newTimeAndPlace[24], newTimeAndPlace[25],
        newTimeAndPlace[26], newTimeAndPlace[27], newTimeAndPlace[28]);

    // Display arrows for latitude
    screen_write_formatted_text("%c%c%c %c%c%c%c ", 5, ALIGN_RIGHT,
        arrow[22], arrow[23], arrow[24], arrow[25], arrow[26], arrow[27], arrow[28]);

    // Longitude information with symbol (positive or negative)
    simbol = (newTimeAndPlace[29] == 0) ? '+' : '-';
    screen_write_formatted_text("lng:       %c%d%d%d.%d%d%d%d°", 6, ALIGN_RIGHT,
        simbol, newTimeAndPlace[30], newTimeAndPlace[31], newTimeAndPlace[32], newTimeAndPlace[33],
        newTimeAndPlace[34], newTimeAndPlace[35], newTimeAndPlace[36]);

    // Display arrows for longitude
    screen_write_formatted_text("%c%c%c%c %c%c%c%c ", 7, ALIGN_RIGHT,
       arrow[29], arrow[30], arrow[31], arrow[32], arrow[33], arrow[34], arrow[35], arrow[36]);

    // Update arrow visualization for selected digit
    for (uint8_t i = 0; i < 38; i++) {
        if (i == *step) {
            if (i < 37)
                arrow[i] = '^';
            else
                arrow[i - 1] = 's';
        }
        else
            arrow[i] = ' ';
    }
}

/**
 * @brief Extracts a specific digit from the time data in the Date_Clock structure.
 * 
 * @param step The index of the digit to extract from the time data.
 * @return The extracted digit.
 */
uint8_t extractDigitFromTime(uint8_t step) {
    switch (step) {
        case 0: return (Date_Clock.year / 1000) % 10;
        case 1: return (Date_Clock.year / 100) % 10;
        case 2: return (Date_Clock.year / 10) % 10;
        case 3: return Date_Clock.year % 10;
        case 4: return (Date_Clock.month / 10) % 10;
        case 5: return Date_Clock.month % 10;
        case 6: return (Date_Clock.day / 10) % 10;
        case 7: return Date_Clock.day % 10;
        case 8: return (Date_Clock.hour / 10) % 10;
        case 9: return Date_Clock.hour % 10;
        case 10: return (Date_Clock.minute / 10) % 10;
        case 11: return Date_Clock.minute % 10;
        case 12: return (Date_Clock.second / 10) % 10;
        case 13: return Date_Clock.second % 10;
        default: return 0;
    }
}

/**
 * @brief Extracts a specific digit from the timezone data in the Date_Clock structure.
 * 
 * @param step The index of the digit to extract from the timezone data.
 * @return The extracted digit.
 */
uint8_t extractDigitFromTimeZone(uint8_t step) {
    switch (step) {
        case 0: return (Date_Clock.timezone < 0) ? 1: 0;
        case 1: return (abs(Date_Clock.timezone) / 10) % 10;
        case 2: return abs(Date_Clock.timezone) % 10;
        default: return 0;
    }
}

/**
 * @brief Extracts a specific digit from the altitude data in the Date_Clock structure.
 * 
 * @param step The index of the digit to extract from the altitude data.
 * @return The extracted digit.
 */
uint8_t extractDigitFromAltitude(uint8_t step) {
    switch (step) {
        case 0: return (Date_Clock.altitude < 0) ? 1 : 0;
        case 1: return (abs(Date_Clock.altitude) / 1000) % 10;
        case 2: return (abs(Date_Clock.altitude) / 100) % 10;
        case 3: return (abs(Date_Clock.altitude) / 10) % 10;
        case 4: return abs(Date_Clock.altitude) % 10;
        default: return 0;
    }
}

/**
 * @brief Extracts a specific digit from the latitude or longitude data in the Date_Clock structure.
 * 
 * @param intgpart The number of integer digits to extract from the value.
 * @param number The latitude or longitude value.
 * @param step The index of the digit to extract from the location data.
 * @return The extracted digit.
 */
uint8_t extractDigitFromLocation(uint8_t intgpart, double number, uint8_t step) {
    if (step == 0) return (number < 0) ? 1 : 0; // Þenklas

    number = fabs(number);
    uint8_t integer_part = (uint8_t)number;
    uint32_t fractional_part = (uint32_t)round((number - integer_part) * 1e4);

    if (step <= intgpart) {
        // Sveikoji dalis
        uint32_t divisor = 1;
        for (uint8_t i = 0; i < intgpart - step; ++i)
            divisor *= 10;
        return (integer_part / divisor) % 10;
    } 
    else {
        // Trupmeninë dalis
        uint32_t divisor = 1;
        for (uint8_t i = 0; i < 4 + intgpart - step; ++i)
            divisor *= 10;
        return (fractional_part / divisor) % 10;
    }
}

/**
 * @brief Extracts a specific digit based on the step index from the time, timezone, altitude, or location data.
 * 
 * @param step The index of the digit to extract.
 * @param newTimeAndPlace Array to store the extracted digits.
 * @return The next step index.
 */
uint8_t DataExtraction(uint8_t step, uint8_t *newTimeAndPlace) {
    if (step < 14) 
        newTimeAndPlace[step] = extractDigitFromTime(step);
    else if (step >= 14 && step < 17) 
        newTimeAndPlace[step] = extractDigitFromTimeZone(step - 14);
    else if (step >= 17 && step < 22) 
        newTimeAndPlace[step] = extractDigitFromAltitude(step - 17);
    else if (step >= 22 && step < 29) 
        newTimeAndPlace[step] = extractDigitFromLocation(2, Date_Clock.latitude, step - 22); 
    else if (step >= 29 && step < 37) 
        newTimeAndPlace[step] = extractDigitFromLocation(3, Date_Clock.longitude, step - 29); 
    step++;                
    return step;
}

/**
 * @brief Function to calculate numerical values.
 * 
 * This function processes an array of digits and calculates the corresponding integer value.
 * @param digits Array of digits.
 * @param length Length of the digits array.
 * @return Calculated integer value.
 */
int64_t calculateValue(uint8_t *digits, uint8_t length) { 
    int64_t result = 0;
    for (uint8_t i = 0; i < length; i++) {
        result = result * 10 + digits[i];
    }
    return result;
}

/**
 * @brief Function to apply the sign to a value.
 * 
 * Based on the sign digit, it either returns the value as it is or applies a negative sign.
 * @param signDigit Sign digit (0 for positive, 1 for negative).
 * @param value The value to which the sign will be applied.
 * @return The value with the applied sign.
 */
int applySign(uint8_t signDigit, int value) { 
    return (signDigit == 0) ? value : -value;
}

/**
 * @brief Function to apply the sign to a double value.
 * 
 * This function behaves similarly to applySign but for floating-point values.
 * @param signDigit Sign digit (0 for positive, 1 for negative).
 * @param value The value to which the sign will be applied.
 * @return The value with the applied sign.
 */
double applySignDouble(uint8_t signDigit, double value) {
    return (signDigit == 0) ? value : -value;
}

/**
 * @brief Validates and processes new time and place data.
 * 
 * This function checks the validity of the new date, time, timezone, altitude, latitude, and longitude.
 * It updates the step variable based on which field needs correction.
 * @param newTimeAndPlace Array containing the new time and place data.
 * @param step Pointer to the current step, which will be updated.
 */
void ValidateNewData(uint8_t * newTimeAndPlace, uint8_t *step)
{
    uint8_t lastAction = 21; // In the end, this variable decides if you will go to the main menu with success or not if you put wrong digits // default value is to stay at this page (DateAndLocationChangeWindow)
    int8_t newTimeZone = applySign(newTimeAndPlace[14], calculateValue(&newTimeAndPlace[15], 2));
    int16_t newAltitude = applySign(newTimeAndPlace[17], calculateValue(&newTimeAndPlace[18], 4));
    double newLatitude = applySignDouble(newTimeAndPlace[22], calculateValue(&newTimeAndPlace[23], 6) / 10000.0);
    double newLongitude = applySignDouble(newTimeAndPlace[29], calculateValue(&newTimeAndPlace[30], 7) / 10000.0);

    if (isValidDateTime(
        ((uint16_t)newTimeAndPlace[0] * 1000) + ((uint16_t)newTimeAndPlace[1] * 100) + (newTimeAndPlace[2] * 10) + newTimeAndPlace[3], // year
        (newTimeAndPlace[4] * 10) + newTimeAndPlace[5],  // month
        (newTimeAndPlace[6] * 10) + newTimeAndPlace[7],  // day
        (newTimeAndPlace[8] * 10) + newTimeAndPlace[9],  // hour
        (newTimeAndPlace[10] * 10) + newTimeAndPlace[11], // minute
        (newTimeAndPlace[12] * 10) + newTimeAndPlace[13]  // seconds
    ) == 1) {
        //screen_write_formatted_text("Neteisinga data", 3, ALIGN_CENTER); // Lithuanian
        //screen_write_formatted_text("arba laikas !", 4, ALIGN_CENTER); // Lithuanian
        screen_write_formatted_text("Wrong date", 3, ALIGN_CENTER); // English
        screen_write_formatted_text("or time !", 4, ALIGN_CENTER); // English
        *step = 0; // put marker on first date number
    }
    else if(isValidTimeZone(newTimeZone) == 1){
        //screen_write_formatted_text("Laiko zonos klaida !", 3, ALIGN_CENTER); // Lithuanian
        screen_write_formatted_text("Time zone error !", 3, ALIGN_CENTER); // English
        *step = 14; // put marker on timezone
    }
    else if(isValidAltitude(newAltitude) == 1){
        //screen_write_formatted_text("Neteisingas aukðis", 3, ALIGN_CENTER); // Lithuanian
        //screen_write_formatted_text("virð jûros lygio !", 4, ALIGN_CENTER); // Lithuanian
        screen_write_formatted_text("Wrong altitude", 3, ALIGN_CENTER); // English
        screen_write_formatted_text("over sea level !", 4, ALIGN_CENTER); // English
        *step = 17; // put marker on altitude
    }
    else if(isValidLatitude(newLatitude) == 1){
        //screen_write_formatted_text("Neteisinga platuma!", 3, ALIGN_CENTER); // Lithuanian
        screen_write_formatted_text("Wrong latitude!", 3, ALIGN_CENTER); // English
        *step = 22; // put marker on latitude
    }
    else if(isValidLongitude(newLongitude) == 1){
        //screen_write_formatted_text("Neteisinga ilguma!", 3, ALIGN_CENTER); // Lithuanian
        screen_write_formatted_text("Wrong longitude!", 3, ALIGN_CENTER); // English
        *step = 30; // put marker on longitude
    }
    else{                                        
        *step = 0;
        lastAction = 0; // all good, go to main window
        //screen_write_formatted_text("Iðsaugota :D", 3, ALIGN_CENTER); // Lithuanian // display success message
        screen_write_formatted_text("Saved :D", 3, ALIGN_CENTER); // English
        PORTF.OUTCLR = PIN2_bm; // Ready to set time and location
        _delay_ms(10); // wait some for clock device to end current action
        USART_printf(1, "<%d%d%d%d%d%d%d%d%d%d%d%d%d%d0|%d|%3.4f|%3.4f>\r\n", // sending new data to clock device
        newTimeAndPlace[0], newTimeAndPlace[1], newTimeAndPlace[2], newTimeAndPlace[3],
        newTimeAndPlace[4], newTimeAndPlace[5], newTimeAndPlace[6], newTimeAndPlace[7],
        newTimeAndPlace[8], newTimeAndPlace[9], newTimeAndPlace[10], newTimeAndPlace[11],
        newTimeAndPlace[12], newTimeAndPlace[13], // data ir laikas
        newTimeZone, // time zone
        newLatitude, // latitude
        newLongitude // longitude
        );
        Date_Clock.altitude = newAltitude; // and save to this device altitude
        PORTF.OUTSET = PIN2_bm; // Time and location is set, continue normal clock work
    }
    _delay_ms(1000); // show any message for 1 second
    Keypad3x4.key_held = lastAction; // going to main window if success, and stay if data is wrong
}

/**
 * @brief Displays parameters when there are no errors.
 * 
 * This function shows parameters such as date, time, azimuth, elevation, and location data when there are no errors.
 * @param upDown The current step to determine which parameters to display.
 */
void parametersWOerror(uint8_t upDown) { 
    if(upDown < 1){
        screen_write_formatted_text("t:", upDown, ALIGN_LEFT);
        screen_write_formatted_text("%4d%02d%02d%02d%02d%02d%d", upDown, ALIGN_RIGHT,
        Date_Clock.year,
        Date_Clock.month,
        Date_Clock.day,
        Date_Clock.hour,
        Date_Clock.minute,
        Date_Clock.second,
        Date_Clock.hunderts
        );
    }
    if (upDown < 2){
        screen_write_formatted_text("az:°", 1-upDown, ALIGN_LEFT);
        screen_write_formatted_text("%3.4f",1-upDown, ALIGN_RIGHT, SUN.azimuth);
    }
    if(upDown < 3){
        screen_write_formatted_text("el.°:", 2-upDown, ALIGN_LEFT);
        screen_write_formatted_text("%3.4f", 2-upDown, ALIGN_RIGHT, SUN.elevation);
    }
    if (upDown < 4)
    {
        //screen_write_formatted_text("kor. el.°:", 3-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("adj. el.°:", 3-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%3.4f", 3-upDown, ALIGN_RIGHT, SUN.adjelevation);
    }
    if (upDown > 4)
    {
        //screen_write_formatted_text("laik. z:", 12-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("t.z:", 12-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%d", 12-upDown, ALIGN_RIGHT, Date_Clock.timezone);
    }
    if (upDown > 5)
    {
        //screen_write_formatted_text("plat. °:", 13-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("lat. °:", 13-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%2.4f", 13-upDown, ALIGN_RIGHT, Date_Clock.latitude);
    }
    if (upDown > 6)
    {
        //screen_write_formatted_text("ilg. °:", 14-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("long. °:", 14-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%3.4f", 14-upDown, ALIGN_RIGHT, Date_Clock.longitude);
    }
}

/**
 * @brief Displays parameters when there are errors.
 * 
 * This function shows parameters related to sensor readings and altitude when there are errors.
 * @param upDown The current step to determine which parameters to display.
 */
void parametersWerror(uint8_t upDown){ 
    if (upDown < 5)
    {
        screen_write_formatted_text("bmp T C°:", 4-upDown, ALIGN_LEFT);
        screen_write_formatted_text("%3.2f", 4-upDown, ALIGN_RIGHT, BMP280.Temperature);
    }
    if (upDown < 6)
    {
        screen_write_formatted_text("sht T C°:", 5-upDown, ALIGN_LEFT);
        screen_write_formatted_text("%3.2f", 5-upDown, ALIGN_RIGHT, SHT21.T);
    }
    if (upDown < 7)
    {
        screen_write_formatted_text("p hPa:", 6-upDown, ALIGN_LEFT);
        screen_write_formatted_text("%3.4f", 6-upDown, ALIGN_RIGHT, BMP280.Pressure);
    }
    if (upDown < 8)
    {
        screen_write_formatted_text("rh %%:", 7-upDown, ALIGN_LEFT);
        screen_write_formatted_text("%3.2f", 7-upDown, ALIGN_RIGHT, SHT21.RH);
    }
    if (upDown > 0 && upDown <= 8)
    {
        //screen_write_formatted_text("nk.aukðt. m:", 8-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("not adj.alt. m:", 8-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%4.1f", 8-upDown, ALIGN_RIGHT, Altitude.UNCOMP);
    }
    if (upDown > 1 && upDown <= 9)
    {
        //screen_write_formatted_text("k.aukðt. m:", 9-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("adj.alt. m:", 9-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%4.1f", 9-upDown, ALIGN_RIGHT, Altitude.COMP);
    }
    if (upDown > 2 && upDown <= 10)
    {
        //screen_write_formatted_text("vid.aukðt. m:", 10-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("avg.alt. m:", 10-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%4.1f", 10-upDown, ALIGN_RIGHT, Altitude.AVRG);
    }
    if (upDown > 3 && upDown <= 11)
    {
        //screen_write_formatted_text("real.aukðt. m:", 11-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("rl.alt. m:", 11-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%d", 11-upDown, ALIGN_RIGHT, Date_Clock.altitude);
    }
    if (upDown > 7 && upDown <= 12)
    {
        //screen_write_formatted_text("v.g. m/s:", 15-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("w.s. m/s:", 15-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%d", 15-upDown, ALIGN_RIGHT, Wind.speed);
    }
    if (upDown > 8 && upDown <= 13)
    {
        //screen_write_formatted_text("v.k.nr:", 16-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("w.d.no:", 16-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%d", 16-upDown, ALIGN_RIGHT, Wind.direction);
    }
    if (upDown > 9 && upDown <= 14)
    {
        //screen_write_formatted_text("a.l. mV:", 17-upDown, ALIGN_LEFT); // Lithuanian
        screen_write_formatted_text("l.l. mV:", 17-upDown, ALIGN_LEFT); // English
        screen_write_formatted_text("%d", 17-upDown, ALIGN_RIGHT, SUN.sunlevel);
    }
}

/**
 * @brief Actions after pressing the back button (* key).
 * 
 * Clears the screen and resets the key held state.
 */
void backButton() { 
    if(Keypad3x4.key == 10){
        screen_clear();
        Keypad3x4.key_held = 0;
    }
}

/**
 * @brief Handles clock error display
 * 
 * This function displays an error message when a clock error occurs.
 * 
 * @param place The position on the screen where the error message will appear
 */
void ClockError(int8_t place)
{
	//screen_write_formatted_text("Laikrodþio klaida!!!", place, ALIGN_CENTER); //Lithuanian
	screen_write_formatted_text("Clock error!!!", place, ALIGN_CENTER); //English 
}

/**
 * @brief Handles the change of date and location
 * 
 * This function allows the user to change the date, time, and location settings.
 * It validates the new data and displays appropriate messages based on validation results.
 */
void DateAndLocationChangeWindow()
{
	static uint8_t newTimeAndPlace[38], // For data storage
				   step = 0;          // Step of changed data
	static char arrow[38];            // For selection visualization of changed data
	displayDateTimeAndLocation(newTimeAndPlace, arrow, &step);
	if (Date_Clock.error == 1) 
		ClockError(3);
	else { // If clock is working correctly
		if ((Keypad3x4.key != 10) && (Keypad3x4.key != 12)) { //if 0-9 are pressed
			while (scan_keypad() != 0);
			if (Keypad3x4.key != 0) {
				newTimeAndPlace[step] = (Keypad3x4.key == 11) ? 0 : Keypad3x4.key; // Handle key 0
				step++;
			}
		} 
		else {
			if (Keypad3x4.key == 10 && step > 0) { // if pressed * key
				step--;
				//newTimeAndPlace[*step] = 0; // uncomment if need to reset digit to 0 after moving back one *step
			} 
			else if (Keypad3x4.key == 12) { //if pressed # key
				step = DataExtraction(step, newTimeAndPlace);			
				if (step == 38) { //All digits are set. Now checking boundaries of all variables (date/time, time zone, altitude, latitude, and longitude)
					screen_clear(); //clear screen
					ValidateNewData(newTimeAndPlace, &step); //Validate date and show error or success message
					screen_clear(); //clear screen
				}
			}
		}
	}
	if(step == 0)
		backButton(); //going back to main window
}

/**
 * @brief Displays parameters when there are no errors
 * 
 * This function displays parameters such as date, time, altitude, latitude, etc., when no errors are present.
 * 
 * @param upDown The parameter indicating the up or down selection for scrolling through parameters
 */
void ParameterViewWindow()
{
	static uint8_t upDown = 0;
			if ((Keypad3x4.key == 8 && upDown < 10) || (Keypad3x4.key == 2 && upDown > 0)) {
				while (scan_keypad() != 0); // Wait until the key is released
				upDown += (Keypad3x4.key == 8) ? 1 : -1;
				screen_clear();
			}
			if (Date_Clock.error == 1) {
				int8_t place = 0;
				if(upDown >= 5 && upDown < 8)
					place = 7;
				else if(upDown >= 8 && upDown)
					place = 14- upDown;
				if(upDown != 4) //if updown is 4 all parameters showing, because at that part of window all parameters are not dependent on data from the clock device
				ClockError(place);	//also scrolling error message if present
			} 
			else 
				parametersWOerror(upDown); //showing those parameters only if the clock device is working normally without errors
		parametersWerror(upDown); //showing these parameters all the time if error is present
		backButton(); // Going back to the main window
}

/**
 * @brief Displays the main window with important parameters
 * 
 * This function displays the most important parameters like temperature, pressure, humidity, wind speed, etc.
 * 
 */
void MainWindow()
{
	//screen_write_formatted_text("Temperatûra:", 0, ALIGN_LEFT);//Lithuanian
	screen_write_formatted_text("Temperature:", 0, ALIGN_LEFT); //English
	screen_write_formatted_text("%2.02fC°", 0, ALIGN_RIGHT, SHT21.T);

	//screen_write_formatted_text("Slëgis:", 1, ALIGN_LEFT);//Lithuanian
	screen_write_formatted_text("Pressure:", 1, ALIGN_LEFT);//English
	screen_write_formatted_text("%4.02fhPa", 1, ALIGN_RIGHT, BMP280.Pressure);

	//screen_write_formatted_text("Drëgmë:", 2, ALIGN_LEFT);//Lithuanian
	screen_write_formatted_text("Humidity:", 2, ALIGN_LEFT);//English
	screen_write_formatted_text("%3.02f%%", 2, ALIGN_RIGHT, SHT21.RH);

	//screen_write_formatted_text("Vëjas:    ", 3, ALIGN_LEFT);//Lithuanian
	screen_write_formatted_text("Wind:    ", 3, ALIGN_LEFT);//English
	screen_write_formatted_text("%s", 3, ALIGN_CENTER, WindDirNames());
	screen_write_formatted_text("%2dm/s", 3, ALIGN_RIGHT, Wind.speed);

	//screen_write_formatted_text("Apð.lygis:", 4, ALIGN_LEFT);//Lithuanian
	screen_write_formatted_text("Light level:", 4, ALIGN_LEFT);//English
	screen_write_formatted_text("%4dmV", 4, ALIGN_RIGHT, SUN.sunlevel);

	screen_write_formatted_text("---------------------", 5, ALIGN_LEFT);

	if(Date_Clock.error == 1)
		ClockError(6);
	else{
		screen_write_formatted_text("%4d-%02d-%02d A:", 6, ALIGN_LEFT, Date_Clock.year, Date_Clock.month, Date_Clock.day);
		screen_write_formatted_text("%3.2f°", 6, ALIGN_RIGHT, SUN.adjazimuth);
		screen_write_formatted_text("  %02d:%02d:%02d E:", 7, ALIGN_LEFT, Date_Clock.hour, Date_Clock.minute, Date_Clock.second);
		screen_write_formatted_text("%2.2f°", 7, ALIGN_RIGHT, SUN.adjelevation);
	}
}

/**
 * @brief Main function to handle window switching based on keypress
 * 
 * This function checks the key press and switches between different windows such as main window, date and location change window, or parameters view window.
 */
void windows() {
	if(Keypad3x4.key_held == 21) //long press 1 menu- Date and time change window
		DateAndLocationChangeWindow();
	else if(Keypad3x4.key_held == 22) //long press 2 menu- all parameters view window
		ParameterViewWindow();		
	else //if long press any other button in any window, go to mainWindow
		MainWindow(); // All roads lead to MainWindow, not to Rome :D //Main window shows most important data: pressure, temperature, humidity, adjusted altitude and elevation, wind speed and direction, light level
};
