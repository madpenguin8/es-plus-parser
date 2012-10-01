#ifndef ESPLUSDATAPARSER_H
#define ESPLUSDATAPARSER_H

#include <QObject>
#include <QStringList>

class ESPlusDataParser : public QObject
{
    Q_OBJECT
public:
    explicit ESPlusDataParser(QObject *parent = 0);
    ~ESPlusDataParser();

    double dischargePressure() const {return m_dischargePressure;}
    double reservoirPressure() const {return m_reservoirPressure;}
    double dischargeTemperature() const {return m_dischargeTemperature;}
    double reservoirTemperature() const {return m_reservoirTemperature;}
    double runHours() const {return m_runHours;}
    double loadHours() const {return m_loadHours;}
    double currentTempHM1() const {return m_currentTempHM1;}
    double currentTempHM2() const {return m_currentTempHM2;}
    double currentTempHM3() const {return m_currentTempHM3;}
    double currentTempHM4() const {return m_currentTempHM4;}
    double currentTempHM5() const {return m_currentTempHM5;}
    double currentTempHM6() const {return m_currentTempHM6;}
    double storedTempHM1() const {return m_storedTempHM1;}
    double storedTempHM2() const {return m_storedTempHM2;}
    double storedTempHM3() const {return m_storedTempHM3;}
    double storedTempHM4() const {return m_storedTempHM4;}
    double storedTempHM5() const {return m_storedTempHM5;}
    double storedTempHM6() const {return m_storedTempHM6;}
    double lastOilChange() const {return m_lastOilChange;}
    double lastOilFilterChange() const {return m_lastOilFilterChange;}
    double oilAge() const {return m_oilAge;}

    uint mainMotorAmps() const {return m_mainMotorAmps;}
    uint sequenceNumber() const {return m_sequenceNumber;}

    QString operatingMode() const {return m_operatingMode;}
    QString operatingState() const {return m_operatingState;}

    QStringList shutdowns() const {return m_shutdowns;}
    QStringList shutdownHours() const {return m_shutdownHours;}
    QStringList advisories() const {return m_advisories;}

    bool parseAdvisoryString(QString advisoryString);
    bool parseOperatingDataString(QString operatingDataString);
    bool parseOperatingModeString(QString operatingModeString);
    bool parseServiceDataString(QString serviceDataString);

private:
    QString modeStringFromCode(int code);
    QString stateStringFromCode(int code);
    QString advisoryStringFromCode(int code);
    QString shutdownStringFromCode(int code);

    double m_dischargePressure;
    double m_reservoirPressure;
    double m_dischargeTemperature;
    double m_reservoirTemperature;
    double m_runHours;
    double m_loadHours;
    double m_currentTempHM1;
    double m_currentTempHM2;
    double m_currentTempHM3;
    double m_currentTempHM4;
    double m_currentTempHM5;
    double m_currentTempHM6;
    double m_storedTempHM1;
    double m_storedTempHM2;
    double m_storedTempHM3;
    double m_storedTempHM4;
    double m_storedTempHM5;
    double m_storedTempHM6;
    double m_lastOilChange;
    double m_lastOilFilterChange;
    double m_oilAge;

    uint m_mainMotorAmps;
    uint m_sequenceNumber;

    QString m_operatingMode;
    QString m_operatingState;

    QStringList m_shutdowns;
    QStringList m_shutdownHours;
    QStringList m_advisories;
};

#endif // ESPLUSDATAPARSER_H
