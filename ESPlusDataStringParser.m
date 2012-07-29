//
//  ESPlusDataStringParser.m
//  ES+ Client
//
//  Created by Michael Diehl on 6/1/12.
//  Copyright (c) 2012 Michael Diehl. All rights reserved.
//

#import "ESPlusDataStringParser.h"

#define OP_MODE_OLD 1
#define OP_MODE_NEW 4
#define OP_DATA_LENGTH 19
#define SERVICE_DATA_LENGTH 214
#define ADVISORY_DATA_LENGTH 4

@implementation ESPlusDataStringParser

@synthesize dischargePressure = m_dischargePressure;
@synthesize reservoirPressure = m_reservoirPressure;
@synthesize dischargeTemperature = m_dischargeTemperature;
@synthesize reservoirTemperature = m_reservoirTemperature;
@synthesize runHours = m_runHours;
@synthesize loadHours = m_loadHours;
@synthesize currentTempHM1 = m_currentTempHM1;
@synthesize currentTempHM2 = m_currentTempHM2;
@synthesize currentTempHM3 = m_currentTempHM3;
@synthesize currentTempHM4 = m_currentTempHM4;
@synthesize currentTempHM5 = m_currentTempHM5;
@synthesize currentTempHM6 = m_currentTempHM6;
@synthesize storedTempHM1 = m_storedTempHM1;
@synthesize storedTempHM2 = m_storedTempHM2;
@synthesize storedTempHM3 = m_storedTempHM3;
@synthesize storedTempHM4 = m_storedTempHM4;
@synthesize storedTempHM5 = m_storedTempHM5;
@synthesize storedTempHM6 = m_storedTempHM6;
@synthesize lastOilChange = m_lastOilChange;
@synthesize lastOilFilterChange = m_lastOilFilterChange;
@synthesize oilAge = m_oilAge;

@synthesize mainMotorAmps = m_mainMotorAmps;
@synthesize sequenceNumber = m_sequenceNumber;

@synthesize advisories = m_advisories;
@synthesize shutdowns = m_shutdowns;

@synthesize operatingMode = m_operatingMode;
@synthesize operatingState = m_operatingState;

- (ESPlusDataStringParser *)init
{
    self = [super init];
    [self setDefaults];
    return self;
}

- (void)setDefaults
{
    m_dischargePressure = 0.0;
    m_reservoirPressure = 0.0;
    m_dischargeTemperature = 0.0;
    m_reservoirTemperature = 0.0;
    m_runHours = 0.0;
    m_loadHours = 0.0;
    m_currentTempHM1 = 0.0;
    m_currentTempHM2 = 0.0;
    m_currentTempHM3 = 0.0;
    m_currentTempHM4 = 0.0;
    m_currentTempHM5 = 0.0;
    m_currentTempHM6 = 0.0;
    m_storedTempHM1 = 0.0;
    m_storedTempHM2 = 0.0;
    m_storedTempHM3 = 0.0;
    m_storedTempHM4 = 0.0;
    m_storedTempHM5 = 0.0;
    m_storedTempHM6 = 0.0;
    m_lastOilChange = 0.0;
    m_lastOilFilterChange = 0.0;
    m_oilAge = 0.0;

    m_mainMotorAmps = 0;
    m_sequenceNumber = 0;

    m_advisories = [[NSMutableArray alloc]init];
    m_shutdowns = [[NSMutableArray alloc]init];

    m_operatingMode = @"Not Available";
    m_operatingState = @"Not Available";
}

- (BOOL)parseAdvisoryString:(NSString *)advisoryString
{
    BOOL success = FALSE;

    if ([advisoryString length] == ADVISORY_DATA_LENGTH) {
        uint dec;
        NSScanner *scan = [NSScanner scannerWithString:advisoryString];
        if ([scan scanHexInt:&dec]){ //Convert 4 digit hex to int
            NSMutableArray *array = [[NSMutableArray alloc]init];
            int count = 0;
            for(NSInteger numberCopy = dec; numberCopy > 0; numberCopy >>= 1)
            { // Inspect each bit to see if it is high. 
                if(numberCopy & 1){
                    [array addObject:[self advisoryStringFromCode:count]];
                }
                count++;
            }
            m_advisories = array;
            success = TRUE;
        }
    }
    return success;
}

- (BOOL)parseOperatingDataString:(NSString *)operatingDataString
{
    BOOL success = FALSE;

    if ([operatingDataString length] == OP_DATA_LENGTH) {
        NSString *disPressString = [operatingDataString substringWithRange:NSMakeRange(0, 4)];
        NSString *resPressString = [operatingDataString substringWithRange:NSMakeRange(4, 4)];
        NSString *disTempString = [operatingDataString substringWithRange:NSMakeRange(8, 4)];
        NSString *resTempString = [operatingDataString substringWithRange:NSMakeRange(12, 4)];
        NSString *ampsString = [operatingDataString substringWithRange:NSMakeRange(16, 3)];

        uint disPDec;
        NSScanner *dPScan = [NSScanner scannerWithString:disPressString];
        if ([dPScan scanHexInt:&disPDec]){
            m_dischargePressure = disPDec / 16.00;
        }

        uint resPDec;
        NSScanner *rPScan = [NSScanner scannerWithString:resPressString];
        if ([rPScan scanHexInt:&resPDec]){
            m_reservoirPressure = resPDec / 16.00;
        }

        uint disTDec;
        NSScanner *disTScan = [NSScanner scannerWithString:disTempString];
        if ([disTScan scanHexInt:&disTDec]){
            m_dischargeTemperature = disTDec / 16.00;
        }
		
        uint resTDec;
        NSScanner *resTScan = [NSScanner scannerWithString:resTempString];
        if ([resTScan scanHexInt:&resTDec]){
            m_reservoirTemperature = resTDec / 16.00;
        }

        uint ampsDec;
        NSScanner *ampsScan = [NSScanner scannerWithString:ampsString];
        if ([ampsScan scanHexInt:&ampsDec]){
            m_mainMotorAmps = ampsDec;
        }

        // Handle negative values
        if(m_dischargePressure > 2047.0){
            m_dischargePressure -= 4096.0;
        }
		
        if(m_reservoirPressure > 2047.0){
            m_reservoirPressure -= 4096.0;
        }

        if(m_dischargeTemperature > 2047.0){
            m_dischargeTemperature -= 4096.0;
        }

        if(m_reservoirTemperature > 2047.0){
            m_reservoirTemperature -= 4096.0;
        }
        success = TRUE;
    }
    return success;
}

- (BOOL)parseOperatingModeString:(NSString *)operatingModeString
{
    BOOL success = FALSE;

    uint modeDataLength = [operatingModeString length];

    if (modeDataLength == OP_MODE_OLD) {
        /* Versions prior to 2.10 do not provide operating state
                or sequence number so we define them as not available. */
        uint dec;
        NSScanner *scan = [NSScanner scannerWithString:operatingModeString];
        if ([scan scanHexInt:&dec]){
            m_operatingMode = [self modeStringFromCode:dec];
            m_operatingState = @"Not Available";
            m_sequenceNumber = 0;
            success = TRUE;
        }
    }
    else if (modeDataLength == OP_MODE_NEW) {
        NSString *opMode = [operatingModeString substringToIndex:1];
        NSString *sequenceStr = [operatingModeString substringWithRange:NSMakeRange(1, 1)];
        NSString *opState = [operatingModeString substringFromIndex:2];

        uint omDec;
        NSScanner *omScan = [NSScanner scannerWithString:opMode];
        if ([omScan scanHexInt:&omDec]){
            m_operatingMode = [self modeStringFromCode:omDec];
        }

        uint seqDec;
        NSScanner *seqScan = [NSScanner scannerWithString:sequenceStr];
        if ([seqScan scanHexInt:&seqDec]){
            m_sequenceNumber = seqDec;
        }

        uint osDec;
        NSScanner *osScan = [NSScanner scannerWithString:opState];
        if ([osScan scanHexInt:&osDec]) {
            m_operatingState = [self stateStringFromCode:osDec];
        }
        success = TRUE;
    }
    else {
        m_operatingMode = @"Not Available";
        m_operatingState = @"Not Available";
        m_sequenceNumber = 0;
    }
    return success;
}

- (BOOL)parseServiceDataString:(NSString *)serviceDataString
{
    BOOL success = FALSE;

    if ([serviceDataString length] == SERVICE_DATA_LENGTH) {
        // Remove pesky '$' delimiters.
        serviceDataString = [serviceDataString stringByReplacingOccurrencesOfString:@"$" withString:@"#"];

        NSArray *serviceArray = [serviceDataString componentsSeparatedByString:@"#"];

        [self parseAdvisoryString:[serviceArray objectAtIndex:0]];

        NSMutableArray *sdArray = [[NSMutableArray alloc] init];
        for (int i = 1; i < 11; i++) {
            NSString *str = [serviceArray objectAtIndex:i];
            // Only add shutdowns to the array if they contain data
            if(![str hasPrefix:@"00"]){
                [sdArray addObject:[self shutdownDataFromString:str]];
            }
        }
        m_shutdowns = sdArray;

        // Operating hourmeters
        m_runHours = [[serviceArray objectAtIndex:11] doubleValue] / 10.0;
        m_loadHours = [[serviceArray objectAtIndex:12] doubleValue] / 10.0;
        // Current temp hourmeters H1-H6
        m_currentTempHM1 = [[serviceArray objectAtIndex:13] doubleValue] / 10.0;
        m_currentTempHM2 = [[serviceArray objectAtIndex:14] doubleValue] / 10.0;
        m_currentTempHM3 = [[serviceArray objectAtIndex:15] doubleValue] / 10.0;
        m_currentTempHM4 = [[serviceArray objectAtIndex:16] doubleValue] / 10.0;
        m_currentTempHM5 = [[serviceArray objectAtIndex:17] doubleValue] / 10.0;
        m_currentTempHM6 = [[serviceArray objectAtIndex:18] doubleValue] / 10.0;
        // Stored temp hourmeters OH1-OH6
        m_storedTempHM1 = [[serviceArray objectAtIndex:19] doubleValue] / 10.0;
        m_storedTempHM2 = [[serviceArray objectAtIndex:20] doubleValue] / 10.0;
        m_storedTempHM3 = [[serviceArray objectAtIndex:21] doubleValue] / 10.0;
        m_storedTempHM4 = [[serviceArray objectAtIndex:22] doubleValue] / 10.0;
        m_storedTempHM5 = [[serviceArray objectAtIndex:23] doubleValue] / 10.0;
        m_storedTempHM6 = [[serviceArray objectAtIndex:24] doubleValue] / 10.0;
        // Maintenance data
        m_lastOilChange = [[serviceArray objectAtIndex:25] doubleValue] / 10.0;
        m_lastOilFilterChange = [[serviceArray objectAtIndex:26] doubleValue] / 10.0;
        m_oilAge = [[serviceArray objectAtIndex:27] doubleValue] / 10.0;

        success = TRUE;
    }
    return success;
}

// Strings 
// Needs translation

- (NSArray *)shutdownDataFromString:(NSString *)shutdownString
{
    // Generates an array containing the shutdown and hours at shutdown strings
    NSString *sd = [self shutdownStringFromCode:[[shutdownString substringToIndex:2] intValue]];

    NSString *sdHoursStr = [shutdownString substringFromIndex:2];
    double sdHours = [sdHoursStr doubleValue] / 10.0f;
    sdHoursStr = [NSString stringWithFormat:@"%0.1f Hours",sdHours];
    NSArray *array = [[NSArray alloc]initWithObjects:sd, sdHoursStr, nil];
    return array;
}

- (NSString *)shutdownStringFromCode:(int)code
{
    NSString *shutdownString = @"";

    switch (code) {
        case 0:
            shutdownString = @"No Fault";
            break;
        case 5:
            shutdownString = @"Fan Overload";
            break;
        case 6:
            shutdownString = @"Main Overload";
            break;
        case 7:
            shutdownString = @"High Vibration";
            break;
        case 8:
            shutdownString = @"Emergency Stop";
            break;
        case 9:
            shutdownString = @"Open System Pressure Transducer";
            break;
        case 10:
            shutdownString = @"I/O Power (CN-8)";
            break;
        case 11:
            shutdownString = @"High System Pressure";
            break;
        case 12:
            shutdownString = @"Shorted System Pressure Transducer";
            break;
        case 13:
            shutdownString = @"Open Reservoir Pressure Transducer";
            break;
        case 14:
            shutdownString = @"High Reservoir Pressure";
            break;
        case 15:
            shutdownString = @"Shorted Reservoir Presssure Transducer";
            break;
        case 16:
            shutdownString = @"Open Air End Thermistor";
            break;
        case 17:
            shutdownString = @"High Separator Temperature";
            break;
        case 18:
            shutdownString = @"Shorted Air End Thermistor";
            break;
        case 19:
            shutdownString = @"Open Separator Thermistor";
            break;
        case 20:
            shutdownString = @"High Air End Temperature";
            break;
        case 21:
            shutdownString = @"Shorted Separator Thermistor";
            break;
        case 23:
            shutdownString = @"Change Separator";
            break;
        case 24:
            shutdownString = @"Fan Starter Error";
            break;
        case 25:
            shutdownString = @"Main Starter Error";
            break;
        case 26:
            shutdownString = @"Low Oil Pressure";
            break;
        case 27:
            shutdownString = @"High Separator Temperature Rise";
            break;
        case 28:
            shutdownString = @"High Air End Temperature Rise";
            break;
        case 29:
            shutdownString = @"Zero System Pressure Transducer";
            break;
        case 30:
            shutdownString = @"Zero Reservoir Pressure Transducer";
            break;
        case 31:
            shutdownString = @"Fan Starter Jammed";
            break;
        case 32:
            shutdownString = @"Main Starter Jammed";
            break;
        case 33:
            shutdownString = @"Open Amp Sensor";
            break;
        case 34:
            shutdownString = @"Shorted Amp Sensor";
            break;
        case 35:
            shutdownString = @"External Device";
            break;
        case 36:
            shutdownString = @"Phase Relay";
            break;
        case 37:
            shutdownString = @"Low Voltage Relay";
            break;
        case 38:
            shutdownString = @"Water Pressure Switch";
            break;
        case 39:
            shutdownString = @"Motor Overtemp Switch";
            break;
        case 81:
            shutdownString = @"EEPROM Checksum Error";
            break;
        case 82:
            shutdownString = @"Analog Failure (CN-7)";
            break;
        default:
            shutdownString = @"Error";
            break;
    }
    return shutdownString;
}

- (NSString *)modeStringFromCode:(int)code
{
    NSString *mode = @"";

    switch (code) {
        case 0:
            mode = @"Stop/Ready";
            break;
        case 1:
            mode = @"Constant Run";
            break;
        case 2:
            mode = @"Low Demand";
            break;
        case 3:
            mode = @"Automatic";
            break;
        case 4:
            mode = @"Sequence";
            break;
        case 5:
            mode = @"Shutdown";
            break;
        default:
            mode = @"Not Available";
            break;
    }
    return mode;
}

- (NSString *)stateStringFromCode:(int)code
{
    NSString *state;

    switch (code) {
        case 1:
            state = @"Auto Restart Sequence";
            break;
        case 2:
            state = @"Shutdown";
            break;
        case 3:
            state = @"Ready";
            break;
        case 4:
            state = @"Enabled";
            break;
        case 5:
            state = @"Auto Enabled";
            break;
        case 6:
            state = @"Not Blowndown";
            break;
        case 7:
            state = @"Prestart";
            break;
        case 8:
            state = @"Start";
            break;
        case 9:
            state = @"Pause";
            break;
        case 10:
            state = @"Run";
            break;
        case 11:
            state = @"Close BDV";
            break;
        case 12:
            state = @"Full Load";
            break;
        case 13:
            state = @"Modulating";
            break;
        case 14:
            state = @"Normal Stop";
            break;
        case 15:
            state = @"Remote Unload";
            break;
        case 16:
            state = @"Remote Halt";
            break;
        case 17:
            state = @"Shutdown Sequence";
            break;
        default:
            state = @"Not Available";
            break;
    }
    return state;
}

- (NSString *)advisoryStringFromCode:(int)code
{
    NSString *advisory = @"";

    switch (code) {
        case 0:
            advisory = @"Change Separator";
            break;
        case 1:
            advisory = @"Change Air Filter";
            break;
        case 2:
            advisory = @"Change Oil Filter";
            break;
        case 3:
            advisory = @"Change Oil";
            break;
        case 4:
            advisory = @"Low Ambient A (Air End)";
            break;
        case 5:
            advisory = @"Low Ambient B (Separator)";
            break;
        case 6:
            advisory = @"High Discharge Temperature";
            break;
        case 7:
            advisory = @"High Reservoir Temperature";
            break;
        case 8:
            advisory = @"High Motor Amps";
            break;
        case 9:
            advisory = @"Check Comm Port";
            break;
        default:
            advisory = @"";
            break;
    }
    return advisory;
}

@end
