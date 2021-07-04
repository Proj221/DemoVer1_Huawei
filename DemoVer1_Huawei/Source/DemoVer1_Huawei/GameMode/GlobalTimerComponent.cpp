#include "GlobalTimerComponent.h"


// Sets default values for this component's properties
UGlobalTimerComponent::UGlobalTimerComponent()
{
    // Disable tick for this component
    PrimaryComponentTick.bCanEverTick = false;

    // Retrieve actor component's Parent Actor
    Parent_Actor = GetOwner();

    // Assume no alterations to the rate and AM/PM output desired initially
    Time_Altered = false;
    Military_Time = false;

    // Set our variable values to standard values
    SecondsReset = 60;
    SecondsResetValue = 0;
    MinutesReset = 60;
    MinutesResetValue = 0;
    HoursReset = 24;
    HoursResetValue = 0;
    DaysReset = 32;
    MonthsReset = 12;
    Sun_Degrees = 0.0f;
    Delta_Degrees = 0.1f;
    Range = 180.0f;
    YearToDateSeconds = 0;
    TotalSecondsPerYear = 31536000;
}

// Called when the game starts
void UGlobalTimerComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UGlobalTimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Set date and time values for timer function
void UGlobalTimerComponent::Earth_Genesis(UPARAM()FStandard_Calendar Standard_Calendar, float Rate, UPARAM(DisplayName = "Military Time") bool TwentyFour, UPARAM()FChaos_Calendar_Defaults Chaos_Calendar, bool Custom_Calendar)
{
    // Should we start counting from initial input values or do we already have values
    if (Time_Altered == false)
    {
        // Determine if we are using a standard or non-standard calendar, if non-standard initialize user defined settings
        if (Custom_Calendar == true)
        {
            // We won't use AM/PM for non-standard calendars
            Military_Time = true;

            // Let the other functions know we are using a custom calendar
            Chaos = true;

            // Set the custom calendar cycle
            Initial_Chaos_Calendar = Chaos_Calendar;
            SecondsReset = Chaos_Calendar.Seconds;
            MinutesReset = Chaos_Calendar.Minutes;
            HoursReset = Chaos_Calendar.Hours;
            DaysReset = (Chaos_Calendar.Day + 1);
            MonthsReset = Chaos_Calendar.Chaos_Months.Num();
            Daylight = Chaos_Calendar.Daylight_Settings;

            // If no months named, populate array with error message & set start month to "0" index (prevents crash & identifies improper input)
            if (Chaos_Calendar.Chaos_Months.Num() < 1)
            {
                Chaos_Months = { "No Months Named In Custom Calendar" };
                Current_Month = 0;
            }
            else
            {
                Chaos_Months = Chaos_Calendar.Chaos_Months;
                Current_Month = Chaos_Calendar.Start_Month;
            }

            // Set initial start date and time for custom calendar
            Current_Seconds = Chaos_Calendar.Start_Seconds;
            Current_Minutes = Chaos_Calendar.Start_Minutes;
            Current_Hours = Chaos_Calendar.Start_Hours;
            Current_Day = Chaos_Calendar.Start_Day;
            Current_Year = Chaos_Calendar.Start_Year;
            YearToDateSeconds = GetYTD_Seconds();

            // Ensure we don't start a month on Day "0"
            if (Current_Day == 0)
            {
                Current_Day = 1;
            }
        }
        else
        {
            // Determine if we are using a 12 or 24 hour clock
            Military_Time = TwentyFour;

            // Set start time and date to user defined input on a standard calendar
            Current_Seconds = Standard_Calendar.Seconds;
            Current_Minutes = Standard_Calendar.Minutes;
            Current_Hours = Standard_Calendar.Hours;
            Current_Day = Standard_Calendar.Day;
            Current_Month = static_cast<int>(Standard_Calendar.Month);
            Current_Year = Standard_Calendar.Year;
            Daylight = Standard_Calendar.Daylight_Settings;
            YearToDateSeconds = GetYTD_Seconds();

            // Ensure we don't start a month on Day "0"
            if (Current_Day == 0)
            {
                Current_Day = 1;
            }
        }

        // Call function to ensure we don't start a month on a non-valid day
        GenerateDaysReset(Current_Month);

        // Set Sun starting position
        Sun_Degrees = Daylight.Sun_Start_Degrees;
    }

    // If we have altered time ensure the Sun is appropriately positioned to the point it was just before alteration
    if (Time_Altered == true && Chaos == true)
    {
        Sun_Degrees = Chaos_Calendar.Daylight_Settings.Sun_Start_Degrees;
        Military_Time = true;
    }
    else if (Time_Altered == true && Chaos == false)
    {
        Sun_Degrees = Standard_Calendar.Daylight_Settings.Sun_Start_Degrees;
        Military_Time = TwentyFour;
    }

    // Prevent division by "0" from improper user input
    if (Rate == 0)
    {
        Rate = 1;
    }

    // Calculate the number of degrees to move the Sun's position based on total daylight hours, total degrees and rate of time passage
    Range = (Daylight.Max_Degrees - Daylight.Min_Degrees);
    Delta_Degrees = (Range / Daylight.Hours_Of_Daylight / MinutesReset / SecondsReset);

    // Create a timer
    Parent_Actor->GetWorldTimerManager().SetTimer(Universal_Age, this, &UGlobalTimerComponent::AgeOfEarth, 1 / Rate, true);
}

// Function that actually tracks date and time
void UGlobalTimerComponent::AgeOfEarth()
{
    Current_Seconds++;
    YearToDateSeconds++;

    if (YearToDateSeconds >= TotalSecondsPerYear)
    {
        YearToDateSeconds = 0;
    }

    if (Current_Seconds == SecondsReset)
    {
        Current_Seconds = SecondsResetValue;
        Current_Minutes++;

        if (Current_Minutes == MinutesReset)
        {
            Current_Minutes = MinutesResetValue;
            Current_Hours++;
        }

        if (Current_Hours == HoursReset)
        {
            Current_Hours = HoursResetValue;
            Current_Day++;
        }
        if (Current_Day == DaysReset)
        {
            Current_Day = 1;
            Current_Month++;
            GenerateDaysReset(Current_Month);
        }
        if (Current_Month == MonthsReset)
        {
            Current_Month = 0;
            Current_Year++;
        }

    }

    // Function calls to create World Date and Clock string values and output the Sun position in degrees
    World_Clock = GenerateWorld_Clock(Current_Hours, Current_Minutes, Current_Seconds);
    World_Date = GenerateWorld_Date(Current_Month, Current_Day, Current_Year);
    Sun_Dial();
}

// Function to convert time values into "00:00:00" format (AM/PM if necessary)
FString UGlobalTimerComponent::GenerateWorld_Clock(int32 Hour, int32 Minute, int32 Second)
{
    FString HoursOutput;
    FString MinutesOutput;
    FString SecondsOutput;
    FString TimeOfDay = "AM";
    FString ClockOutput;
    int32 StandardHours = Hour;
    bool PM = false;

    if (Military_Time == false && StandardHours == 0)
    {
        StandardHours = 12;
        TimeOfDay = "AM";
    }
    else if (Military_Time == false && StandardHours > 12)
    {
        StandardHours -= 12;
        PM = true;
        TimeOfDay = "PM";
    }
    else if (Military_Time == false && StandardHours == 12)
    {
        PM = true;
        TimeOfDay = "PM";
    }

    HoursOutput = FString::FromInt(StandardHours);
    if (HoursOutput.Len() < 2)
    {
        HoursOutput = "0" + HoursOutput;
    }
    MinutesOutput = FString::FromInt(Minute);
    if (MinutesOutput.Len() < 2)
    {
        MinutesOutput = "0" + MinutesOutput;
    }
    SecondsOutput = FString::FromInt(Second);
    if (SecondsOutput.Len() < 2)
    {
        SecondsOutput = "0" + SecondsOutput;
    }

    // If not on military time output string will include "AM/PM" designation
    if (Military_Time == false)
    {
        ClockOutput = (HoursOutput + " : " + MinutesOutput + " : " + SecondsOutput + " " + TimeOfDay);
    }
    else
    {
        ClockOutput = (HoursOutput + " : " + MinutesOutput + " : " + SecondsOutput);
    }

    return ClockOutput;
}

// Function to convert month, day, year values into "MM/DD/YYYY" format
FString UGlobalTimerComponent::GenerateWorld_Date(int32 Month, int32 Day, int32 Year)
{
    FString MonthOutput;
    FString DayOutput;
    FString YearOutput;
    FString DateOutput;

    MonthOutput = MonthAsString(Month);

    DayOutput = FString::FromInt(Day);
    if (DayOutput.Len() < 2)
    {
        DayOutput = "0" + DayOutput;
    }

    YearOutput = FString::FromInt(Year);
    DateOutput = (MonthOutput + " " + DayOutput + ", " + YearOutput);
    return DateOutput;
}

// Function to "Pause" World Time
void UGlobalTimerComponent::Pause()
{
    Parent_Actor->GetWorldTimerManager().PauseTimer(Universal_Age);
}

// Function to "Unpause" World Time
void UGlobalTimerComponent::Unpause()
{
    Parent_Actor->GetWorldTimerManager().UnPauseTimer(Universal_Age);
}

// Function to "Clear" World Time
void UGlobalTimerComponent::ClearTimer()
{
    Parent_Actor->GetWorldTimerManager().ClearTimer(Universal_Age);
}

// Function to "Reset" World Time to 00:00:00
void UGlobalTimerComponent::ResetClock()
{
    Current_Seconds = 0;
    Current_Minutes = 0;
    Current_Hours = 0;
    YearToDateSeconds = GetYTD_Seconds();
}

// Function to "Save" current World Time and Date
void UGlobalTimerComponent::SaveStandardDate(FStandard_Calendar& End_Date, bool Pause_Timer, bool Clear_Timer)
{
    // Should we pause our timer during a save cycle or continue counting
    if (Pause_Timer == true)
    {
        Pause();
    }

    End_Date.Month = static_cast<ECalendarMonths>(Current_Month);
    End_Date.Day = Current_Day;
    End_Date.Year = Current_Year;
    End_Date.Hours = Current_Hours;
    End_Date.Minutes = Current_Minutes;
    End_Date.Seconds = Current_Seconds;
    End_Date.Daylight_Settings = Daylight;
    End_Date.YearToDateSeconds = YearToDateSeconds;

    // Overwrite the default Sun position during a save cycle
    End_Date.Daylight_Settings.Sun_Start_Degrees = Sun_Degrees;

    // Should we stop our timer after saving or continue counting
    if (Clear_Timer == true)
    {
        ClearTimer();
    }
}

// Function to "Save" current World Time and Date for custom calendar types
void UGlobalTimerComponent::SaveChaosDate(FChaos_Calendar_Defaults& End_Date, bool Pause_Timer, bool Clear_Timer)
{
    // Should we pause our timer during a save cycle or continue counting
    if (Pause_Timer == true)
    {
        Pause();
    }

    // When saving a custom calendar cycle, save the input defaults back out
    End_Date.Seconds = Initial_Chaos_Calendar.Seconds;
    End_Date.Minutes = Initial_Chaos_Calendar.Minutes;
    End_Date.Hours = Initial_Chaos_Calendar.Hours;
    End_Date.Day = Initial_Chaos_Calendar.Day;
    End_Date.Chaos_Months = Initial_Chaos_Calendar.Chaos_Months;
    End_Date.Daylight_Settings = Daylight;
    End_Date.YearToDateSeconds = YearToDateSeconds;

    /* The new start time and date will be set to the last registered values, this way the struct can be directly fed back into the "Earth Genesis"
       function with no additional modifications and the cycle will continue at the exact spot of the last save with the same default cycle values*/
    End_Date.Start_Seconds = Current_Seconds;
    End_Date.Start_Minutes = Current_Minutes;
    End_Date.Start_Hours = Current_Hours;
    End_Date.Start_Day = Current_Day;
    End_Date.Start_Month = Current_Month;
    End_Date.Start_Year = Current_Year;
    End_Date.YearToDateSeconds = YearToDateSeconds;

    // Overwrite the default Sun position during a save cycle
    End_Date.Daylight_Settings.Sun_Start_Degrees = Sun_Degrees;

    // Should we stop our timer after saving or continue counting
    if (Clear_Timer == true)
    {
        ClearTimer();
    }
}

// Function to change rate of World Time passage
void UGlobalTimerComponent::AlterTime(float New_Rate, UPARAM(DisplayName = "Military Time") bool NewClock)
{
    // Declare both possible input types for a new Earth Genesis call **only one will be valid and used**
    FStandard_Calendar New_Start_Date;
    FChaos_Calendar_Defaults New_Chaos_Date;

    if (Chaos == true)
    {
        New_Chaos_Date.Seconds = Current_Seconds;
        New_Chaos_Date.Minutes = Current_Minutes;
        New_Chaos_Date.Hours = Current_Hours;
        New_Chaos_Date.Day = Current_Day;
        New_Chaos_Date.Start_Month = Current_Month;
        New_Chaos_Date.Start_Year = Current_Year;
        New_Chaos_Date.Daylight_Settings = Daylight;
        New_Chaos_Date.Daylight_Settings.Sun_Start_Degrees = Sun_Degrees;
        YearToDateSeconds = GetYTD_Seconds();
    }
    else
    {
        New_Start_Date.Seconds = Current_Seconds;
        New_Start_Date.Minutes = Current_Minutes;
        New_Start_Date.Hours = Current_Hours;
        New_Start_Date.Day = Current_Day;
        New_Start_Date.Month = static_cast<ECalendarMonths>(Current_Month);
        New_Start_Date.Year = Current_Year;
        New_Start_Date.Daylight_Settings = Daylight;
        New_Start_Date.Daylight_Settings.Sun_Start_Degrees = Sun_Degrees;
        YearToDateSeconds = GetYTD_Seconds();
    }

    // Let other functions know we have altered time
    Time_Altered = true;

    // Clear our current timer
    Parent_Actor->GetWorldTimerManager().ClearTimer(Universal_Age);

    // Reset our Earth Timer to the new rate and/or clock output type
    Earth_Genesis(New_Start_Date, New_Rate, NewClock, New_Chaos_Date, Chaos);
}

// Function to convert Month Enum to string for output to blueprints
FString UGlobalTimerComponent::MonthAsString(int32 ConvertMonth)
{
    FString EnumMonth;

    if (Chaos == true)
    {
        EnumMonth = Chaos_Months[ConvertMonth];
    }
    else
    {
        switch (ConvertMonth)
        {
        case 0:
            EnumMonth = "January";
            break;
        case 1:
            EnumMonth = "February";
            break;
        case 2:
            EnumMonth = "March";
            break;
        case 3:
            EnumMonth = "April";
            break;
        case 4:
            EnumMonth = "May";
            break;
        case 5:
            EnumMonth = "June";
            break;
        case 6:
            EnumMonth = "July";
            break;
        case 7:
            EnumMonth = "August";
            break;
        case 8:
            EnumMonth = "September";
            break;
        case 9:
            EnumMonth = "October";
            break;
        case 10:
            EnumMonth = "November";
            break;
        case 11:
            EnumMonth = "December";
            break;
        default:
            EnumMonth = "Bad Month";
            break;
        }
    }
    return EnumMonth;
}

// Function to determine when to "reset" day count (adjusts for Leap year)
void UGlobalTimerComponent::GenerateDaysReset(int32 ActiveMonth)
{
    /* If using a non-standard calendar skip determining how many days/month, all months will have the same # of days
        Only modify the Total Seconds Per Year Variable */
    if (Chaos == true)
    {
        TotalSecondsPerYear = (SecondsReset * MinutesReset * HoursReset * (DaysReset - 1) * MonthsReset);
    }
    else
    {
        switch (ActiveMonth)
        {
        case 0: case 2: case 4: case 6: case 7: case 9: case 11:
            DaysReset = 32;
            break;
        case 1:
            if (Current_Year % 4 == 0)
            {
                DaysReset = 30;
            }
            else
            {
                DaysReset = 29;
            }
            break;
        case 3: case 5: case 8: case 10:
            DaysReset = 31;
            break;
        default:
            DaysReset = 31;
            break;
        }

        if (Current_Year % 4 == 0)
        {
            TotalSecondsPerYear = 31622400;
        }
        else
        {
            TotalSecondsPerYear = 31536000;
        }
    }
    // Now that we know how many days are in the current month ensure we are on a valid day
    if (Current_Day >= DaysReset)
    {
        Current_Day = (DaysReset - 1);
    }
}

// Function to determine the Sun's position in degrees
void UGlobalTimerComponent::Sun_Dial()
{
    /* Ensure we are only moving the Sun through the specified "Min" and "Max" degrees and make sure we only
       change the Sun's position during the specified "Daylight" hours, otherwise Sun position will be at "0" degrees*/
    if (Current_Hours < Daylight.Sunrise || Sun_Degrees >= Daylight.Max_Degrees || (Current_Hours - Daylight.Sunrise) >= Daylight.Hours_Of_Daylight)
    {
        Sun_Degrees = 0;
    }
    else
    {
        Sun_Degrees += Delta_Degrees;
    }
}

// Function that calculates the total seconds passed to date of the current year
int32 UGlobalTimerComponent::GetYTD_Seconds()
{
    int32 SummedDays = 0;
    bool LeapYear;

    if (Current_Year % 4 == 0)
    {
        LeapYear = true;
    }
    else
    {
        LeapYear = false;
    }

    if (Chaos == false)
    {
        switch (Current_Month)
        {
        case 0:
            SummedDays = (Current_Day - 1);
            break;
        case 1:
            SummedDays = (Current_Month * 30) + Current_Day;
            break;
        case 2:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day - 1);
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day - 2);
            }
            break;
        case 3:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day);
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day - 1);
            }
            break;
        case 4:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + Current_Day;
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day - 1);
            }
            break;
        case 5:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 1);
            }
            else
            {
                SummedDays = (Current_Month * 30) + Current_Day;
            }
            break;
        case 6:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 1);
            }
            else
            {
                SummedDays = (Current_Month * 30) + Current_Day;
            }
            break;
        case 7:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 2);
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 1);
            }
            break;
        case 8:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 3);
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 2);
            }
            break;
        case 9:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 3);
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 2);
            }
            break;
        case 10:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 4);
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 3);
            }
            break;
        case 11:
            if (LeapYear == true)
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 4);
            }
            else
            {
                SummedDays = (Current_Month * 30) + (Current_Day + 3);
            }
            break;
        }
    }
    else
    {
        SummedDays = (Current_Month * (DaysReset - 1)) + (Current_Day - 1);
    }

    int32 YTD_Seconds = ((SummedDays * HoursReset * MinutesReset * SecondsReset) + (Current_Hours * MinutesReset * SecondsReset) + (Current_Minutes * SecondsReset) + Current_Seconds);
    return YTD_Seconds;
}

// Function to generate a time stamp used for calculating time elapsed between two given time stamps
FTimeStamp UGlobalTimerComponent::Generate_Time_Stamp()
{
    FTimeStamp Time_Stamp;

    Time_Stamp.Time_Stamp.Append(World_Date + " " + World_Clock);
    Time_Stamp.YTD_Seconds = GetYTD_Seconds();
    Time_Stamp.Seconds = Current_Seconds;
    Time_Stamp.Minutes = Current_Minutes;
    Time_Stamp.Hours = Current_Hours;
    Time_Stamp.Day = Current_Day;
    Time_Stamp.Month = Current_Month;
    Time_Stamp.Year = Current_Year;

    return Time_Stamp;
}

// Function that takes 2 time stamps and calculates the elapsed time between them
FTimeStamp UGlobalTimerComponent::Delta_Time(FTimeStamp Start_Time, FTimeStamp End_Time)
{
    int32 TempInt1 = (End_Time.Year - Start_Time.Year);
    int32 TempInt2 = 0;
    FTimeStamp FinalTimeStamp;

    // If we only have a difference in the year-to-date totals we can ignore the years category
    if ((TempInt1 >= 0 && (End_Time.YTD_Seconds - Start_Time.YTD_Seconds) >= 0))
    {
        TempInt2 = (End_Time.YTD_Seconds - Start_Time.YTD_Seconds);
        FinalTimeStamp.Year = TempInt1;
        FinalTimeStamp.Month = (TempInt2 / (SecondsReset * MinutesReset * HoursReset * (DaysReset - 1)));
        TempInt2 = (TempInt2 % (SecondsReset * MinutesReset * HoursReset * (DaysReset - 1)));
        FinalTimeStamp.Day = (TempInt2 / (SecondsReset * MinutesReset * HoursReset));
        TempInt2 = (TempInt2 % (SecondsReset * MinutesReset * HoursReset));
        FinalTimeStamp.Hours = (TempInt2 / (SecondsReset * MinutesReset));
        TempInt2 = (TempInt2 % (SecondsReset * MinutesReset));
        FinalTimeStamp.Minutes = (TempInt2 / SecondsReset);
        FinalTimeStamp.Seconds = (TempInt2 % SecondsReset);
    }
    // If there is a difference in the years we will take this into account when determining the total elapsed time
    else if (TempInt1 > 0 && (End_Time.YTD_Seconds - Start_Time.YTD_Seconds) <= 0)
    {
        TempInt2 = (TotalSecondsPerYear - Start_Time.YTD_Seconds) + (End_Time.YTD_Seconds);
        FinalTimeStamp.Year = (TempInt1 - 1);
        FinalTimeStamp.Month = (TempInt2 / (SecondsReset * MinutesReset * HoursReset * (DaysReset - 1)));
        TempInt2 = (TempInt2 % (SecondsReset * MinutesReset * HoursReset * (DaysReset - 1)));
        FinalTimeStamp.Day = (TempInt2 / (SecondsReset * MinutesReset * HoursReset));
        TempInt2 = (TempInt2 % (SecondsReset * MinutesReset * HoursReset));
        FinalTimeStamp.Hours = (TempInt2 / (SecondsReset * MinutesReset));
        TempInt2 = (TempInt2 % (SecondsReset * MinutesReset));
        FinalTimeStamp.Minutes = (TempInt2 / SecondsReset);
        FinalTimeStamp.Seconds = (TempInt2 % SecondsReset);
    }

    // Display the elapsed time as a string in "year:month:day + 00:00:00" format
    if (FinalTimeStamp.Year > 0)
    {
        FinalTimeStamp.Time_Stamp.Append(FString::FromInt(FinalTimeStamp.Year) + " years : ");
    }
    if (FinalTimeStamp.Month > 0)
    {
        FinalTimeStamp.Time_Stamp.Append(FString::FromInt(FinalTimeStamp.Month) + " months : ");
    }
    if (FinalTimeStamp.Day > 0)
    {
        FinalTimeStamp.Time_Stamp.Append(FString::FromInt(FinalTimeStamp.Day) + " days and ");
    }

    FinalTimeStamp.Time_Stamp.Append(GenerateWorld_Clock(FinalTimeStamp.Hours, FinalTimeStamp.Minutes, FinalTimeStamp.Seconds));

    return FinalTimeStamp;
}

// Function that auto-calculates the elapsed time between the input reference time stamp and the current world time
void UGlobalTimerComponent::Elapsed_Time(FTimeStamp Time, FTimeStamp& Elapsed)
{
    FTimeStamp PresentTime = Generate_Time_Stamp();
    Elapsed = Delta_Time(Time, PresentTime);
}