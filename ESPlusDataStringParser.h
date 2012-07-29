//
//  ESPlusDataStringParser.h
//  ES+ Client
//
//  Created by Michael Diehl on 6/1/12.
//  Copyright (c) 2012 Michael Diehl. All rights reserved.
//


@interface ESPlusDataStringParser : NSObject
{
    @private
    double	m_dischargePressure,
            m_reservoirPressure,
            m_dischargeTemperature,
            m_reservoirTemperature,
            m_runHours,
            m_loadHours,
            m_currentTempHM1,
            m_currentTempHM2,
            m_currentTempHM3,
            m_currentTempHM4,
            m_currentTempHM5,
            m_currentTempHM6,
            m_storedTempHM1,
            m_storedTempHM2,
            m_storedTempHM3,
            m_storedTempHM4,
            m_storedTempHM5,
            m_storedTempHM6,
            m_lastOilChange,
            m_lastOilFilterChange,
            m_oilAge;

    uint m_mainMotorAmps,
        m_sequenceNumber;

    NSMutableArray *m_shutdowns,
                   *m_advisories;
	
    NSString *m_operatingMode,
             *m_operatingState;
}

@property (readonly, nonatomic) double dischargePressure;
@property (readonly, nonatomic) double reservoirPressure;
@property (readonly, nonatomic) double dischargeTemperature;
@property (readonly, nonatomic) double reservoirTemperature;
@property (readonly, nonatomic) double runHours;
@property (readonly, nonatomic) double loadHours;
@property (readonly, nonatomic) double currentTempHM1;
@property (readonly, nonatomic) double currentTempHM2;
@property (readonly, nonatomic) double currentTempHM3;
@property (readonly, nonatomic) double currentTempHM4;
@property (readonly, nonatomic) double currentTempHM5;
@property (readonly, nonatomic) double currentTempHM6;
@property (readonly, nonatomic) double storedTempHM1;
@property (readonly, nonatomic) double storedTempHM2;
@property (readonly, nonatomic) double storedTempHM3;
@property (readonly, nonatomic) double storedTempHM4;
@property (readonly, nonatomic) double storedTempHM5;
@property (readonly, nonatomic) double storedTempHM6;
@property (readonly, nonatomic) double lastOilChange;
@property (readonly, nonatomic) double lastOilFilterChange;
@property (readonly, nonatomic) double oilAge;

@property (readonly, nonatomic) uint mainMotorAmps;
@property (readonly, nonatomic) uint sequenceNumber;

@property (readonly, retain) NSMutableArray *shutdowns;
@property (readonly, retain) NSMutableArray *advisories;
@property (readonly, retain) NSString *operatingMode;
@property (readonly, retain) NSString *operatingState;

- (ESPlusDataStringParser *)init;

- (BOOL)parseAdvisoryString:(NSString *)advisoryString;
- (BOOL)parseOperatingDataString:(NSString *)operatingDataString;
- (BOOL)parseOperatingModeString:(NSString *)operatingModeString;
- (BOOL)parseServiceDataString:(NSString *)serviceDataString;

- (void)setDefaults;

- (NSArray *)shutdownDataFromString:(NSString *)shutdownString;

- (NSString *)modeStringFromCode:(int)code;
- (NSString *)stateStringFromCode:(int)code;
- (NSString *)advisoryStringFromCode:(int)code;
- (NSString *)shutdownStringFromCode:(int)code;

@end
