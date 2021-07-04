
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/KismetStringLibrary.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "GlobalTimerComponent.generated.h"

// Enum of Months
UENUM(BlueprintType)
enum class ECalendarMonths : uint8
{
    January, February, March, April, May, June, July,
    August, September, October, November, December
};

// Input struct containing necessary values to determine "Sun position" during day/night cycle
USTRUCT(BlueprintType)
struct FDay_Night_Cycle
{
    GENERATED_USTRUCT_BODY()

public:

    // The sun will start at this value in the sky on an invisible 360 path (0 degrees would be flat on the ground, 90 degrees directly above)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Position", meta = (ClampMin = "0", UIMin = "0"))
        float Min_Degrees;
    // The total amount of degrees the sun will travel through on an imaginary 360 degree path (180 would be a traditional "path" that started at 0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Position", meta = (ClampMax = "359", UIMax = "359"))
        float Max_Degrees;
    // Determines the time of day the sun will begin to travel along its path
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Position", meta = (ClampMin = "0", UIMin = "0"))
        int32 Sunrise;
    // Determines how long it will take the sun to reach the final position of its path before being reset
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Position", meta = (ClampMin = "0", UIMin = "0"))
        int32 Hours_Of_Daylight;

    // Hidden value we will pass along with this struct to keep track of the sun's position during save/load cycles
    float Sun_Start_Degrees;
};

// Input struct for holding time stamp data 
USTRUCT(BlueprintType)
struct FTimeStamp
{
    GENERATED_USTRUCT_BODY()

public:

    // The current time stamp in MM/DD/YYYY + 00:00:00 format
    UPROPERTY(BlueprintReadOnly, Category = "Time Stamp")
        FString Time_Stamp;
    // Current second at the time this function was called
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Stamp", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int32 Seconds;
    // Current minute at the time this function was called
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Stamp", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int32 Minutes;
    // Current hour at the time this function was called
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Stamp", meta = (ClampMin = "0", ClampMax = "23", UIMin = "0", UIMax = "23"))
        int32 Hours;
    // Current day at the time this function was called
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Stamp", meta = (ClampMin = "1", ClampMax = "31", UIMin = "1", UIMax = "31"))
        int32 Day;
    // Current month at the time this function was called
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Stamp", meta = (ClampMin = "1", ClampMax = "12", UIMin = "1", UIMax = "12"))
        int32 Month;
    // Current year at the time this function was called
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Stamp", meta = (ClampMin = "0", UIMin = "0"))
        int32 Year;

    /* Hidden value we will pass along with this struct to keep track of the total seconds passed in this year
        (may not be necessary as this can easily be calculated) */
    int32 YTD_Seconds;
};

// Input struct containing all necessary values to determine where to begin counting time
USTRUCT(BlueprintType)
struct FStandard_Calendar
{
    GENERATED_USTRUCT_BODY()

public:

    // Where to start counting seconds on a standard calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Standard", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int32 Seconds;
    // Where to start counting minutes on a standard calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Standard", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int32 Minutes;
    // Where to start counting hours on a standard calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Standard", meta = (ClampMin = "0", ClampMax = "23", UIMin = "0", UIMax = "23"))
        int32 Hours;
    // Where to start counting days on a standard calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Standard", meta = (ClampMin = "1", ClampMax = "31", UIMin = "1", UIMax = "31"))
        int32 Day;
    // Where to start counting months on a standard calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Standard", meta = (ClampMin = "1", ClampMax = "12", UIMin = "1", UIMax = "12"))
        ECalendarMonths Month;
    // Where to start counting years on a standard calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Standard", meta = (ClampMin = "0", UIMin = "0"))
        int32 Year;
    // Detemines the day/night cycle for a standard calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Standard")
        FDay_Night_Cycle Daylight_Settings;

    /* Hidden value we will pass along with this struct to keep track of the total seconds passed in this year
        (may not be necessary as this can easily be calculated) */
    int32 YearToDateSeconds;
};

// Struct to define custom calendar types with non-standard day/month/year & second/minute/hour cycles
USTRUCT(BlueprintType)
struct FChaos_Calendar_Defaults
{
    GENERATED_USTRUCT_BODY()

public:

    // Detemines the seconds per minute on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Seconds Per Minute", ClampMin = "0", UIMin = "0"))
        int32 Seconds;
    // Detemines the minutes per hour on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Minutes Per Hour", ClampMin = "0", UIMin = "0"))
        int32 Minutes;
    // Detemines the hours per day on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Hours Per Day", ClampMin = "0", UIMin = "0"))
        int32 Hours;
    // Detemines the days per month on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Days Per Month", ClampMin = "1", UIMin = "1"))
        int32 Day;
    // The names of the months for a custom calendar
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Month Names"))
        TArray<FString> Chaos_Months;
    // Detemines the day/night cycle for a custom calendar
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos")
        FDay_Night_Cycle Daylight_Settings;

    // Where to start counting seconds on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Start Second", ClampMin = "0", UIMin = "0"))
        int32 Start_Seconds;
    // Where to start counting minutes on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Start Minute", ClampMin = "0", UIMin = "0"))
        int32 Start_Minutes;
    // Where to start counting hours on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Start Hour", ClampMin = "0", UIMin = "0"))
        int32 Start_Hours;
    // Where to start counting days on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Start Day", ClampMin = "1", UIMin = "1"))
        int32 Start_Day;
    // Where to start counting months on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Start Month Index", ClampMin = "0", UIMin = "0"))
        int32 Start_Month;
    // Where to start counting years on a custom calendar cycle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar|Chaos", meta = (DisplayName = "Start Year", ClampMin = "0", UIMin = "0"))
        int32 Start_Year;

    // Hidden data on the total year to date seconds we will pass along with this struct
    int32 YearToDateSeconds;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEMOVER1_HUAWEI_API UGlobalTimerComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    // Sets default values for this component's properties
    UGlobalTimerComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Variable that returns the current world clock output as 00:00:00
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        FString World_Clock;
    // Variable that returns the current world date
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        FString World_Date;
    // The current world seconds
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        int32 Current_Seconds;
    // The current world minutes
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        int32 Current_Minutes;
    // The current world hours
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        int32 Current_Hours;
    // The current world days
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        int32 Current_Day;
    // The current world months
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        int32 Current_Month;
    // The current world years
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        int32 Current_Year;
    // The position of the sun in degrees
    UPROPERTY(BlueprintReadOnly, Category = "Earth Time")
        float Sun_Degrees;

    // Function to start a world date and clock
    UFUNCTION(BlueprintCallable)
        void Earth_Genesis(UPARAM()FStandard_Calendar Standard_Calendar, float Rate, UPARAM(DisplayName = "Military Time") bool TwentyFour, UPARAM()FChaos_Calendar_Defaults Chaos_Calendar, bool Custom_Calendar);
    // Function to change the rate of time passage
    UFUNCTION(BlueprintCallable)
        void AlterTime(float New_Rate, UPARAM(DisplayName = "Military Time") bool NewClock);
    // Function to pause the world clock
    UFUNCTION(BlueprintCallable)
        void Pause();
    // Function to unpause the world clock
    UFUNCTION(BlueprintCallable)
        void Unpause();
    // Function to clear the timer associated with the world clock
    UFUNCTION(BlueprintCallable)
        void ClearTimer();
    // Function to reset the world clock to 00:00:00 (does not affect world date)
    UFUNCTION(BlueprintCallable)
        void ResetClock();
    // Function to save a world date and time running a standard calendar cycle
    UFUNCTION(BlueprintCallable)
        void SaveStandardDate(FStandard_Calendar& End_Date, bool Pause_Timer, bool Clear_Timer);
    // Function to save a world date and time running a custom calendar cycle
    UFUNCTION(BlueprintCallable)
        void SaveChaosDate(FChaos_Calendar_Defaults& End_Date, bool Pause_Timer, bool Clear_Timer);
    /* Function to generate a time stamp (used to calculate elapsed times, output not user friendly)
        for user friendly world date/time grab the "World Date" and "World Time" string variables*/
    UFUNCTION(BlueprintCallable)
        FTimeStamp Generate_Time_Stamp();
    // Function to calculate the time elapsed from the given input value (gathered from a call to "Generate Time Stamp") to the current moment
    UFUNCTION(BlueprintCallable)
        void Elapsed_Time(FTimeStamp Time, FTimeStamp& Elapsed);
    // Function to calculate the elapsed time between 2 user input dates (both must be acquired using the "Generate Time Stamp" function)
    UFUNCTION(BlueprintCallable)
        FTimeStamp Delta_Time(FTimeStamp Start_Time, FTimeStamp End_Time);


    // Internal Functions 
    void AgeOfEarth();
    FString GenerateWorld_Clock(int32 Hour, int32 Minute, int32 Second);
    FString GenerateWorld_Date(int32 Month, int32 Day, int32 Year);
    void GenerateDaysReset(int32 ActiveMonth);
    void Sun_Dial();
    FString MonthAsString(int32 ConvertMonth);
    int32 GetYTD_Seconds();

    // Global variables necessary for Earth Time functionality
    bool Time_Altered;
    bool Military_Time;
    bool Chaos;
    float Delta_Degrees;
    float Range;
    int32 SecondsReset;
    int32 SecondsResetValue;
    int32 MinutesReset;
    int32 MinutesResetValue;
    int32 HoursReset;
    int32 HoursResetValue;
    int32 DaysReset;
    int32 MonthsReset;
    int32 YearToDateSeconds;
    int32 TotalSecondsPerYear;
    ECalendarMonths Calendar_Months;
    FChaos_Calendar_Defaults Initial_Chaos_Calendar;
    FDay_Night_Cycle Daylight;
    TArray<FString> Chaos_Months;
    FTimerHandle Universal_Age;
    AActor* Parent_Actor;

protected:

    // Called when the game starts
    virtual void BeginPlay() override;
};