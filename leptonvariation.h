#ifndef LEPTONVARIATION_H
#define LEPTONVARIATION_H

#include <QObject>
#include "LEPTON_Types.h"
#include "LEPTON_ErrorCodes.h"
#include "LEPTON_AGC.h"
#include "LEPTON_OEM.h"
#include "LEPTON_RAD.h"
#include "LEPTON_SYS.h"
#include "LEPTON_VID.h"

#include <libuvc/libuvc.h>

#include "abstractccinterface.h"
#include "leptonvariation_types.h"

#include <functional>
using namespace std;
using namespace std::placeholders;
using namespace LEP;

#define SDK_SIMPLE_PROPERTY(type, sdk_type, name, sdk_name) \
    Q_PROPERTY(type name \
               READ (bind_get<sdk_type, type>(LEP_Get##sdk_name)) \
               WRITE (bind_set<sdk_type, type>(LEP_Set##sdk_name, (bind(&LeptonVariation::name##Changed, _t, _1)))) \
               NOTIFY name##Changed)

#define SDK_ENUM_PROPERTY(type, name, sdk_name) \
    SDK_SIMPLE_PROPERTY(type, LEP_##type, name, sdk_name)

#define SDK_UINT16_PROPERTY(name, sdk_name) \
    SDK_SIMPLE_PROPERTY(unsigned int, uint16_t, name, sdk_name)

class LeptonVariation : public AbstractCCInterface
{
    Q_OBJECT
public:
    LeptonVariation(uvc_context_t *ctx,
                    uvc_device_t *dev,
                    uvc_device_handle_t *devh);

    LEP_RESULT UVC_GetAttribute(LEP_COMMAND_ID commandID,
                                LEP_ATTRIBUTE_T_PTR attributePtr,
                                LEP_UINT16 attributeWordLength);

    LEP_RESULT UVC_SetAttribute(LEP_COMMAND_ID commandID,
                                LEP_ATTRIBUTE_T_PTR attributePtr,
                                LEP_UINT16 attributeWordLength);

    LEP_RESULT UVC_RunCommand(LEP_COMMAND_ID commandID);

    LEP_CAMERA_PORT_DESC_T_PTR GetPortDescription() { return &m_portDesc; }

    virtual const AbstractCCInterface& operator =(const AbstractCCInterface&);

    SDK_ENUM_PROPERTY(AGC_ENABLE_E, agcEnable, AgcEnableState)
    SDK_ENUM_PROPERTY(AGC_POLICY_E, agcPolicy, AgcPolicy)
    SDK_ENUM_PROPERTY(AGC_HEQ_SCALE_FACTOR_E, agcHeqScaleFactor, AgcHeqScaleFactor)
    SDK_ENUM_PROPERTY(AGC_ENABLE_E, agcCalcEnable, AgcCalcEnableState)

    SDK_UINT16_PROPERTY(agcLinearHistogramTailSize, AgcLinearHistogramTailSize)
    SDK_UINT16_PROPERTY(agcLinearHistogramClipPercent, AgcLinearHistogramClipPercent)
    SDK_UINT16_PROPERTY(agcLinearMaxGain, AgcLinearMaxGain)
    SDK_UINT16_PROPERTY(agcLinearMidPoint, AgcLinearMidPoint)
    SDK_UINT16_PROPERTY(agcLinearDampeningFactor, AgcLinearDampeningFactor)

    SDK_UINT16_PROPERTY(agcHeqDampingFactor, AgcHeqDampingFactor)
    SDK_UINT16_PROPERTY(agcHeqMaxGain, AgcHeqMaxGain)
    SDK_UINT16_PROPERTY(agcHeqClipLimitHigh, AgcHeqClipLimitHigh)
    SDK_UINT16_PROPERTY(agcHeqClipLimitLow, AgcHeqClipLimitLow)
    SDK_UINT16_PROPERTY(agcHeqBinExtension, AgcHeqBinExtension)
    SDK_UINT16_PROPERTY(agcHeqMidPoint, AgcHeqMidPoint)
    SDK_UINT16_PROPERTY(agcHeqEmptyCount, AgcHeqEmptyCount)
    SDK_UINT16_PROPERTY(agcHeqNormalizationFactor, AgcHeqNormalizationFactor)

    Q_PROPERTY(const QString sysFlirSerialNumber READ getSysFlirSerialNumber)
    const QString getSysFlirSerialNumber();

    Q_PROPERTY(const QString oemFlirPartNumber READ getOemFlirPartNumber)
    const QString getOemFlirPartNumber();

    Q_PROPERTY(const QString oemGppSoftwareVersion READ getOemGppSoftwareVersion)
    const QString getOemGppSoftwareVersion();

    Q_PROPERTY(const QString oemDspSoftwareVersion READ getOemDspSoftwareVersion)
    const QString getOemDspSoftwareVersion();

    SDK_ENUM_PROPERTY(PCOLOR_LUT_E, vidPcolorLut, VidPcolorLut)
    SDK_ENUM_PROPERTY(POLARITY_E, vidPolarity, VidPolarity)
    SDK_ENUM_PROPERTY(VID_SBNUC_ENABLE_E, vidSbNucEnableState, VidSbNucEnableState)

signals:

    void agcEnableChanged(AGC_ENABLE_E val);
    void agcPolicyChanged(AGC_POLICY_E val);
    void agcHeqScaleFactorChanged(AGC_HEQ_SCALE_FACTOR_E val);
    void agcCalcEnableChanged(AGC_ENABLE_E val);

    void agcLinearHistogramTailSizeChanged(uint16_t val);
    void agcLinearHistogramClipPercentChanged(uint16_t val);
    void agcLinearMaxGainChanged(uint16_t val);
    void agcLinearMidPointChanged(uint16_t val);
    void agcLinearDampeningFactorChanged(uint16_t val);

    void agcHeqDampingFactorChanged(uint16_t val);
    void agcHeqMaxGainChanged(uint16_t val);
    void agcHeqClipLimitHighChanged(uint16_t val);
    void agcHeqClipLimitLowChanged(uint16_t val);
    void agcHeqBinExtensionChanged(uint16_t val);
    void agcHeqMidPointChanged(uint16_t val);
    void agcHeqEmptyCountChanged(uint16_t val);
    void agcHeqNormalizationFactorChanged(uint16_t val);

    void vidPcolorLutChanged(PCOLOR_LUT_E val);
    void vidPolarityChanged(POLARITY_E val);
    void vidSbNucEnableStateChanged(VID_SBNUC_ENABLE_E val);

public slots:
    virtual void performFfc();

private:

    template <class T, class W>
    function<W(void)> bind_get(function<LEP_RESULT(LEP_CAMERA_PORT_DESC_T_PTR, T*)> F)
    {
        return bind(&LeptonVariation::pget<T, W>, this, F);
    }

    template <class T, class W>
    function<void(W)> bind_set(function<LEP_RESULT(LEP_CAMERA_PORT_DESC_T_PTR, T)> F,
                               function<void(W)> E)
    {
        return bind(&LeptonVariation::pset<T, W>, this, F, E, _1);
    }

    template <class T, class W>
    W pget(function<LEP_RESULT(LEP_CAMERA_PORT_DESC_T_PTR, T*)> F)
    {
        T var;
        F(&m_portDesc, &var);
        return (W)var;
    }

    template <class T, class W>
    void pset(function<LEP_RESULT(LEP_CAMERA_PORT_DESC_T_PTR, T)> F, function<void(W)> E, W var)
    {
        F(&m_portDesc, (T)var);
        emit E(var);
    }

    uvc_context_t *ctx;
    uvc_device_t *dev;
    uvc_device_handle_t *devh;
    LEP_CAMERA_PORT_DESC_T m_portDesc;

    int leptonCommandIdToUnitId(LEP_COMMAND_ID commandID);
};

Q_DECLARE_METATYPE(PCOLOR_LUT_E)
Q_DECLARE_METATYPE(POLARITY_E)
Q_DECLARE_METATYPE(VID_SBNUC_ENABLE_E)
Q_DECLARE_METATYPE(AGC_ENABLE_E)
Q_DECLARE_METATYPE(AGC_POLICY_E)
Q_DECLARE_METATYPE(AGC_HEQ_SCALE_FACTOR_E)

#endif // LEPTONVARIATION_H