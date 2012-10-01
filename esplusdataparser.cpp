#include "esplusdataparser.h"

static const int OP_MODE_OLD = 1;
static const int OP_MODE_NEW = 4;
static const int OP_DATA_LENGTH = 19;
static const int ADVISORY_DATA_LENGTH = 4;
static const int SERVICE_DATA_LENGTH = 214;

ESPlusDataParser::ESPlusDataParser(QObject *parent) :
    QObject(parent)
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

    m_operatingMode = "Not Available";
    m_operatingState = "Not Available";

    m_shutdowns = QStringList();
    m_shutdownHours = QStringList();
    m_advisories = QStringList();
}

ESPlusDataParser::~ESPlusDataParser()
{

}

bool
ESPlusDataParser::parseAdvisoryString(QString advisoryString)
{
    bool success = false;

    if (advisoryString.length() == ADVISORY_DATA_LENGTH) {
        uint dec = advisoryString.toUInt(&success, 16);     // hex == 255, ok == true
        if (success){ //Convert 4 digit hex to int
            QStringList advisoryList;
            int count = 0;
            for(int i = dec; i > 0; i >>= 1)
            { // Inspect each bit to see if it is high.
                if(i & 1){
                    advisoryList.append(advisoryStringFromCode(count));
                }
                count++;
            }
            m_advisories = advisoryList;
        }
    }
    return success;
}

bool
ESPlusDataParser::parseOperatingDataString(QString operatingDataString)
{
    bool success = FALSE;

    // Reset to zero
    m_dischargePressure = 0.0;
    m_reservoirPressure = 0.0;
    m_dischargeTemperature = 0.0;
    m_reservoirTemperature = 0.0;
    m_mainMotorAmps = 0;

    uint disPDec = 0;
    uint resPDec = 0;
    uint disTDec = 0;
    uint resTDec = 0;
    uint ampsDec = 0;

    if (operatingDataString.length() == OP_DATA_LENGTH) {
        QString disPressString = operatingDataString.mid(0,4);
        QString resPressString = operatingDataString.mid(4,4);
        QString disTempString = operatingDataString.mid(8,4);
        QString resTempString = operatingDataString.mid(12,4);
        QString ampsString = operatingDataString.mid(16,3);

        disPDec = disPressString.toUInt(&success, 16);
        m_dischargePressure = disPDec / 16.00;

        resPDec = resPressString.toUInt(&success, 16);
        m_reservoirPressure = resPDec / 16.00;

        disTDec = disTempString.toUInt(&success, 16);
        m_dischargeTemperature = disTDec / 16.00;

        resTDec = resTempString.toUInt(&success, 16);
        m_reservoirTemperature = resTDec / 16.00;

        ampsDec = ampsString.toUInt(&success, 16);
        m_mainMotorAmps = ampsDec;

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
        success = true;
    }
    return success;
}

bool
ESPlusDataParser::parseOperatingModeString(QString operatingModeString)
{
    bool success = false;

    int modeDataLength = operatingModeString.length();

    if (modeDataLength == OP_MODE_OLD) {
        /* Versions prior to 2.10 do not provide operating state
                or sequence number so we define them as not available. */
        uint dec;
        dec = operatingModeString.toUInt(&success, 16);
        if (success){
            m_operatingMode = modeStringFromCode(dec);
            m_operatingState = "Not Available";
            m_sequenceNumber = 0;
        }
    }
    else if (modeDataLength == OP_MODE_NEW) {
        QString opMode = operatingModeString.mid(0,1);
        QString sequenceStr = operatingModeString.mid(1,1);
        QString opState = operatingModeString.mid(2,2);

        uint omDec;
        omDec = opMode.toUInt(&success, 16);
        m_operatingMode = modeStringFromCode(omDec);

        uint seqDec;
        seqDec = sequenceStr.toUInt(&success, 16);
        m_sequenceNumber = seqDec;

        uint osDec;
        osDec = opState.toUInt(&success, 16);
        m_operatingState = stateStringFromCode(osDec);
    }
    else {
        m_operatingMode = "Not Available";
        m_operatingState = "Not Available";
        m_sequenceNumber = 0;
    }
    return success;
}

bool
ESPlusDataParser::parseServiceDataString(QString serviceDataString)
{
    bool success = false;

    // Reset to zero
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
    m_shutdowns.clear();
    m_shutdownHours.clear();

    if (serviceDataString.length() == SERVICE_DATA_LENGTH) {
        // Remove pesky '$' delimiters.
        QString fixedServiceDataStr = serviceDataString;
        fixedServiceDataStr.replace(QString("$"), QString("#"));

        QStringList serviceList = fixedServiceDataStr.split("#");
        parseAdvisoryString(serviceList[0]);

        for (int i = 1; i < 11; i++) {
            QString str = serviceList[i];
            // Only add shutdowns to the lists if they contain data
            if(!str.startsWith("00")){
                bool ok = false;
                QString sdCode = str.mid(0,2);
                QString sd = shutdownStringFromCode(sdCode.toUInt(&ok, 10));
                m_shutdowns.append(sd);
                // Generates a list containing the shutdown and hours at shutdown strings
                QString sdHoursStr = str.mid(2);
                double hours = sdHoursStr.toUInt(&ok, 10) / 10.0;
                QString sdHours = QString("%1 Hours").arg(hours);
                m_shutdownHours.append(sdHours);
            }
        }

        // Operating hourmeters
        m_runHours = serviceList[11].toUInt(&success, 10) / 10.0;
        m_loadHours = serviceList[12].toUInt(&success, 10) / 10.0;

        // Current temp hourmeters H1-H6
        m_currentTempHM1 = serviceList[13].toUInt(&success, 10) / 10.0;
        m_currentTempHM2 = serviceList[14].toUInt(&success, 10) / 10.0;
        m_currentTempHM3 = serviceList[15].toUInt(&success, 10) / 10.0;
        m_currentTempHM4 = serviceList[16].toUInt(&success, 10) / 10.0;
        m_currentTempHM5 = serviceList[17].toUInt(&success, 10) / 10.0;
        m_currentTempHM6 = serviceList[18].toUInt(&success, 10) / 10.0;

        // Stored temp hourmeters OH1-OH6
        m_storedTempHM1 = serviceList[19].toUInt(&success, 10) / 10.0;
        m_storedTempHM2 = serviceList[20].toUInt(&success, 10) / 10.0;
        m_storedTempHM3 = serviceList[21].toUInt(&success, 10) / 10.0;
        m_storedTempHM4 = serviceList[22].toUInt(&success, 10) / 10.0;
        m_storedTempHM5 = serviceList[23].toUInt(&success, 10) / 10.0;
        m_storedTempHM6 = serviceList[24].toUInt(&success, 10) / 10.0;

        // Maintenance data
        m_lastOilChange = serviceList[25].toUInt(&success, 10) / 10.0;
        m_lastOilFilterChange = serviceList[26].toUInt(&success, 10) / 10.0;
        m_oilAge = serviceList[27].toUInt(&success, 10) / 10.0;

        success = true;
    }
    return success;
}

QString
ESPlusDataParser::modeStringFromCode(int code)
{
    QString mode = "";

    switch (code) {
        case 0:
            mode = "Stop/Ready";
            break;
        case 1:
            mode = "Constant Run";
            break;
        case 2:
            mode = "Low Demand";
            break;
        case 3:
            mode = "Automatic";
            break;
        case 4:
            mode = "Sequence";
            break;
        case 5:
            mode = "Shutdown";
            break;
        default:
            mode = "Not Available";
            break;
    }
    return mode;
}

QString
ESPlusDataParser::stateStringFromCode(int code)
{
    QString state = "";

    switch (code) {
        case 1:
            state = "Auto Restart Sequence";
            break;
        case 2:
            state = "Shutdown";
            break;
        case 3:
            state = "Ready";
            break;
        case 4:
            state = "Enabled";
            break;
        case 5:
            state = "Auto Enabled";
            break;
        case 6:
            state = "Not Blowndown";
            break;
        case 7:
            state = "Prestart";
            break;
        case 8:
            state = "Start";
            break;
        case 9:
            state = "Pause";
            break;
        case 10:
            state = "Run";
            break;
        case 11:
            state = "Close BDV";
            break;
        case 12:
            state = "Full Load";
            break;
        case 13:
            state = "Modulating";
            break;
        case 14:
            state = "Normal Stop";
            break;
        case 15:
            state = "Remote Unload";
            break;
        case 16:
            state = "Remote Halt";
            break;
        case 17:
            state = "Shutdown Sequence";
            break;
        default:
            state = "Not Available";
            break;
    }
    return state;
}

QString
ESPlusDataParser::advisoryStringFromCode(int code)
{
    QString advisory = "";

    switch (code) {
        case 0:
            advisory = "Change Separator";
            break;
        case 1:
            advisory = "Change Air Filter";
            break;
        case 2:
            advisory = "Change Oil Filter";
            break;
        case 3:
            advisory = "Change Oil";
            break;
        case 4:
            advisory = "Low Ambient A (Air End)";
            break;
        case 5:
            advisory = "Low Ambient B (Separator)";
            break;
        case 6:
            advisory = "High Discharge Temperature";
            break;
        case 7:
            advisory = "High Reservoir Temperature";
            break;
        case 8:
            advisory = "High Motor Amps";
            break;
        case 9:
            advisory = "Check Comm Port";
            break;
        default:
            advisory = "";
            break;
    }
    return advisory;
}

QString
ESPlusDataParser::shutdownStringFromCode(int code)
{
    QString shutdownString = "";

    switch (code) {
        case 0:
            shutdownString = "No Fault";
            break;
        case 5:
            shutdownString = "Fan Overload";
            break;
        case 6:
            shutdownString = "Main Overload";
            break;
        case 7:
            shutdownString = "High Vibration";
            break;
        case 8:
            shutdownString = "Emergency Stop";
            break;
        case 9:
            shutdownString = "Open System Pressure Transducer";
            break;
        case 10:
            shutdownString = "I/O Power (CN-8)";
            break;
        case 11:
            shutdownString = "High System Pressure";
            break;
        case 12:
            shutdownString = "Shorted System Pressure Transducer";
            break;
        case 13:
            shutdownString = "Open Reservoir Pressure Transducer";
            break;
        case 14:
            shutdownString = "High Reservoir Pressure";
            break;
        case 15:
            shutdownString = "Shorted Reservoir Presssure Transducer";
            break;
        case 16:
            shutdownString = "Open Air End Thermistor";
            break;
        case 17:
            shutdownString = "High Separator Temperature";
            break;
        case 18:
            shutdownString = "Shorted Air End Thermistor";
            break;
        case 19:
            shutdownString = "Open Separator Thermistor";
            break;
        case 20:
            shutdownString = "High Air End Temperature";
            break;
        case 21:
            shutdownString = "Shorted Separator Thermistor";
            break;
        case 23:
            shutdownString = "Change Separator";
            break;
        case 24:
            shutdownString = "Fan Starter Error";
            break;
        case 25:
            shutdownString = "Main Starter Error";
            break;
        case 26:
            shutdownString = "Low Oil Pressure";
            break;
        case 27:
            shutdownString = "High Separator Temperature Rise";
            break;
        case 28:
            shutdownString = "High Air End Temperature Rise";
            break;
        case 29:
            shutdownString = "Zero System Pressure Transducer";
            break;
        case 30:
            shutdownString = "Zero Reservoir Pressure Transducer";
            break;
        case 31:
            shutdownString = "Fan Starter Jammed";
            break;
        case 32:
            shutdownString = "Main Starter Jammed";
            break;
        case 33:
            shutdownString = "Open Amp Sensor";
            break;
        case 34:
            shutdownString = "Shorted Amp Sensor";
            break;
        case 35:
            shutdownString = "External Device";
            break;
        case 36:
            shutdownString = "Phase Relay";
            break;
        case 37:
            shutdownString = "Low Voltage Relay";
            break;
        case 38:
            shutdownString = "Water Pressure Switch";
            break;
        case 39:
            shutdownString = "Motor Overtemp Switch";
            break;
        case 81:
            shutdownString = "EEPROM Checksum Error";
            break;
        case 82:
            shutdownString = "Analog Failure (CN-7)";
            break;
        default:
            shutdownString = "Error";
            break;
    }
    return shutdownString;
}
